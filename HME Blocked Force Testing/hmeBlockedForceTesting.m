clear; close all; clc;

% try
% Load the CSV file
df = readtable('data_20230911134955.csv');

% Negate the values in the "Zeroed Force Z (N)" column
df.ZeroedForceZ_N_ = -df.ZeroedForceZ_N_;

% Convert the "Time (s)" column to represent time from the start
df.Time_s_ = df.Time_s_ - df.Time_s_(1);

% Find the time when the negated zeroed force first crosses above 0.1 N
threshold = 0.1;
crossing_time = NaN;
for index = 1:height(df)
    if df.ZeroedForceZ_N_(index) > threshold
        crossing_time = df.Time_s_(index);
        break;
    end
end

if ~isnan(crossing_time)
    fprintf('Time when the force crosses above 0.1 N: %.2f seconds\n', crossing_time);

    % Define the time intervals
    interval_duration = 10;  % seconds
    low_duration = 2.5;  % seconds
    num_intervals = 17;

    % Initialize arrays to store average values, duty cycles, and commanded forces
    average_values = zeros(1, num_intervals);
    duty_cycles = zeros(1, num_intervals);
    commanded_forces = zeros(1, num_intervals);

    % Calculate the averages, duty cycles, and commanded forces
    for i = 1:num_intervals
        start_time = crossing_time + (i - 1) * (interval_duration + low_duration);
        end_time = start_time + interval_duration;
        interval_data = df(df.Time_s_ >= start_time & df.Time_s_ < end_time, :);
        average_value = mean(interval_data.ZeroedForceZ_N_);
        % Assign duty cycle based on interval number
        duty_cycle = 20 + (i - 1) * 5;  % Start from 20% and increase by 5% each interval
        commanded_force = duty_cycle / 10;  % Calculate commanded force (N)
        average_values(i) = average_value;
        duty_cycles(i) = duty_cycle;
        commanded_forces(i) = commanded_force;
        %fprintf('Duty Cycle: %d%% - Commanded Force: %.2f N - Average Value: %.2f N\n', duty_cycle, commanded_force, average_value);
    end

    % Plot the data
    figure;

    % Plot the average values vs. duty cycle
    % subplot(1, 3, 1);
    subplot(2, 2, 1);
    plot(duty_cycles, average_values, 'bo-');
    xlabel('Duty Cycle (%)');
    ylabel('Measured Force Z (N)');
    title('Measured Force Z vs. Duty Cycle');
    grid on;
    ylim([0 25]);

    % Plot the average values vs. commanded force
    % subplot(1, 3, 2);
    subplot(2, 2, 2);
    plot(commanded_forces, average_values, 'bo-');
    xlabel('Commanded Force (N)');
    ylabel('Measured Force Z (N)');
    title('Measured Force Z vs. Commanded Force');
    grid on;
    ylim([0 25]);

    % Plot the original data
    % subplot(1, 3, 3);
    subplot(2, 2, [3 4]);
    plot(df.Time_s_, df.ZeroedForceZ_N_, 'b');
    xlabel('Time (s)');
    ylabel('Force Z (N)');
    title('Force Z (N) vs. Time');
    grid on;
    ylim([-1 25]);

    sgtitle('Force Analysis', 'fontsize', 18);
    % set(gcf, 'Position', [100, 100, 1200, 400]);

    improvePlot_v2(false, true, 14, 1400, 800);

else
    disp('No crossing above 0.1 N found');
end

% catch ME
%     disp(['An error occurred: ', ME.message]);
% end
