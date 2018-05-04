close all
clc

data_path = './data';
num_measurements = 10;
num_cycles = 100;
matrix_sizes = [10 25 50];

% load data
cycles = zeros(num_cycles,num_measurements);
for v = matrix_sizes
    for j=1:num_measurements
        data_path = sprintf(...
            'parsed_log_size=%d.dat.%d',v,(j-1)...
        );
        data = load(data_path);
        cycles(:,j) = data(:,1);
        total_time(:,j) = data(:,2) ./ 1e3; % total time in [ms]
        enclave_time(:,j) = data(:,3) ./ 1e3; % enclave time in [ms]
    end
end
data = load('/Users/alberto/Downloads/results.dat');

mean_total_time = mean(total_time,2);
mean_enclave_tim = mean(enclave_tim,2);
  

% plot
%createfigure(cycles, total_time, 'Total Time [ms]')
%createfigure(cycles, enclave_time, 'Enclave Time [ms]')
