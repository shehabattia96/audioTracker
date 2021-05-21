#include <iostream>
#include <fstream>
#include "RtAudio.h"
#include <cstdlib>
#include <cstring>
#include <vector>
#include <set>

#include <chrono>
#include <thread>
#include <future>
#include <mutex>

#define DEBUG true

// #define MAX_PICTUREQUEUE_SIZE 3
// std::queue<std::vector<char>> microphoneStreamQueue;
// std::mutex microphoneStreamMutex;

using std::chrono::operator""ms;

RtAudio adc;
std::set<size_t> microphoneDeviceIndecies; // holds all the available microphone

int record( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status, void *userData )
{
  if ( status )
    std::cout << "Stream overflow detected!" << std::endl;
  
    // Do something with the data in the "inputBuffer" buffer.
    int16_t* buffer = (int16_t*)inputBuffer;
    size_t microphoneIndex = (size_t)userData;
    std::cout << "microphoneIndex" << microphoneIndex << std::endl;
    std::cout << "streamTime" << streamTime << std::endl;

    for (int16_t i = 0; i < 256; i++)
    {
        int16_t sample = buffer[i];
        std::cout << sample << "|";
    }
    std::cout << std::endl;
    

  return 0;
}

void scanForMicrophones() {
    unsigned int allAudioDevices = adc.getDeviceCount();
    RtAudio::DeviceInfo info;
    for ( unsigned int audioDeviceIndex=0; audioDeviceIndex<allAudioDevices; audioDeviceIndex++ ) {
        info = adc.getDeviceInfo( audioDeviceIndex );
        if ( info.probed == true ) {
            if (info.inputChannels == 1)
                microphoneDeviceIndecies.insert(audioDeviceIndex);
            #if DEBUG
                std::cout << "device = " << audioDeviceIndex;
                std::cout << ": name = " << info.name << "\n";
                std::cout << ": maximum intput channels = " << info.inputChannels << "\n";
            #endif
        }
    }
}

std::vector<RtAudio> microphones;

void listenToMicrophone(size_t microphoneIndex) {
    std::cout << "Starting stream for microphoneIndex " << microphoneIndex  << std::endl;
    RtAudio microphone;
    RtAudio::StreamParameters parameters;
    parameters.deviceId = microphoneIndex;
    parameters.nChannels = 2;
    parameters.firstChannel = 0;
    unsigned int sampleRate = 48000;
    unsigned int bufferFrames = 256; // 256 sample frames

    microphones.push_back(microphone);

    try {
        microphone.openStream( NULL, &parameters, RTAUDIO_SINT16,
                        sampleRate, &bufferFrames, &record, &microphoneIndex );
        microphone.startStream();
    }
    catch ( RtAudioError& e ) {
        std::cout << "Failed to start stream for microphoneIndex " << microphoneIndex  << std::endl;
        e.printMessage();
    }

}

int main()
{
    while ( microphoneDeviceIndecies.size() == 0 ) {
        scanForMicrophones();
        if (microphoneDeviceIndecies.size() == 0) {
            std::cout << "No microphones found, waiting for a microphone to be connected." << std::endl;
            std::this_thread::sleep_for(2000ms);
        }
    }

    std::vector<std::future<void>*> promises;
    for (auto& microphoneIndex : microphoneDeviceIndecies) {
	    std::future<void> microphoneThreadPromise = std::async(
            std::launch::async, 
            &listenToMicrophone, 
            microphoneIndex);
        promises.push_back(&microphoneThreadPromise);
    }
   
    
    char input;
    std::cout << "\nRecording ... press <enter> to quit.\n";
    std::cin.get( input );
    for (auto& microphone : microphones) {
        try {
            // Stop the stream
            microphone.stopStream();
        }
        catch (RtAudioError& e) {
            e.printMessage();
        }
        if ( microphone.isStreamOpen() ) microphone.closeStream();
    }

    // resolve the promises
    for (auto& promise : promises) {
        promise->get();
    }

    return 0;
}