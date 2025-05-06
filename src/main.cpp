#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <chrono>
#include "modulator.hpp"
#include "demodulator.hpp"
#include "noise.hpp"
#include "QAMTypes.hpp"
#include <random>

std::vector<int> generateRandomBits(int numBits) {
    std::vector<int> bits(numBits);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 1);

    #pragma omp parallel for
    for (int i = 0; i < numBits; ++i) {
        bits[i] = dist(gen);
    }
    return bits;
}

double calculateBER(const std::vector<int>& original, const std::vector<int>& decoded) {
    if (original.empty() || decoded.empty()) return 1.0;
    
    size_t errors = 0;
    size_t len = std::min(original.size(), decoded.size());
    
    #pragma omp parallel for reduction(+:errors)
    for (size_t i = 0; i < len; ++i) {
        if (original[i] != decoded[i]) errors++;
    }
    return static_cast<double>(errors) / len;
}

int main() {
    const std::vector<ModType> modulationTypes = {QPSK, QAM16, QAM64};
    const std::vector<double> snrValues = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 
                                          10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20.0};
    const int numBits = 100000;
    const int numTrials = 50;

    auto start_time = std::chrono::high_resolution_clock::now();

    std::ofstream file("results.csv");
    file << "Modulation,SNR (dB),BER\n";

    for (const auto& type : modulationTypes) {
        std::cout << "Processing " << (type == QPSK ? "QPSK" : (type == QAM16 ? "16-QAM" : "64-QAM")) 
                  << " modulation..." << std::endl;

        for (const auto& snr_dB : snrValues) {
            double totalBER = 0.0;

            #pragma omp parallel for reduction(+:totalBER)
            for (int trial = 0; trial < numTrials; ++trial) {
                auto bits = generateRandomBits(numBits);

                QAMModulator mod(type);
                auto symbols = mod.modulate(bits);

                AWGNChannel channel(snr_dB);
                auto noisySymbols = channel.transmit(symbols);

                QAMDemodulator demod(type);
                auto decodedBits = demod.demodulate(noisySymbols);

                totalBER += calculateBER(bits, decodedBits);
            }

            double avgBER = totalBER / numTrials;
            std::string modName = (type == QPSK) ? "QPSK" : (type == QAM16) ? "16-QAM" : "64-QAM";

            file << modName << "," << snr_dB << "," << avgBER << "\n";
            file.flush();

            std::cout << "SNR: " << snr_dB << " dB, BER: " << avgBER << std::endl;
        }
    }

    file.close();

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::minutes>(end_time - start_time);
    std::cout << "Simulation completed in " << duration.count() << " minutes.\n";

    return 0;
}