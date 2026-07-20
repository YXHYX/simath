#include "example_demos.h"

#include <iostream>
#include "tools.h"
#include "Graphics.h"
#include "resource/ResourceManager.h"
#include <chrono>
#include "util/video_decode.h"
//for microseconds delay
#include "util/delays.h"

//Simple video player in cmd
// todo:
// dithering
// color correction ?
// 
// fix AV1 encoding it sucks
// anti aliasing


using namespace graphics;
const char * asciiShade = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'.       ";
auto asciiSize =76;

void process_frame(AVFrame* frame, Graphics* graph) {
    // Let's assume YUV420p format
    int width = frame->width;
    int height = frame->height;
    int gwidth = graph->getBufferSize().X, gheight = graph->getBufferSize().Y;

    // 1. Accessing the Luminance (Y) Plane (The full image)
    uint8_t* y_plane = frame->data[0];
    int y_stride = frame->linesize[0];

    uint8_t* u_plane = frame->data[1];
    uint8_t* v_plane = frame->data[2];
    int uv_stride = frame->linesize[1];

    for (int j{}; j < gheight; j++)
    {
        float pixelY = height * float(j) / gheight;
        uint8_t* row_ptr = y_plane + ((int)pixelY * y_stride);
        uint8_t* row_u = u_plane + ((int)(pixelY / 2.f) * uv_stride); // UV takes HALF image
        uint8_t* row_v = v_plane + ((int)(pixelY / 2.f) * uv_stride); // UV takes HALF image

        for (int i{}; i < gwidth; i++)
        {

            const auto startFRAMEPROCESS{ std::chrono::steady_clock::now() };
            //conserve the ASPECT RATIO for later
            float pixelX = width * float(i) / gwidth;
            //convert pixel surrounding

            uint8_t u_value = row_u[int(pixelX/2.f)];
            uint8_t v_value = row_v[int(pixelX/2.f)];
            //get brightness of pixel and pass it through the map
            int brightness = row_ptr[(int)pixelX];
            //require RGB
            uint8_t r, g, b;

            //Conversion
            r = brightness + 1.402 * (v_value-128.0);
            g = brightness - 0.344136 * (u_value - 128.0) - 0.714136 * (v_value - 128.0);
            b = brightness + 1.772 * (u_value - 128.0);
            
            
            /*/then use the map from texture to get the closest attribute
            //simple round
            r = r >= 127 ? (r > 191 ? 255 : 127) : (r > 64 ? 127 : 0);
            b = b >= 127 ? (b > 191 ? 255 : 127) : (b > 64 ? 127 : 0);
            g = g >= 127 ? (g > 191 ? 255 : 127) : (g > 64 ? 127 : 0);
            int index = 0;
            for (; index < 16; index++)
                if (FIXED_COLORS[index] == vec3d(r, g, b))
                    break;
            */
            unsigned short attr = Texture::colorMap(r,g,b);//index > 7 ? (index << 4 ): index;// // take surrounding pixel colors


            int characterShaded = (255.f-float(brightness))/ 255.f * float(asciiSize);

            graph->setPixel(i, j, asciiShade[characterShaded], attr);
            const auto finishFRAMEPROCESS{ std::chrono::steady_clock::now() };
            const std::chrono::duration<double> elapsed_seconds{ finishFRAMEPROCESS - startFRAMEPROCESS };
            
        }
    }
}


enum class VideoState{ PLAY, PAUSE, REWIND, SKIP};

