close all;
clc;

%% config
folder_path = './ocr-data';
file_format = 'parsed_ocr_log=%d.dat.%d';
num_measurements = 10;
num_cycles = 99;
matrix_sizes = [1];
legend_title = {'1kB image'};


%% init
cycles = zeros(num_cycles);
mean_total_time = zeros(num_cycles,length(matrix_sizes));
mean_enclave_time = zeros(num_cycles,length(matrix_sizes));
std_total_time = zeros(num_cycles,length(matrix_sizes));
std_enclave_tim = zeros(num_cycles,length(matrix_sizes));
    
%% load data
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
        total_time(:,j) = data(:,2) ./ 1e6;
        enclave_time(:,j) = data(:,3) ./ 1e6;
    end
    
    % apply simulated network delay
    for k=1:size(total_time, 1)
        for l=1:size(total_time, 2)
            total_time(k,l) = total_time(k,l) + (k-1)*total_time(1,l);
        end
    end

    % compute avg and std
    mean_total_time(:,i) = mean(total_time,2);
    mean_enclave_time(:,i) = mean(enclave_time,2);
    std_total_time(:,i) = std(total_time,[],2);
    std_enclave_time(:,i) = std(enclave_time,[],2);
    
    end
    
    
%% plot
log_scale = 1;
smooth_plot = 0;
createfigure(cycles, mean_total_time,...
    'Total Time [s]', legend_title, log_scale, smooth_plot);
createfigure(cycles, mean_enclave_time,...
    'Enclave Time [s]', legend_title, log_scale, smooth_plot);

%figure;
%boundedline(cycles,mean_enclave_time,std_enclave_time, '-rx')
