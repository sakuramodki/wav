#pragma once
#include <boost/circular_buffer.hpp>
#include <boost/shared_ptr.hpp>
#include <fftw3.h>
#include <vector>
#include "structs.hpp"

class WavShow {
    typedef std::vector<double> DoubleVector;
    typedef boost::shared_ptr<DoubleVector> DoubleVectorPtr;

private:
    boost::circular_buffer<int> buf;
    boost::circular_buffer<DoubleVectorPtr> bufFft;

    int sampleCount;

    double *in;
    double *out;
    fftw_plan p;

    void showFft();
    void stashFft();
    void fft();
    double getRMS();

public:
    void fftinit();
    void init();
    void onLoad(signed int values[], int channels, WavHeader& wav);
};

