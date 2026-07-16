#include "example_demos.h"

#include <iostream>
#include "tools.h"
#include "Graphics.h"
#include "resource/ResourceManager.h"
#include <chrono>
#include "util/video_decode.h"

//Simple video player in cmd
// todo:
// dithering
// color correction ?
// encoding 
// everything else

using namespace graphics;
const char * asciiShade = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";
auto asciiSize = sizeof(asciiShade) / sizeof(char);

void process_frame(AVFrame* frame, Graphics* graph) {
    // Let's assume YUV420p format
    int width = frame->width;
    int height = frame->height;

    // 1. Accessing the Luminance (Y) Plane (The full image)
    uint8_t* y_plane = frame->data[0];
    int y_stride = frame->linesize[0];

    uint8_t* u_plane = frame->data[1];
    uint8_t* v_plane = frame->data[2];
    int uv_stride = frame->linesize[1];

    for (int j{}; j < graph->getBufferSize().Y; j++)
    {
        float pixelY = height * float(j) / graph->getBufferSize().Y;
        uint8_t* row_ptr = y_plane + ((int)pixelY * y_stride);
        uint8_t* row_u = u_plane + ((int)(pixelY / 2.f) * uv_stride); // UV takes HALF image
        uint8_t* row_v = v_plane + ((int)(pixelY / 2.f) * uv_stride); // UV takes HALF image

        for (int i{}; i < graph->getBufferSize().X; i++)
        {

            const auto startFRAMEPROCESS{ std::chrono::steady_clock::now() };
            //conserve the ASPECT RATIO for later
            float pixelX = width * float(i) / graph->getBufferSize().X;
            //convert pixel surrounding

            uint8_t u_value = row_u[int(pixelX/2.f)];
            uint8_t v_value = row_v[int(pixelX / 2.f)];
            //get brightness of pixel and pass it through the map
            uint8_t brightness = row_ptr[(int)pixelX];
            //require RGB
            uint8_t r, g, b;

            //Conversion
            r = brightness + 1.402 * (v_value-128.0);
            g = brightness - 0.344136 * (u_value - 128.0) - 0.714136 * (v_value - 128.0);
            b = brightness + 1.772 * (u_value - 128.0);
            
            //clamp
            r = max(min(r,255), 0);
            g = max(min(g, 255), 0);
            b = max(min(b, 255), 0);
            //then use the map from texture to get the closest attribute
            unsigned short attr = Texture::colorMap(r,g,b); // take surrounding pixel colors

            int characterShaded = float(brightness) / 255.f * asciiSize;

            graph->setPixel(i, j, asciiShade[characterShaded], attr);
            const auto finishFRAMEPROCESS{ std::chrono::steady_clock::now() };
            const std::chrono::duration<double> elapsed_seconds{ finishFRAMEPROCESS - startFRAMEPROCESS };
            
        }
    }
}

int video_player_demo()
{
	graphics::Graphics graph;
	//ResourceManager resourceManager;

	float elapsedTime = 0;
	float totalTime = 0.0001;

    std::string filename = "./res/videos/yahai.mp4";
	
    // 1. Open the video/audio container
    AVFormatContext* format_ctx = nullptr;
    if (avformat_open_input(&format_ctx, filename.c_str(), nullptr, nullptr) < 0) return -1;
    avformat_find_stream_info(format_ctx, nullptr);

    // 2. Find Stream Indices & Decoders
    int video_stream_idx = -1, audio_stream_idx = -1;
    for (unsigned int i = 0; i < format_ctx->nb_streams; i++) {
        if (format_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO && video_stream_idx < 0)
            video_stream_idx = i;
        
        //WORK ON AUDIO TOO!!!
        //if (format_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO && audio_stream_idx < 0)
            //audio_stream_idx = i;
    }

    // 3. Initialize Video Decoder (Prefer GPU-accelerated decoders like h264_cuvid / h264_qsv)
    AVCodecParameters* v_params = format_ctx->streams[video_stream_idx]->codecpar;
    
    const AVCodec* video_codec = avcodec_find_decoder(v_params->codec_id);


    // Optimization: In a production build, you'd explicitly bind this to NVDEC (CUDA) or VAAPI here.
    AVCodecContext* video_ctx = avcodec_alloc_context3(video_codec);
    avcodec_parameters_to_context(video_ctx, v_params);
    video_ctx->thread_count = 0; // 0 instructs FFmpeg to auto-select optimal CPU thread count
    // If you don't have libdav1d, try the generic software AV1 decoder:
    
    if (avcodec_open2(video_ctx, video_codec, nullptr) < 0)
        return -1;

    // 4. Initialize Audio Decoder
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
    
    while (true)
	{

		Sleep(1);
		const auto start{ std::chrono::steady_clock::now() };

		
        // 5. The Demux & Decode Loop
        while (av_read_frame(format_ctx, packet) >= 0) {
            if (packet->stream_index == video_stream_idx) {
                // Pass the compressed packet to the video decoder
                if (avcodec_send_packet(video_ctx, packet) == 0) {
                    while (avcodec_receive_frame(video_ctx, video_frame) == 0) {
                        //if its the first frame, calculate the static offsets into a lookup table




                        const auto startFRAMEPROCESS{ std::chrono::steady_clock::now() };
                        // --- VIDEO PIXELS RETRIEVED HERE ---
                        // video_frame->data[0], data[1], data[2] contain the raw pixel planes (usually YUV420p)
                        // process frame

                        //graph.updateInput();
                        graph.clearBuffer();
                        process_frame(video_frame, &graph);

                        //graph.render();


                        const auto finishFRAMEPROCESS{ std::chrono::steady_clock::now() };
                        const std::chrono::duration<double> elapsed_seconds{ finishFRAMEPROCESS - startFRAMEPROCESS };
                        graph.printGui("TIME ELAPSED: " + std::to_string(elapsed_seconds.count()), vec2i(10, 10), WHITE);

                        graph.display();

                    }
                }
            }
            //until later
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


            av_packet_unref(packet); // Extremely important: wipe packet data for the next frame
        }


		
		const auto finish{ std::chrono::steady_clock::now() };
		const std::chrono::duration<double> elapsed_seconds{ finish - start };
		elapsedTime = elapsed_seconds.count();
		totalTime += elapsedTime;

		//quit
		if (GetKeyState(VK_ESCAPE) & 0x8000)
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
