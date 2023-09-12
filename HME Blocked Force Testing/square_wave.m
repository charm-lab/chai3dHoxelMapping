function y = square_wave(t, frequency, amplitude, phase, offset)
    y = offset + amplitude * sign(sin(2 * pi * frequency * t + phase));
end