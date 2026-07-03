#include "Sound.h"

void Sound::loadSound(std::string path)
{
    //copied from alplay.c soft openal examples
    int sample_format = 0;
    ALint byteblockalign = 0;
    ALint splblockalign = 0;
    sf_count_t num_frames;
    ALenum err;
    ALenum format;
    ALsizei num_bytes;
    SNDFILE* sndfile;
    SF_INFO sfinfo;
    short* membuf;

    /* Open the audio file and check that it's usable. */
    sndfile = sf_open(path.c_str(), SFM_READ, &sfinfo);
    if (!sndfile)
    {
        throw "Could not open audio : "+ path;
    }
    if (sfinfo.frames < 1)
    {
        sf_close(sndfile);
        throw "Bad sample count in : " + path;
    }

    /* Detect a suitable format to load. Formats like Vorbis and Opus use float
     * natively, so load as float to avoid clipping when possible. Formats
     * larger than 16-bit can also use float to preserve a bit more precision.
     */
    switch ((sfinfo.format & SF_FORMAT_SUBMASK))
    {
    case SF_FORMAT_PCM_24:
    case SF_FORMAT_PCM_32:
    case SF_FORMAT_FLOAT:
    case SF_FORMAT_DOUBLE:
    case SF_FORMAT_VORBIS:
    case SF_FORMAT_OPUS:
    case SF_FORMAT_ALAC_20:
    case SF_FORMAT_ALAC_24:
    case SF_FORMAT_ALAC_32:
    case 0x0080/*SF_FORMAT_MPEG_LAYER_I*/:
    case 0x0081/*SF_FORMAT_MPEG_LAYER_II*/:
    case 0x0082/*SF_FORMAT_MPEG_LAYER_III*/:
        if (alIsExtensionPresent("AL_EXT_FLOAT32"))
            sample_format = 1;
        break;
    case SF_FORMAT_IMA_ADPCM:
        /* ADPCM formats require setting a block alignment as specified in the
         * file, which needs to be read from the wave 'fmt ' chunk manually
         * since libsndfile doesn't provide it in a format-agnostic way.
         */
        if (sfinfo.channels <= 2 && (sfinfo.format & SF_FORMAT_TYPEMASK) == SF_FORMAT_WAV
            && alIsExtensionPresent("AL_EXT_IMA4")
            && alIsExtensionPresent("AL_SOFT_block_alignment"))
            sample_format = 2;
        break;
    case SF_FORMAT_MS_ADPCM:
        if (sfinfo.channels <= 2 && (sfinfo.format & SF_FORMAT_TYPEMASK) == SF_FORMAT_WAV
            && alIsExtensionPresent("AL_SOFT_MSADPCM")
            && alIsExtensionPresent("AL_SOFT_block_alignment"))
            sample_format = 3;
        break;
    }

    if (sample_format == 2 || sample_format == 3)
    {
        /* For ADPCM, lookup the wave file's "fmt " chunk, which is a
         * WAVEFORMATEX-based structure for the audio format.
         */
        SF_CHUNK_INFO inf = { "fmt ", 4, 0, NULL };
        SF_CHUNK_ITERATOR* iter = sf_get_chunk_iterator(sndfile, &inf);

        /* If there's an issue getting the chunk or block alignment, load as
         * 16-bit and have libsndfile do the conversion.
         */
        if (!iter || sf_get_chunk_size(iter, &inf) != SF_ERR_NO_ERROR || inf.datalen < 14)
            sample_format = 0;
        else
        {
            ALubyte* fmtbuf = static_cast<ALubyte*>(calloc(inf.datalen, 1));
            inf.data = fmtbuf;
            if (sf_get_chunk_data(iter, &inf) != SF_ERR_NO_ERROR)
                sample_format = 0;
            else
            {
                /* Read the nBlockAlign field, and convert from bytes- to
                 * samples-per-block (verifying it's valid by converting back
                 * and comparing to the original value).
                 */
                byteblockalign = fmtbuf[12] | (fmtbuf[13] << 8);
                if (sample_format == 2)
                {
                    splblockalign = (byteblockalign / sfinfo.channels - 4) / 4 * 8 + 1;
                    if (splblockalign < 1
                        || ((splblockalign - 1) / 2 + 4) * sfinfo.channels != byteblockalign)
                        sample_format = 0;
                }
                else
                {
                    splblockalign = (byteblockalign / sfinfo.channels - 7) * 2 + 2;
                    if (splblockalign < 2
                        || ((splblockalign - 2) / 2 + 7) * sfinfo.channels != byteblockalign)
                        sample_format = 0;
                }
            }
            free(fmtbuf);
        }
    }

    if (sample_format == 0)
    {
        splblockalign = 1;
        byteblockalign = sfinfo.channels * 2;
    }
    else if (sample_format == 1)
    {
        splblockalign = 1;
        byteblockalign = sfinfo.channels * 4;
    }

    /* Figure out the OpenAL format from the file and desired sample type. */
    format = AL_NONE;
    if (sfinfo.channels == 1)
    {
        if (sample_format == 0)
            format = AL_FORMAT_MONO16;
        else if (sample_format == 1)
            format = AL_FORMAT_MONO_FLOAT32;
        else if (sample_format == 2)
            format = AL_FORMAT_MONO_IMA4;
        else if (sample_format == 3)
            format = AL_FORMAT_MONO_MSADPCM_SOFT;
    }
    else if (sfinfo.channels == 2)
    {
        if (sample_format == 0)
            format = AL_FORMAT_STEREO16;
        else if (sample_format == 1)
            format = AL_FORMAT_STEREO_FLOAT32;
        else if (sample_format == 2)
            format = AL_FORMAT_STEREO_IMA4;
        else if (sample_format == 3)
            format = AL_FORMAT_STEREO_MSADPCM_SOFT;
    }
    else if (sfinfo.channels == 3)
    {
        if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
        {
            if (sample_format == 0)
                format = AL_FORMAT_BFORMAT2D_16;
            else if (sample_format == 1)
                format = AL_FORMAT_BFORMAT2D_FLOAT32;
        }
    }
    else if (sfinfo.channels == 4)
    {
        if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
        {
            if (sample_format == 0)
                format = AL_FORMAT_BFORMAT3D_16;
            else if (sample_format == 1)
                format = AL_FORMAT_BFORMAT3D_FLOAT32;
        }
    }
    if (!format)
    {
        sf_close(sndfile);
        throw"Unsupported channel count:" + path;
    }

    if (sfinfo.frames / splblockalign > (sf_count_t)(INT_MAX / byteblockalign))
    {
        sf_close(sndfile);
        throw "Too many samples in : " + path;
    }

    /* Decode the whole audio file to a buffer. */
    membuf = static_cast<short*>(malloc((size_t)(sfinfo.frames / splblockalign * byteblockalign)));

    if (sample_format == 0)
        num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
//    else if (sample_format == 1)
  //      num_frames = sf_readf_float(sndfile, (membuf), sfinfo.frames);
    else
    {
        sf_count_t count = sfinfo.frames / splblockalign * byteblockalign;
        num_frames = sf_read_raw(sndfile, membuf, count);
        if (num_frames > 0)
            num_frames = num_frames / byteblockalign * splblockalign;
    }
    if (num_frames < 1)
    {
        free(membuf);
        sf_close(sndfile);
        throw "Failed to read samples in : " + path;
    }
    num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);

    //printf("Loading: %s (%s, %dhz)\n", path.c_str(), sfinfo.samplerate);
    //fflush(stdout);

    /* Buffer the audio data into a new buffer object, then free the data and
     * close the file.
     */
    err = alGetError();
    buffer = 0;
    alGenBuffers(1, &buffer);
    err = alGetError();
    // (splblockalign > 1)
        //alBufferi(buffer, AL_UNPACK_BLOCK_ALIGNMENT_SOFT, splblockalign);
    alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);

    free(membuf);
    sf_close(sndfile);

    /* Check if an error occurred, and clean up if so. */
    err = alGetError();
    if (err != AL_NO_ERROR)
    {
        if (buffer && alIsBuffer(buffer))
            alDeleteBuffers(1, &buffer);
        std::string errorm = std::string(alGetString(err));
        throw "OpenAL Error: " + errorm;
        return;
    }


    alSourcei(source, AL_BUFFER, (ALint)buffer);
    err = alGetError();
    if (err != AL_NO_ERROR)
    {
        throw "alSourcei AL_BUFFER 0 : ";
    }

}

