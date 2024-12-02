# %%
import matplotlib.pyplot as plt

# %%
# Data
transmission_speed = [1000, 800, 500, 300]
loss_rate_c10 = [0.88, 0.93, 1.7, 2.82]
loss_rate_c100 = [0, 0.007, 0.014, 0.069]
loss_rate_c1000 = [0, 0, 0, 0]

# Plot
plt.figure(figsize=(10, 6))

plt.plot(transmission_speed, loss_rate_c10, marker='o', markersize=5, label='consumer=10')
plt.plot(transmission_speed, loss_rate_c100, marker='o', markersize=5, label='consumer=100')
plt.plot(transmission_speed, loss_rate_c1000, marker='o', markersize=5, label='consumer=1000')


# plt.gca().invert_xaxis()
plt.ylim(-0.5, 5)

plt.title('Loss Rate vs. Transmission Speed\n(data=1000, buffer=3)', fontsize=14)
plt.xlabel('Transmission Speed (ms)', fontsize=12)
plt.ylabel('Loss Rate (%)', fontsize=12)
plt.grid(True, linestyle='--', alpha=0.6)
plt.legend()

plt.show()

# %%
# Data
buffer_size = [10,9,8,7,6,5,4,3,2,1]
loss_rate_c10 = [1.18, 1.13, 1.43, 1.89, 1.47, 0.97, 1.74, 1.38, 1.93, 4.15]
loss_rate_c100 = [0.012, 0.055, 0.067, 0.074, 0.025, 0.004, 0.05, 0.001, 0.001, 0.729]
loss_rate_c1000 = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0559]

# Plot
plt.figure(figsize=(10, 6))

plt.plot(buffer_size, loss_rate_c10, marker='o', markersize=5, label='consumer=10')
plt.plot(buffer_size, loss_rate_c100, marker='o', markersize=5, label='consumer=100')
plt.plot(buffer_size, loss_rate_c1000, marker='o', markersize=5, label='consumer=1000')

plt.ylim(-0.5, 5)

# plt.gca().invert_xaxis()
plt.xticks(range(1, 11))

plt.title('Loss Rate vs. Buffer Size\n(data=1000, transmission speed=500ms)', fontsize=14)
plt.xlabel('Buffer Size', fontsize=12)
plt.ylabel('Loss Rate (%)', fontsize=12)
plt.grid(True, linestyle='--', alpha=0.6)
plt.legend()

plt.show()

# %%



