close all;
clc;

% config
file_path = './ocr-imsize-parsed.dat';

% load data
data = load(file_path) ./ 1e3; % convert in ms

% plot
log_scale = 0;
smooth_plot = 0;
createfigure(1:length(data), data,...
    'Enclave Time [ms]', {}, log_scale, smooth_plot);
