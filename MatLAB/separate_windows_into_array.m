function [windows] = separate_windows_into_array(out, N)
    
    window_1 = out(1:N);
    window_2 = out(1*N+1:N*2);
    window_3 = out(2*N+1:N*3);
    window_4 = out(3*N+1:N*4);
    window_5 = out(4*N+1:N*5);
    window_6 = out(5*N+1:N*6);
    window_7 = out(6*N+1:N*7);
    window_8 = out(7*N+1:N*8);
    window_9 = out(8*N+1:N*9);
    window_10 = out(9*N+1:N*10);
    window_11 = out(10*N+1:N*11);
    window_12 = out(11*N+1:N*12);

    % Stored as the 'cell' datatype.
    windows = { window_1, window_2, window_3, window_4, window_5, window_6, window_7, window_8, window_9, window_10, window_11, window_12 };
    
end