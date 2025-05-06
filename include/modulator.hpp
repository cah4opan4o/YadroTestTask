#pragma once
#include <vector>
#include <complex>
#include "QAMTypes.hpp"

class QAMModulator {
public:
    QAMModulator(ModType type);
    std::vector<std::complex<double>> modulate(const std::vector<int>& bits);
private:
    ModType modulationType;
    int bitsPerSymbol;

    std::complex<double> mapBitsToSymbol(const std::vector<int>& bits);
};
