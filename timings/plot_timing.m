close all
clc

% load data
data = load('/Users/alberto/Downloads/results.dat');
cycles = data(:,1);
total_time = data(:,2) ./ 1e3; % total time in [ms]
enclave_time = data(:,3) ./ 1e3;  % enclave time in [ms]

% plot
createfigure(cycles, total_time, 'Total Time [ms]')
createfigure(cycles, enclave_time, 'Enclave Time [ms]')
