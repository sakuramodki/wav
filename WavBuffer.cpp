#include "WavBuffer.hpp"
#include <iostream>

void WavBuffer::init(int channels, int bufferSize) {
    this->buf.clear();

    for(int i = 0; i < channels; ++i) {
        this->buf.push_back(BufferPtr(new Buffer));
        this->buf[i]->set_capacity(bufferSize);
    }
}

void WavBuffer::pushData(signed int values[], int channels) {
    for(int ch = 0; ch < channels; ch++) {
        this->buf[ch]->push_front(values[ch]);
    }
}
