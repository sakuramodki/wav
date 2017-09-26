#include "Wav.hpp"
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

	/*
	std::string str;
	std::cout << "file type:" << getString(riff.riff, str) << std::endl;
	std::cout << "file size : " << riff.fileSize << std::endl;
	std::cout << "file format : " << getString(riff.type,str) << std::endl;

	std::cout << "format Id : " << wavHeader.formatId << std::endl;
	std::cout << "channels : " << wavHeader.channel << std::endl;
	std::cout << "samplingRatio : " << wavHeader.samplingRatio << std::endl;
	*/

    int blockSizePerChannel =  wavHeader.blockSize / wavHeader.channel;

    int values[wavHeader.channel];
    show.init();
	const double testFreq = 44100 / 1024 * 500;
    for (int i = 0 ; i < 44100 * 10 ; ++i) {
        for (int ch = 0 ; ch < wavHeader.channel; ++ch) {
            unsigned char tmp[blockSizePerChannel];
            for (int j = 0; j < blockSizePerChannel ; j++) {
                this->fin.read( (char *)&tmp[j], 1);
            }
            values[ch] = convertToInt((char *)tmp, blockSizePerChannel);
            //values[ch] = sin(i * testFreq * M_PI / 44100) * SHRT_MAX/2;
        }
        show.onLoad(values, wavHeader.channel, wavHeader);
        output.onLoad(values, wavHeader.channel, wavHeader);
    }
}

WavHeader& Wav::getWavHeader() {
    return wavHeader;
}
