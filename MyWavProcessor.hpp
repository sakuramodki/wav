#pragma once
#include "WavOnLoadInterface.hpp"
#include "structs.hpp" // For WavHeader
#include <iostream>    // For std::cout

class MyWavProcessor : public WavOnLoadInterface {
public:
    void onLoad(signed int values[], int channels, WavHeader& wav) override;
};
