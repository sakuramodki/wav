#include "MyWavProcessor.hpp"

void MyWavProcessor::onLoad(signed int values[], int channels, WavHeader& wav) {
    if (channels > 0) {
        // Print the first sample of the first channel
        std::cout << "MyWavProcessor::onLoad: First sample of first channel = " << values[0] << std::endl;
    }
}
