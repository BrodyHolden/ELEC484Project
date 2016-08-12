% This generates a C header file containing many arrays representing the
% windowed Room Impulse Response.

clear all;
close all;

% Impulse response in floats with real and imaginary values interlaced.
[out, Fs, N] = RIR_windowed_split('48k_RIR.wav');
windows = separate_windows_into_array(out, N);
filename = generate_impulse_response_arrays_header_float_real_imag(windows);

fprintf('Generated a C header file named %s with impulse response arrays\n', filename);
