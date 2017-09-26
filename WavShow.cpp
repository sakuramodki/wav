#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <fftw3.h>
#include "WavShow.hpp"


const int bufferSize = 1024*4;

void WavShow::fftinit() {
    in = new double[bufferSize];
    out = new double[bufferSize];
    p = fftw_plan_r2r_1d(bufferSize, in, out, FFTW_REDFT00, FFTW_ESTIMATE);
}

void WavShow::init() {
    fftinit();
    buf.set_capacity(bufferSize);
	for(int i = 0; i < bufferSize; i++) {
		buf.push_back(0.0);
	}
    //std::cout << std::setfill(' ') << std::setw(10) <<std::fixed << std::setprecision(2);
}

void WavShow::onLoad(signed int values[], int channels, WavHeader& wav) {
    buf.push_back(values[0]);

    sampleCount++;
    if (sampleCount % (bufferSize/4) == 0) {
        //std::cout << (1.0*sampleCount / wav.samplingRatio) << "\t" << getRMS() << std::endl;
        //std::cout << (1.0*sampleCount / wav.samplingRatio) << "\t";
        //std::cout << "|" << getRMS() <<"|" <<"\t";
        fft();
        showFft();
        stashFft();
        std::cout << std::endl;
    }
}

double WavShow::getRMS() {
    double sum = 0.0;
    for (int i = 0; i < bufferSize; i++) {
        double val = buf[i];
        sum += val * val;
    }
    return sqrt(sum/bufferSize) / SHRT_MAX;
}

void WavShow::fft() {
    for (int i = 0; i < bufferSize; i++) {
        // 窓関数をかけるならココで畳み込む
        in[i] = buf[i];
    }
    fftw_execute(p);
}

void WavShow::showFft() {
    const int devision = bufferSize/1;
    double m[devision] = {};

    for (int i = 0; i < bufferSize; i++) {
        int index = i / (bufferSize/devision);
        m[index] += std::abs(out[i]);
    }

    for (int i = 0; i < devision; i++) {
        std::cout << m[i] / (bufferSize/devision) / SHRT_MAX / bufferSize << "\t";
        // std::cout << std::abs(out[i] / SHRT_MAX) << "\t" ;
    }
}

void WavShow::stashFft() {
    DoubleVectorPtr backup = DoubleVectorPtr(new DoubleVector() );
    for( int i = 0; i < bufferSize; i++) {
        backup->push_back(out[i]);
    }
    bufFft.push_back(backup);
}
