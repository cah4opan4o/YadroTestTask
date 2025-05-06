#include "modulator.hpp"
#include <cmath>
#include <stdexcept>

QAMModulator::QAMModulator(ModType type) : modulationType(type) {
    switch (type) {
        case QPSK: bitsPerSymbol = 2; break;
        case QAM16: bitsPerSymbol = 4; break;
        case QAM64: bitsPerSymbol = 6; break;
        default: throw std::invalid_argument("Unsupported modulation type");
    }
}

std::vector<std::complex<double>> QAMModulator::modulate(const std::vector<int>& bits) {
    std::vector<std::complex<double>> symbols;
    for (size_t i = 0; i + bitsPerSymbol <= bits.size(); i += bitsPerSymbol) {
        std::vector<int> symbolBits(bits.begin() + i, bits.begin() + i + bitsPerSymbol);
        symbols.push_back(mapBitsToSymbol(symbolBits));
    }
    return symbols;
}

std::complex<double> QAMModulator::mapBitsToSymbol(const std::vector<int>& b) {
    switch (modulationType) {
        case QPSK: {
            int I = 2 * b[0] - 1;
            int Q = 2 * b[1] - 1;
            return std::complex<double>(I, Q) / std::sqrt(2);
        }
        case QAM16: {
            int I = (2 * b[0] + b[1]) * 2 - 3;
            int Q = (2 * b[2] + b[3]) * 2 - 3;
            return std::complex<double>(I, Q) / std::sqrt(10);
        }
        case QAM64: {
            int I = (4 * b[0] + 2 * b[1] + b[2]) * 2 - 7;
            int Q = (4 * b[3] + 2 * b[4] + b[5]) * 2 - 7;
            return std::complex<double>(I, Q) / std::sqrt(42);
        }
        default: return {0, 0};
    }
}
