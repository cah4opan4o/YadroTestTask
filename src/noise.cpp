#include "noise.hpp"
#include <random>
#include <cmath>

AWGNChannel::AWGNChannel(double snr_dB) : snr_dB(snr_dB) {}

std::vector<std::complex<double>> AWGNChannel::transmit(const std::vector<std::complex<double>>& signal) {
    double snr_linear = std::pow(10.0, snr_dB / 10.0);
    double signal_power = 0.0;
    for (const auto& s : signal) {
        signal_power += std::norm(s);
    }
    signal_power /= signal.size();

    double noise_power = signal_power / snr_linear;
    double sigma = std::sqrt(noise_power / 2.0);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> dist(0.0, sigma);

    std::vector<std::complex<double>> noisy(signal.size());
    for (size_t i = 0; i < signal.size(); ++i) {
        double noise_real = dist(gen);
        double noise_imag = dist(gen);
        noisy[i] = signal[i] + std::complex<double>(noise_real, noise_imag);
    }

    return noisy;
}
