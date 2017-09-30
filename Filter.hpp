#pragma once
#include <boost/circular_buffer.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>
#include "structs.hpp"
#include "WavOutput.hpp"
#include "WavBuffer.hpp"

class Filter {
private:
    // filter paramters
    double cutoff;
    int samplingRatio;
    int filterQ;
    int filterTaps;

    double *a;

public:
    Filter();
    void setSamplingRatio(int samplingRatio);
    void setCutoff(double cutoff);
    void genFilterParams();
    double getFiltered(WavBuffer& buf, int ch);
};

