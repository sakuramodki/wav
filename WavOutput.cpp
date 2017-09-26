#include <iostream>
#include <fstream>
#include <string>
#include "WavOutput.hpp"

void WavOutput::setWavHeader(WavHeader& wav) {
    memcpy(&this->wavHeader, &wav, sizeof(WavHeader) );
}

void WavOutput::pushData(signed int values[], int channels, WavHeader& wav) {
    int* data = new int[channels];
    for (int i = 0; i < channels; i++) {
        data[i] = values[i];
    }
    this->datas.push_back(data);
}

void WavOutput::save(std::string fname) {
    // calcurate file size
    int dataSize = sizeof(signed short int) * this->wavHeader.channel * this->datas.size();
    int totalSize = sizeof(RiffHeader) + sizeof(DataHeader) + sizeof(WavHeader) + sizeof(DataHeader) + dataSize;

    // setup output
    std::ofstream fout;
    fout.open(fname, std::ios::out | std::ios::binary);

    // setup Riff header
    RiffHeader riff;
    memcpy(&riff.riff, "RIFF", sizeof(riff.riff));
    memcpy(&riff.type, "WAVE", sizeof(riff.type));
    riff.fileSize = totalSize - 8;

    fout.write( (char *)&riff, sizeof( RiffHeader ) );

    // setup Data Header
    DataHeader dataHeader;
    memcpy(dataHeader.typeName, "fmt ", sizeof(dataHeader.typeName));
    dataHeader.size = sizeof(WavHeader);
    fout.write( (char *)&dataHeader, sizeof( DataHeader ) );

    // setup WAV Header
    wavHeader.blockSize = wavHeader.channel * sizeof(signed short int);
    fout.write( (char *)&wavHeader, dataHeader.size );

    // setup Data Header
    memcpy(dataHeader.typeName, "data", sizeof(dataHeader.typeName));
    dataHeader.size = dataSize;
    fout.write( (char *)&dataHeader, sizeof( DataHeader ) );

    // write Datas
    for (int i = 0 ; i < this->datas.size(); ++i) {
        for (int ch = 0 ; ch < wavHeader.channel; ++ch) {
            signed short int data = this->datas[i][ch];
            fout.write((char*)&data, sizeof(data));
        }
    }
}
