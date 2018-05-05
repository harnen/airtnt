close all;
clc;

%% config
% config - Game of Life
% folder_path = './life-data';
% file_format = 'parsed_log_size=%d.dat.%d';
% num_measurements = 10;
% num_cycles = 99;
% matrix_sizes = [10 25 50];
% legend_title = {'10x10 matrix', '25x25 matrix', '50x50 matrix'};

% config - OCR
folder_path = './ocr-data';
file_format = 'parsed_ocr_log=%d.dat.%d';
num_measurements = 10;
num_cycles = 99;
matrix_sizes = [1];
legend_title = {};


%% parse data
% init
cycles = zeros(num_cycles);
mean_total_time = zeros(num_cycles,length(matrix_sizes));
mean_enclave_time = zeros(num_cycles,length(matrix_sizes));
std_total_time = zeros(num_cycles,length(matrix_sizes));
std_enclave_tim = zeros(num_cycles,length(matrix_sizes));
    
% load data
for i=1:length(matrix_sizes)
    % init
    total_time = zeros(num_cycles,num_measurements);
    enclave_time = zeros(num_cycles,num_measurements);

    % gather measurements
    for j=1:num_measurements
        data_path = sprintf(...
            file_format,matrix_sizes(i),(j-1)...
        );
        data = load(fullfile(folder_path, data_path));
        cycles = data(:,1);
        total_time(:,j) = data(:,2) ./ 1e3; % total time in [ms]
        enclave_time(:,j) = data(:,3) ./ 1e3; % enclave time in [ms]
    end
    
    % compute avg and std
    mean_total_time(:,i) = mean(total_time,2);
    mean_enclave_time(:,i) = mean(enclave_time,2);
    std_total_time(:,i) = std(total_time,[],2);
    std_enclave_tim(:,i) = std(enclave_time,[],2);
    
end


%% plot
log_scale = 0;
smooth_plot = 0;
createfigure(cycles, mean_total_time,...
    'Total Time [ms]', legend_title, log_scale, smooth_plot);
createfigure(cycles, mean_enclave_time,...
    'Enclave Time [ms]', legend_title, log_scale, smooth_plot);

plot(cycles, std_enclave_tim(:,1))

