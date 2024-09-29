import subprocess
import numpy as np
import matplotlib.pyplot as plt

RESOLUTION = 20
# Define the ranges for the input parameters
TEMPERATURE_RANGE = np.linspace(10, 40, RESOLUTION)
TEMPERATURE_CHANGE_RANGE = np.linspace(-5, 5, RESOLUTION)
TEC_POWER_RANGE = np.linspace(0, 25, RESOLUTION)
FAN_RANGE = np.linspace(0, 100, RESOLUTION)

# Create a figure with a 3D subplot
fig = plt.figure(figsize=(10, 8))
ax = fig.add_subplot(111, projection='3d')

# Define the fixed parameters for the plot
fixed_params = (0, 50)

# Calculate the total number of iterations
total_iterations = len(TEMPERATURE_RANGE) * len(TEC_POWER_RANGE)

# Initialize a counter for the current iteration
current_iteration = 0

# Loop over the parameters and plot the results
results = np.zeros((len(TEMPERATURE_RANGE), len(TEC_POWER_RANGE)))
for i, temperature in enumerate(TEMPERATURE_RANGE):
    for j, TEC_power in enumerate(TEC_POWER_RANGE):
        # Print the current progress
        current_iteration += 1
        progress = (current_iteration / total_iterations) * 100
        print(f"Progress: {progress:.2f}% ({current_iteration}/{total_iterations}) - Temperature: {temperature}, TEC Power: {TEC_power}", end='\r')

        # Call the program with the current parameter values and fixed values for the other parameters
        output = subprocess.check_output(['./out/TecFanControl.out', str(temperature), str(fixed_params[0]), str(TEC_power), str(fixed_params[1])])
        # Decode the output and split it into lines
        lines = output.decode().strip().split('\n')
        # Extract the fan speed value from the last line
        fan_speed = float(lines[-1].split(': ')[1].strip('%'))
        # Append the result to the array
        results[i, j] = fan_speed

# Print a final message
print("\nProgress: 100.00% - Completed")

# Plot the results
X, Y = np.meshgrid(TEMPERATURE_RANGE, TEC_POWER_RANGE)
ax.plot_surface(X, Y, results, cmap='viridis', edgecolor='none')

# Set the labels and title
ax.set_xlabel('Temperature')
ax.set_ylabel('TEC Power')
ax.set_zlabel('Fan Speed (%)')
ax.set_title('Fan Speed vs Temperature and TEC Power')

# Show the plot
plt.show()
