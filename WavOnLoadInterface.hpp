#include "structs.hpp"

class WavOnLoadInterface {
public:
    virtual void onLoad(signed int values[], int channels, WavHeader& wav) = 0;
};
