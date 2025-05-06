#include "demodulator.hpp"
#include <cmath>
#include <limits>

QAMDemodulator::QAMDemodulator(ModType type) : modulationType(type) {
    switch (type) {
        case QPSK: bitsPerSymbol = 2; break;
        case QAM16: bitsPerSymbol = 4; break;
        case QAM64: bitsPerSymbol = 6; break;
        default: throw std::invalid_argument("Unsupported modulation type");
    }
}

std::vector<int> QAMDemodulator::demodulate(const std::vector<std::complex<double>>& symbols) {
    std::vector<int> bits;
    for (const auto& s : symbols) {
        auto b = mapSymbolToBits(s);
        bits.insert(bits.end(), b.begin(), b.end());
    }
    return bits;
}

std::vector<int> QAMDemodulator::mapSymbolToBits(const std::complex<double>& sym) {
    std::vector<std::complex<double>> constellation;
    std::vector<std::vector<int>> bitPatterns;

    switch (modulationType) {
        case QPSK: {
            double s = 1.0 / std::sqrt(2);
            constellation = {{s, s}, {s, -s}, {-s, s}, {-s, -s}};
            bitPatterns = {{1,1}, {1,0}, {0,1}, {0,0}};
            break;
        }
        case QAM16: {
            double s = 1.0 / std::sqrt(10);
            for (int i = -3; i <= 3; i += 2) {
                for (int q = -3; q <= 3; q += 2) {
                    constellation.emplace_back(i * s, q * s);
                    int ii = (i + 3) / 2, qq = (q + 3) / 2;
                    bitPatterns.push_back({(ii >> 1) & 1, ii & 1, (qq >> 1) & 1, qq & 1});
                }
            }
            break;
        }
        case QAM64: {
            double s = 1.0 / std::sqrt(42);
            for (int i = -7; i <= 7; i += 2) {
                for (int q = -7; q <= 7; q += 2) {
                    constellation.emplace_back(i * s, q * s);
                    int ii = (i + 7) / 2, qq = (q + 7) / 2;
                    bitPatterns.push_back({
                        (ii >> 2) & 1, (ii >> 1) & 1, ii & 1,
                        (qq >> 2) & 1, (qq >> 1) & 1, qq & 1
                    });
                }
            }
            break;
        }
    }

    double minDist = std::numeric_limits<double>::max();
    int bestIndex = 0;

    for (size_t i = 0; i < constellation.size(); ++i) {
        double dist = std::norm(sym - constellation[i]);
        if (dist < minDist) {
            minDist = dist;
            bestIndex = i;
        }
    }

    return bitPatterns[bestIndex];
}
