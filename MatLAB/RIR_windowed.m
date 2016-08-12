function [out, Fs_R, N] = RIR_windowed(filename)
% RIR_windowed() Split Room Impulse Response by hanning windows
% returned as windows one after another in an array.
% Input parameters:
%   filename: Name of Room Impulse Response file.
% Output parameters:
%   out: Array of windows end to end
%   Fs_R: Sample rate in Hertz
%   N: Size of window

    samples = [1,2880];
    [y_R,Fs_R]=audioread(filename, samples);  %AK: read in the Room impulse response

    N = 480; %AK:set the room window size to 480 to get 10ms of the RIR

    hop = N/2;

    w = hanningz(N)'; %AK: make a 480 sample wide Hanning Window

    %out = zeros (length(y_R),1); %AK: create output buffer of length of RIR
    i=-1;
    loop=((length(y_R) / hop)-1);
    while i < loop %((length(y_R) / hop)-2)

        if i==-1 %AK: apply first window without a hop
            w_hold=w(hop/2:hop); %AK: take second half of the window to avoid loosing half of the beginning
            w2 = transpose(w_hold); %AK: transpose it to be able to use .*
            w2(numel(y_R))=0; %AK: add zeros to the end until to pad the length to length(y_R)
            y_R_w = y_R.*w2; %AK: multiply y_T by window
            y_R_w_nz= y_R_w(1:N); %AK: take non-zero parts of y_T
            fft_y_R= fft(y_R_w_nz); %AK: take fft of non-zero parts
            out(1:N)=out(1:N)+fft_y_R; %AK: out time domeain signal in the output buffer
            i=i+1;
        elseif i==0 %AK: take one window without a hop
            w_hold_1=w; %AK: make copy of hanning window to use in this if statement
            w2 = transpose(w_hold_1); %AK: transpose it to be able to use .*
            w2(numel(y_R))=0; %AK: add zeros to the end until to pad the length to length(y_R)
            y_R_w = y_R.*w2; %AK: multiply y_T by window
            y_R_w_nz= y_R_w(1:N); %AK: take non-zero parts of y_T
            fft_y_R= fft(y_R_w_nz); %AK: take fft of non-zero parts
            out(1:N)=out(1:N)+fft_y_R; %AK: out time domeain signal in the output buffer
            i=i+1;
        else
    w_hopped = [zeros(1,hop*i), w]; %AK: add i hop sizes of zeros to the front of the window

    w_column = transpose(w_hopped); %AK: turn the Hanning window from a row vector into a column vector to be able to multiply it to te  song -> may be able to fix the function to return a row vector

    w_column(numel(y_R)) = 0; %AK: add zeros to the end of the window to make it the same length as the RIR so that it can be multiplied

    y_R_windowed = w_column.*y_R; %AK: multiply the RIR by the hanning window

    y_R_windowed_nz = y_R_windowed((hop*i)+1 : hop*(i+2)); %AK: take the nonzero elements of the song

    y_R_windowed_nz_fft = fft(y_R_windowed_nz); %AK:take the fft of the windowed part of the song

    out((hop*i)+1:hop*(i+2))=out((hop*i)+1:hop*(i+2))+ y_R_windowed_nz_fft; %AK: overlap adding in iffts of the windows to the output buffers

    i = i+1;

        end
    end

    w_hopped = [zeros(1,hop*i), w((N/2)+1:N)]; %AK: add i hop sizes of zeros to the front of the window

    w_column = transpose(w_hopped); %AK: turn the Hanning window from a row vector into a column vector to be able to multiply it to te  song -> may be able to fix the function to return a row vector

    w_column(numel(y_R)) = 0; %AK: add zeros to the end of the window to make it the same length as the RIR so that it can be multiplied

    y_R_windowed = w_column.*y_R; %AK: multiply the RIR by the hanning window

    y_R_windowed_nz = y_R_windowed((hop*i)+1 : hop*(i+1)); %AK: take the nonzero elements of the song

    y_R_windowed_nz_fft = fft(y_R_windowed_nz); %AK:take the fft of the windowed part of the song

    out((hop*i)+1:hop*(i+1))=out((hop*i)+1:hop*(i+1))+ y_R_windowed_nz_fft; %AK: overlap adding in iffts of the windows to the output buffers

end