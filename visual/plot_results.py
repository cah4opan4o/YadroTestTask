import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('results.csv')

plt.figure(figsize=(10, 6))
for mod in df['Modulation'].unique():
    subset = df[df['Modulation'] == mod]
    plt.semilogy(subset['SNR (dB)'], subset['BER'], marker='o', label=mod)

plt.xlabel('SNR (dB)')
plt.ylabel('BER (log scale)')
plt.title('BER vs SNR for Different Modulations')
plt.grid(True, which='both', linestyle='--')
plt.legend()
plt.tight_layout()
plt.savefig('ber_vs_snr.png', dpi=300)
print('Plot saved as ber_vs_snr.png')