int video_player_demo()
{
	graphics::Graphics graph;
	//ResourceManager resourceManager;

	float elapsedTime = 0;
	float totalTime = 0.0001;

    std::string filename = "./res/videos/excuseme.mp4";
	
    // open video and retrieve information
    AVFormatContext* format_ctx = nullptr;
    if (avformat_open_input(&format_ctx, filename.c_str(), nullptr, nullptr) < 0) return -1;
    avformat_find_stream_info(format_ctx, nullptr);

    // stream indeices and setting up decoders
    int video_stream_idx = -1, audio_stream_idx = -1;
    for (unsigned int i = 0; i < format_ctx->nb_streams; i++) {
        if (format_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO && video_stream_idx < 0)
            video_stream_idx = i;
        
        //WORK ON AUDIO TOO!!!
        //if (format_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO && audio_stream_idx < 0)
            //audio_stream_idx = i;
    }

    // Initialize Video Decoder (Prefer GPU-accelerated decoders like h264_cuvid / h264_qsv)
    AVCodecParameters* v_params = format_ctx->streams[video_stream_idx]->codecpar;
    
    const AVCodec* video_codec = avcodec_find_decoder(v_params->codec_id);


    // Optimization: bind this to NVDEC (CUDA) or VAAPI.
    AVCodecContext* video_ctx = avcodec_alloc_context3(video_codec);
    avcodec_parameters_to_context(video_ctx, v_params);
    video_ctx->thread_count = 0; // 0 = auto-select optimal CPU thread count
    

    if (avcodec_open2(video_ctx, video_codec, nullptr) < 0)
        return -1;

    // Initialize Audio Decoder
    /* later
    AVCodecParameters* a_params = format_ctx->streams[audio_stream_idx]->codecpar;
    const AVCodec* audio_codec = avcodec_find_decoder(a_params->codec_id);
    AVCodecContext* audio_ctx = avcodec_alloc_context3(audio_codec);
    avcodec_parameters_to_context(audio_ctx, a_params);
    avcodec_open2(audio_ctx, audio_codec, nullptr);
    */

    //allocate frames and packets
    AVPacket* packet = av_packet_alloc();
    AVFrame* video_frame = av_frame_alloc();
    //AVFrame* audio_frame = av_frame_alloc();
    
    //video player state
    VideoState video_state = VideoState::PLAY;
    bool playing = true;

    while (true)
	{

		Sleep(1);
        //inverse framerate to maintain fps
        double inv_framerate = 40;// double(video_ctx->framerate.den) / double(video_ctx->framerate.num);
        //delay add up in case renderer cant keep up with processing the frames
        int laggingtime = 0;
        // demux and decode
        while (playing && av_read_frame(format_ctx, packet) >= 0) {
            const auto start{ std::chrono::steady_clock::now() };
            if (packet->stream_index == video_stream_idx) {
                // pass the compressed packet to the video decoder
                if (avcodec_send_packet(video_ctx, packet) == 0) {
                    while (avcodec_receive_frame(video_ctx, video_frame) == 0) {
                        //May implement an offset indice lookup table to increase speed
                        const auto startFRAMEPROCESS{ std::chrono::steady_clock::now() };
                        // video_frame->data[0], data[1], data[2] raw pixel planes YUV420p
                        // process frame

                        //no need for input
                        //graph.updateInput();
                        graph.clearBuffer();
                        process_frame(video_frame, &graph);

                        //no object or triangle to render
                        //graph.render();


                        //debugging
                        //graph.printGui("TIME ELAPSED: " + std::to_string(elapsed_seconds.count()), vec2i(10, 10), WHITE);

                        graph.display();

                        const auto finishFRAMEPROCESS{ std::chrono::steady_clock::now() };
                        const std::chrono::duration<double, std::milli> elapsed_seconds{ finishFRAMEPROCESS - startFRAMEPROCESS };
                        //maintain 60 fps
                        //block for the remaining time
                        int delay = inv_framerate - elapsed_seconds.count();
                        
                        //if the frame is lagging behind, skip the Sleep maintaining framerate and render the next frame to catchup
                        laggingtime += delay;
                        if (laggingtime < 0)
                        {
                            ;//skip frame
                        }
                        else
                            Sleep(abs(delay));
                        //DelayMicroseconds(0.0167);


                    }
                }
            }
            //until later 150930
            /*else if (packet->stream_index == audio_stream_idx) {
                // Pass the compressed packet to the audio decoder
                if (avcodec_send_packet(audio_ctx, packet) == 0) {
                    while (avcodec_receive_frame(audio_ctx, audio_frame) == 0) {
                        // --- AUDIO SAMPLES RETRIEVED HERE ---
                        // audio_frame->data[0] contains raw PCM samples (e.g. float planar or int16)
                    }
                }
            }*/
            //PAUSE/PLAY/SKIP/REWIND
            if (GetKeyState(VK_SPACE) & 0x8000)
            {
                if (video_state == VideoState::PLAY)
                {
                    video_state = VideoState::PAUSE;
                    playing = false;
                }
            }

            if (GetAsyncKeyState(VK_LEFT) & 0x8000)
            {
                video_state = VideoState::REWIND;
                //rewind by X frames
                format_ctx->seek2any;
                format_ctx->skip_estimate_duration_from_pts;

            }

            /*const auto finish{std::chrono::steady_clock::now()};
            const std::chrono::duration<double> elapsed_seconds{ finish - start };
            elapsedTime = elapsed_seconds.count();
            totalTime += elapsedTime;
            */
            av_packet_unref(packet); // Extremely important: wipe packet data for the next frame
        }
        if (GetKeyState(VK_SPACE) & 0x8000)
        {
            if (video_state == VideoState::PAUSE)
            {
                playing = true;
                video_state = VideoState::PLAY;
            }
        }


		//quit
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
			break;
	}

    // Cleanup
    av_frame_free(&video_frame);
    //av_frame_free(&audio_frame);
    av_packet_free(&packet);
    avcodec_free_context(&video_ctx);
    //avcodec_free_context(&audio_ctx);
    avformat_close_input(&format_ctx);

	return 0;
}
