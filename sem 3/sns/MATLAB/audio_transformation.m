[x, Fs] = audioread('CantinaBand3.wav');

Fs_new = 60000; % Sampling rate for x(t) as 60000 Hz

x_resampled = resample(x, Fs_new, Fs); % Resample the signal to the new sampling rate
t = (0:length(x_resampled)-1)/Fs_new;  % Define time axis for the original signal


x_double = 2 * x_resampled; % Transformation 1: 2*x(t)
x_reversed = flip(x_resampled); % Transformation 2: x(-t) (time reversal)
x_scaled2 = resample(x_resampled, 1, 2); % Transformation 3: x(2t) (time scaling)
x_scaled_half = resample(x_resampled, 2, 1); % Transformation 3: x(2t) (time scaling)

%% 

disp('Playing x(t)');
sound(x_resampled, Fs_new);
pause(length(x_double)/Fs_new + 2);
%% 

disp('Playing 2*x(t)');
sound(x_double, Fs_new);
pause(length(x_double)/Fs_new + 2); % Adding a small delay for clarity
%% 

disp('Playing x(-t)');
sound(x_reversed, Fs_new);
pause(length(x_reversed)/Fs_new + 2);
%% 

disp('Playing x(2t)');
sound(x_scaled2, Fs_new);
pause(length(x_scaled2)/Fs_new + 2);
%% 

disp('Playing x(0.5*t)');
sound(x_scaled_half, Fs_new);
pause(length(x_scaled_half)/Fs_new + 2);
