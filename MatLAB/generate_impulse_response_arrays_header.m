function [filename] = generate_impulse_response_arrays_header(windows)
% Input parameter windows must be a 'cell' that contains arrays.

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

        fprintf(fileID, format, winIndex, sz(1) * 2);

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
