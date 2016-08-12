function [filename] = generate_impulse_response_arrays_header_float_real_imag(windows)
% GENERATE_IMPULSE_RESPINSE_ARRAYS_HEADER_FLOAT_REAL_IMAG()
% Output a C header file containing floats of real and imaginary impulse
% response.
% Input parameters:
%   windows: Cell of arrays of complex numbers
% Output parameters:
%   filename: Name of generated file.

    filename = 'impulse_response_arrays.h';

    fileID = fopen(filename, 'wt');

    % TODO Check for fopen error.
    
    fprintf(fileID, '/* Auto generated in matlab by %s */\n\n', mfilename);
    fprintf(fileID, '#ifndef IMPLUSE_RESPONSE_ARRAYS_H\n');
    fprintf(fileID, '#define IMPLUSE_RESPONSE_ARRAYS_H\n\n');

    windowsSize = size(windows);
    
    for winIndex = 1:windowsSize(2)
        window = windows{winIndex};

        re = real(window);
        im = imag(window);
        sz = size(window);
        
        format = 'const float impulseResponse_%d[%d] = {\n';

        fprintf(fileID, format, winIndex - 1, sz(1) * 2);

        for i = 1:sz(1)
            if i ~= 1
                fprintf(fileID, ', ');
            end
            fprintf(fileID, '%f, %f', re(i), im(i));
        end

        fprintf(fileID, '\n};\n\n');
    end

    fprintf(fileID, '#endif /* IMPLUSE_RESPONSE_ARRAYS_H */\n');
    
    fclose(fileID);
    
end
