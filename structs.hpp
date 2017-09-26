#pragma once

typedef struct {
    char         riff[4];
    unsigned int fileSize;
    char         type[4];
} RiffHeader;

typedef struct {
    char         typeName[4];
    unsigned int size;
} DataHeader;

typedef struct {
    unsigned short formatId;
    unsigned short channel;
    unsigned int   samplingRatio;
    unsigned int   dataSpeed;
    unsigned short blockSize;
    unsigned short bitsPerBlock;
} WavHeader;
