import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

# Ask the user for the CSV file name (without extension)
csv_file_name = input("Enter the name of the CSV file (without .csv extension): ")

# Construct the full file path with the .csv extension
csv_file_path = f"{csv_file_name}.csv"

def square_wave(t, frequency, amplitude, phase, offset):
    return offset + amplitude * np.sign(np.sin(2 * np.pi * frequency * t + phase))

try:
    # Load the CSV file
    df = pd.read_csv(csv_file_path)

    # Negate the values in the "Zeroed Force Z (N)" column
    df["Zeroed Force Z (N)"] = -df["Zeroed Force Z (N)"]

    # Convert the "Time (s)" column to represent time from the start
    df["Time (s)"] = df["Time (s)"] - df["Time (s)"].iloc[0]

    # Find the time when the negated zeroed force first crosses above 0.1 N
    threshold = 0.1
    crossing_time = None
    for index, row in df.iterrows():
        if row["Zeroed Force Z (N)"] > threshold:
            crossing_time = row["Time (s)"]
            break

    if crossing_time is not None:
        print(f"Time when the force crosses above 0.1 N: {crossing_time} seconds")

        # Define the time intervals
        interval_duration = 10  # seconds
        low_duration = 2.5  # seconds
        num_intervals = 17

        # Initialize lists to store average values, duty cycles, and commanded forces
        average_values = []
        duty_cycles = []
        commanded_forces = []

        # Calculate the averages, duty cycles, and commanded forces
        for i in range(num_intervals):
            start_time = crossing_time + i * (interval_duration + low_duration)
            end_time = start_time + interval_duration
            interval_data = df[(df["Time (s)"] >= start_time) & (df["Time (s)"] < end_time)]
            average_value = interval_data["Zeroed Force Z (N)"].mean()
            # Assign duty cycle based on interval number
            duty_cycle = 20 + i * 5  # Start from 20% and increase by 5% each interval
            commanded_force = duty_cycle / 10  # Calculate commanded force (N)
            average_values.append(average_value)
            duty_cycles.append(duty_cycle)
            commanded_forces.append(commanded_force)
            print(f"Duty Cycle: {duty_cycle}% - Commanded Force: {commanded_force:.2f} N - Average Value: {average_value:.2f} N")

        # Plot the data
        plt.figure(figsize=(15, 6))
        
        # Plot the average values vs. duty cycle
        plt.subplot(1, 3, 1)
        plt.plot(duty_cycles, average_values, marker='o')
        plt.xlabel("Duty Cycle (%)")
        plt.ylabel("Measured Force Z (N)")
        plt.title("Measured Force Z vs. Duty Cycle")
        plt.grid(True)
        
        # Plot the average values vs. commanded force
        plt.subplot(1, 3, 2)
        plt.plot(commanded_forces, average_values, marker='o')
        plt.xlabel("Commanded Force (N)")
        plt.ylabel("Measured Force Z (N)")
        plt.title("Measured Force Z vs. Commanded Force")
        plt.grid(True)
        
        # Plot the original data
        plt.subplot(1, 3, 3)
        plt.plot(df["Time (s)"], df["Zeroed Force Z (N)"])
        plt.xlabel("Time (s)")
        plt.ylabel("Force Z (N)")
        plt.title("Force Z (N) vs. Time")
        plt.grid(True)
        
        plt.tight_layout()
        plt.show()
    else:
        print("No crossing above 0.1 N found")

except FileNotFoundError:
    print(f"CSV file '{csv_file_path}' not found.")
except Exception as e:
    print(f"An error occurred: {e}")
