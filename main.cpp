#include <iostream>
//#include <sndfile.h>
#include <portaudio.h>

#define SAMPLE_RATE (44100)
#define FRAMES_PER_BUFFER (256)
#define NUM_OUTPUT_CHANNEL (1)

#ifndef M_PI
#define M_PI (3.14159265)
#endif




typedef struct {
    float phase;
} paTestData;


static int patestCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
    paTestData *data = (paTestData *)userData;
    
    float *out = (float*) outputBuffer;

    for (unsigned long i = 0; i < framesPerBuffer; ++i)
    {
        out[i] = sin(data->phase * 2 * M_PI) * 0.1;
        if (data->phase >= 1.0) data->phase -= 1.0;
        data->phase += (1. / ((double) SAMPLE_RATE / 1000));
    }

    return paContinue;
}

int main()
{
    PaError err;

    /* Initialize PortAudio */   
    err = Pa_Initialize();
    if (err != paNoError) goto error;

    /* Get default output device */
    PaStreamParameters outputParameters; 
    outputParameters.device = Pa_GetDefaultOutputDevice();
    if (outputParameters.device == paNoDevice) {
        std::cout << "Error: No default output device.\n" << std::endl;
        goto error;
    }
    outputParameters.channelCount = NUM_OUTPUT_CHANNEL;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    paTestData data = { 0 };
    PaStream *stream;
    /* Open an audio I/O stream. */
    err = Pa_OpenStream(&stream,
                        NULL, /* no input */
                        &outputParameters,
                        SAMPLE_RATE,
                        FRAMES_PER_BUFFER,
                        paClipOff, /* we won't output out of range samples so don't bother clipping them */
                        patestCallback,
                        &data); /* custom data*/
    if (err != paNoError) goto error;

    err = Pa_StartStream(stream);
    if (err != paNoError) goto error;

    Pa_Sleep(1000);

    err = Pa_StopStream(stream);
    if (err != paNoError) goto error;

    err = Pa_CloseStream(stream);
    if (err != paNoError) goto error;

    err = Pa_Terminate();
    if (err != paNoError) goto error;
    
    return 0;

error:
    std::cout << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
    return 1;
}
