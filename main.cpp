#include <iostream>
#include <portaudio.h>

#define SAMPLE_RATE (44100)

/* This routine will be called by the PortAudio engine when audio is needed.
** It may called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
static int patestCallback( const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData )
{
    return paContinue;
}

int main()
{
    std::cout << "--- portaudio test ---" << std::endl;
    
    auto err = Pa_Initialize();
    if (err != paNoError) goto error;

    PaStream *stream;
    uint16_t frames_per_buffer = 256;
    uint16_t num_output_chan = 2;
    /* Open an audio I/O stream. */
    err = Pa_OpenDefaultStream( &stream,
                                0, /* no input channels */
                                num_output_chan,
                                paFloat32,  
                                SAMPLE_RATE,
                                frames_per_buffer,  
                                patestCallback, 
                                NULL ); /* custom data*/
    if (err != paNoError) goto error;


    err = Pa_Terminate();
    if (err != paNoError) goto error;
    
error:
    std::cout << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;

    return 0;
}