#pragma once
#include <boost/circular_buffer.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>

class WavBuffer {
public:
    typedef boost::circular_buffer<int> Buffer;
    typedef boost::shared_ptr<Buffer> BufferPtr;
    std::vector<BufferPtr> buf;

    void init(int channels, int bufferSize);
    void pushData(signed int values[], int channels);
};
