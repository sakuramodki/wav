#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "structs.hpp"
#include "Filter.hpp"
#include "WavBuffer.hpp"

class Wav {
private:
    typedef boost::shared_ptr<Filter> FilterPtr;
    std::vector<FilterPtr> filters;
    std::ifstream fin;

    RiffHeader riff;
    DataHeader header;
    WavHeader wavHeader;

    typedef boost::shared_ptr<WavOutput> WavOutputPtr;
    std::vector<WavOutputPtr> output;
    WavBuffer buf;

private:
    signed int convertToInt (char* val, int size);

public:
    void open(std::string fname);
    WavHeader& getWavHeader();
};



