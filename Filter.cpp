#include "Filter.hpp"
#include <cmath>

Filter::Filter() {
    // setup Filter Parameter
    this->samplingRatio = 44100;
    this->filterTaps = 257;
    this->cutoff = 250;
    this->filterQ = 2.0;

    // create filter steps
    this->genFilterParams();
}

// initialize filter
void Filter::setSamplingRatio(int samplingRatio) {
    this->samplingRatio = samplingRatio;

    // create filter steps
    this->genFilterParams();
}

// set Cutoff Frequency of this filter;
void Filter::setCutoff(double cutoff) {
    this->cutoff = cutoff;

    // create filter steps
    this->genFilterParams();
}

void conbolution(double *a, int filterTaps) {
    double b[filterTaps];

    for(int i = 0; i < filterTaps; i++) {
        double sum = 0.0;
        for(int j = 0; j < filterTaps; j++) {
            sum += a[i] * a[j];
        }
        b[i] = sum;
    }

    for(int i = 0; i < filterTaps; i++) {
        a[i] = b[i];
    }
}

// set Cutoff Frequency of this filter;
void Filter::genFilterParams() {
    if(!this->a) {
        delete this->a;
    }
    this->a = new double[this->filterTaps];

    int herfTap = (this->filterTaps - 1) / 2 + 1;
    double omega = this->cutoff * (M_PI) / (this->samplingRatio/2);
    double omegac = 1.0 - (this->cutoff / (this->samplingRatio/2));
    double tapArray[herfTap];

    tapArray[0] = 1-omegac;
    for (int i = 1; i < herfTap; i++) {
        double coeff = 1/(M_PI*i) * sin(i*omega);
        double x = i / (herfTap-1);
        double window = 0.54 + 0.46*cos(i*M_PI/(herfTap-1));
        // double window = 0.42 - 0.5*cos(M_PI*x) + 0.08 * cos(2.0*M_PI*x);
        tapArray[i] = coeff * window;
    }

    for (int i = 0; i < herfTap; i++) {
        this->a[i] = tapArray[herfTap - 1 - i];
        this->a[herfTap + i] = tapArray[1+i];
    }
}

double Filter::getFiltered(WavBuffer& buf, int ch) {
    WavBuffer::BufferPtr in  = buf.buf[ch];
    double output = 0.0;

    for (int  i = 0; i < this->filterTaps; i++) {
        output += (*in)[i] * this->a[i];
    }

    return output;
}


