#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "WavShow.hpp"
#include "structs.hpp"

class Wav {
private:
    WavShow show;
	WavOutput output;
    std::ifstream fin;

    RiffHeader riff;
    DataHeader header;
    WavHeader wavHeader;


private:
    signed int convertToInt (char* val, int size);

public:
    void open(std::string fname);
    WavHeader& getWavHeader();
};



