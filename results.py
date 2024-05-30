import pandas as pd
import matplotlib.pyplot as plt

# Load the data
data = pd.read_csv('shuffle_times.csv')

# Plot the data
plt.figure(figsize=(10, 6))

for shuffle_type in data['type'].unique():
    subset = data[data['type'] == shuffle_type]
    plt.plot(subset['size'], subset['time'], label=shuffle_type)

plt.xlabel('Input Size')
plt.ylabel('Running Time (seconds)')
plt.title('Running times of LloydiusShuffle algorithm')
plt.legend()
plt.xscale('log')
plt.yscale('log')
plt.grid(True)

plt.savefig('custom_shuffle_results.png')
plt.show()
