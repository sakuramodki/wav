#pragma once
#include <vector>
#include "structs.hpp"

class WavOutput {
private:
    RiffHeader riff;
    WavHeader wavHeader;
    std::vector<signed int*> datas;

public:
    void setWavHeader(WavHeader& wav);
    void pushData(signed int values[], int channels);
    void save(std::string fname, WavHeader& wav);
};