Sound::Sound()
{
    alGenSources(1, &source);
}

Sound::~Sound()
{
    /* All done. Delete resources. */
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
}

void Sound::setSourcePosition(mathT::vec3d p)
{
    ALfloat srcPos[3] = {p.x, p.y, p.z};
    alGetError(); // clear error state
    alSourcefv(source, AL_POSITION, srcPos);
    ALenum error = alGetError();
    if (error != AL_NO_ERROR)
        throw "alSourcefv 0 AL_POSITION : \n";
}

void Sound::setSourceVelocity(mathT::vec3d v)
{
    ALfloat srcVel[3] = { v.x, v.y, v.z };
    alGetError(); // clear error state
    alSourcefv(source, AL_VELOCITY, srcVel);
    ALenum error = alGetError();
    if (error != AL_NO_ERROR)
        throw "alSourcefv 0 AL_VELOCITY : \n";
}

void Sound::setSourceDirection(mathT::vec3d direction)
{
}

void Sound::setPitch(float pitch)
{
    this->sPitch = pitch;
    alGetError(); // clear error state
    alSourcef(source, AL_PITCH, this->sPitch);
    ALenum err;
    if ((err = alGetError()) != AL_NO_ERROR)
        throw"alSourcef 0 AL_PITCH : \n";
}

void Sound::setGain(float gain)
{
    this->sGain = gain;
    alGetError(); // clear error state
    alSourcef(source, AL_GAIN, gain);
    ALenum err;
    if ((err = alGetError()) != AL_NO_ERROR)
        throw"alSourcef 0 AL_GAIN : \n";
}

void Sound::playSound()
{

    alGetSourcei(source, AL_SOURCE_STATE, &state);
    if(source && state != AL_PLAYING)
        alSourcePlay(source);

}
