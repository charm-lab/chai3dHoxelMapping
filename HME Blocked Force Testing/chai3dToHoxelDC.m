% CHAI3D to Hoxel Duty Cycle Values
% Author: Jasmin E. Palmer
clear; close all; clc;
min_force = 0.1;  % min
max_pump_speed = 100;

commandedForce = [min_force:min_force:25]';

% get_pump_speed
scaledForce = 10.*commandedForce;

% Initialize pump_Speed array
pump_Speed = zeros(1, length(scaledForce));

for i = 1:length(scaledForce)
    if (scaledForce(i) >= max_pump_speed)
        pump_Speed(i) = max_pump_speed;
    else
        pump_Speed(i) = scaledForce(i);
    end
end

% duty2bits(duty) -- convert pump speed to DC
duty = pump_Speed' % <--- This Variable
duty2bits = round(duty * 65535 / 100); 

% Plot Model
figure;
plot(commandedForce, duty, 'b.'); hold on;
xlabel("Commanded CHAI3D Force [N]");
ylabel("Duty Cycle");
title("Force vs Duty Cycle");

% Plot Force Limit Point:
forceLimit = 20; % N
plot(forceLimit, duty(find(commandedForce == forceLimit)),...
    'ro', 'LineWidth', 2); 

legend("Model", "Force Limit", "Location","southeast");

ylim([0 110]); yticklabels([0:10:100])

hold off;
