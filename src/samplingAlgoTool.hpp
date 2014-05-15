#ifndef SAMPLING_ALGO_TOOL_H
#define SAMPLING_ALGO_TOOL_H


unsigned short int computeLvl(const float& density, const float& spaceSize, const unsigned short int& subdivFactor);

unsigned short int computeLvlMinusHalfRank(const float& density, const float& spaceSize, const unsigned short int& subdivFactor);

unsigned short int computeLvlMinusOneRank(const float& density, const float& spaceSize, const unsigned short int& subdivFactor);

short int computeRank(const float& density, const float& spaceSize, const unsigned short int& lvl, const unsigned short int& subdivFactor);

float computeRankCont(const float& density, const float& spaceSize, const unsigned short int& lvl, const unsigned short int& subdivFactor);

float computeDensity(const unsigned short int& lvl, const unsigned short int& rank, const float& spaceSize, const unsigned short int& subdivFactor);


#endif
