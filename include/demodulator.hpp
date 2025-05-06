#pragma once
#include <vector>
#include <complex>
#include "QAMTypes.hpp"

class QAMDemodulator {
public:
    QAMDemodulator(ModType type);
    std::vector<int> demodulate(const std::vector<std::complex<double>>& symbols);
private:
    ModType modulationType;
    int bitsPerSymbol;

    std::vector<int> mapSymbolToBits(const std::complex<double>& sym);
};
