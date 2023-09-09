% CHAI3D to Hoxel Duty Cycle Values
clear; clc;
min_force = 0.1;  % min
max_pump_speed = 100;


commandedForce = min_force:1:50;

% get_pump_speed
scaledForce = 10 * commandedForce;
if (scaledForce >= max_pump_speed)
    pump_Speed = max_pump_speed;
else
    pump_Speed = scaledForce;
end


% duty2bits(duty) -- convert pump speed to DC

duty = pump_Speed;
duty2bits = round(duty * 65535 / 100)'

