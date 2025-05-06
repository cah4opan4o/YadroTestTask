#pragma once
#include <vector>
#include <complex>

class AWGNChannel {
public:
    AWGNChannel(double snr_dB);
    std::vector<std::complex<double>> transmit(const std::vector<std::complex<double>>& signal);
private:
    double snr_dB;
};
