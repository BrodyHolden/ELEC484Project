function out = fft_reverb(y_T,y_R)

% [y_T,Fs_T]=audioread('Two Voices_1.wav'); %AK: read in the guitar file
% [y_R,Fs_R]=audioread('60ms_church_44100.wav');  %AK: read in the Room impulse response

N = 2646; %AK:set the room window size to 2646 samples, the size of the  60ms room impulse response, change this to 2880 when working with a 48000Hz input

hop = N/2; 


w = hanningz(N)'; %AK: make a 2464 sample wide Hanning Window

y_R_fft = fft(y_R); %AK: take fft of RIR


out = zeros (length(y_T),1); %AK: create output buffer of length of song
i=0;

while i < ((length(y_T) / hop)-2)
    
    if i==0 %AK: apply first window without a hop
        w_hold=w; %AK: make a copy of the window for use in this if statement
        w2 = transpose(w_hold); %AK: transpose it to be ablle to use .*
        w2(numel(y_T))=0; %AK: add zeros to the end until to pad the length to length(y_T)
        y_T_w = y_T.*w2; %AK: multiply y_T by window
        y_T_w_nz= y_T_w(1:N); %AK: take non-zero parts of y_T
        fft_y_T= fft(y_T_w_nz); %AK: take fft of non-zero parts
        y_T_reverb = fft_y_T.*y_R_fft; %AK: multiply by RIR fft
        y_out = ifft(y_T_reverb); %AK: take ifft of product to get time domain with reverb
        out(1:N)=out(1:N)+y_out; %AK: out time domeain sigan in the output buffer
        i=i+1;
    else
    
w_hopped = [zeros(1,hop*i), w]; %AK: add i hop sizes of zeros to the front of the window

w_column = transpose(w_hopped); %AK: turn the Hanning window from a row vector into a column vector to be able to multiply it to te  song -> may be able to fix the function to return a row vector

w_column(numel(y_T)) = 0; %AK: add zeros to the end of the window to make it the same length as the song so that it can be multiplied

y_T_windowed = w_column.*y_T; %AK: multiply the song by the hanning window
 
y_T_windowed_non_zero = y_T_windowed((hop*i)+1 : hop*(i+2)); %AK: take the nonzero elements of the song 

y_T_windowed_non_zero_fft = fft(y_T_windowed_non_zero); %AK:take the fft of the windowed part of the song

y_out_fft = y_T_windowed_non_zero_fft.*y_R_fft; %AK: Multiply ffts

y_out_ifft = ifft(y_out_fft); %AK: take ifft to get time domain response

out((hop*i)+1:hop*(i+2))=out((hop*i)+1:hop*(i+2))+ y_out_ifft; %AK: overlap adding in iffts of the windows to the output buffers

i = i+1;

    end
end