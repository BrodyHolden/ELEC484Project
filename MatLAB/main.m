% This generates a C header file containing many arrays representing the
% windowed Room Impulse Response.

% TODO Make into a function
RIR_windowed_split; % Creates 'out' and 'N' in the workspace

windows = separate_windows_into_array(out, N);
filename = generate_impulse_response_arrays_header(windows);

fprintf('Generated a C header file named %s with impulse response arrays\n', filename);
