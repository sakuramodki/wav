#include "Wav.hpp"
#include <sstream>
#include <cmath>

signed int Wav::convertToInt (char* val, int size) {
    signed int result;

    // 8bitの場合は0-255なので正規化する
    if (size == 1) {
        result = *(unsigned char*)val - 128;
        return result;
    }

    // 16bitなら16bit/signedとして扱う
    if (size == 2) {
        result = *(signed short int*)val;
        return result;
    }

    // 32bitなら32bit/floatとして扱う
    if (size == 4) {
        result = *(float*)val;
        return result;
    }

    std::cout<< "erro : undefined file format" << std::endl;
    exit(1);
    return result;
}

template<typename T>
std::string& getString(T& val, std::string& str) {
    char tmp[sizeof(T) + 1] = {};
    memcpy(tmp, val, sizeof(T));
    str = tmp;
    return str;
}

void Wav::open(std::string fname) {
    this->fin.open(fname, std::ios::in | std::ios::binary);

    this->fin.read( (char *)&riff, sizeof( RiffHeader ) );
    this->fin.read( (char *)&header, sizeof( DataHeader ) );
    this->fin.read( (char *)&wavHeader, header.size );
    this->fin.read( (char *)&header, sizeof( DataHeader ) );

    /**
    // output debug infomations
    std::string str;
    std::cout << "file type:" << getString(riff.riff, str) << std::endl;
    std::cout << "file size : " << riff.fileSize << std::endl;
    std::cout << "file format : " << getString(riff.type,str) << std::endl;

    std::cout << "format Id : " << wavHeader.formatId << std::endl;
    std::cout << "channels : " << wavHeader.channel << std::endl;
    std::cout << "samplingRatio : " << wavHeader.samplingRatio << std::endl;
    **/


    int values[wavHeader.channel];

    int splitSize = 16;
    int effected[splitSize][wavHeader.channel];

    // init multiband filter
    const int maxFreq = 20000;
    const int minFreq = 40;
    for(int band = 0; band < splitSize; ++band ) {
        FilterPtr filter = FilterPtr(new Filter);

        filter->setSamplingRatio(wavHeader.samplingRatio);
        double cutoff =  minFreq + (maxFreq - minFreq) * pow(band,2) / pow(splitSize,2);
        std::cout << "bakd " << band << " = "<< cutoff << " Hz" << std::endl;
        filter->setCutoff(cutoff);

        this->filters.push_back(filter);
    }

    // init output
    for(int band = 0; band < splitSize; ++band ) {
        this->output.push_back(WavOutputPtr(new WavOutput));
    }

    this->buf.init(wavHeader.channel, 1024);
    int blockSizePerChannel =  wavHeader.blockSize / wavHeader.channel;
    int cnt = 0;
    double totalTime = 0.0;
    for (int i = 0 ; i < 44100 * 10 ; ++i) {

        // load data
        for (int ch = 0 ; ch < wavHeader.channel; ++ch) {
            unsigned char tmp[blockSizePerChannel];
            for (int j = 0; j < blockSizePerChannel ; j++) {
                this->fin.read( (char *)&tmp[j], 1);
            }
            values[ch] = convertToInt((char *)tmp, blockSizePerChannel);
        }

        // main calcs.
        clock_t start = clock();
        this->buf.pushData(values, wavHeader.channel);
        for(int band = 0; band < splitSize; ++band ) {
            for (int ch = 0 ; ch < wavHeader.channel; ++ch) {
                effected[band][ch] = this->filters[band]->getFiltered(this->buf, ch);
            }
        }
        for (int ch = 0 ; ch < wavHeader.channel; ++ch) {
            double sum = 0.0;
            for(int band = 0; band < splitSize; ++band ) {
                    effected[band][ch] -= sum;
                    sum += effected[band][ch];
            }
        }
        totalTime += clock() - start;
        cnt++;

        for(int band = 0; band < splitSize; ++band ) {
            this->output[band]->pushData(effected[band], wavHeader.channel);
        }
    }

    totalTime /= CLOCKS_PER_SEC;
    std::cout << "sum " << totalTime << std::endl;
    std::cout << "ave " << totalTime / cnt<< std::endl;
    std::cout << "aveSample " << totalTime / cnt * wavHeader.samplingRatio << std::endl;

    for(int band = 0; band < splitSize; ++band ) {
        std::ostringstream fname;
        fname << "output-band_" << band << ".wav";
        this->output[band]->save(fname.str(), wavHeader);
    }
}

WavHeader& Wav::getWavHeader() {
    return wavHeader;
}
