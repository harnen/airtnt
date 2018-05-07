close all;
clc;

%% config
life_folder_path = './life-data';
life_file_format = 'parsed_log_size=%d.dat.%d';

ocr_folder_path = './ocr-data';
ocr_file_format = 'parsed_ocr_log=%d.dat.%d';

num_measurements = 10;
legend_title = {'Game of Life', 'OCR'};

num_cycles = 99;
steps = 10;
total_iter = 1000;

%% parse life data
% load data
life_enclave_time = zeros(num_cycles,num_measurements);

% gather measurements
for j=1:num_measurements
    data_path = sprintf(...
        life_file_format,50,(j-1)... % 50x50 matrix size
    );
    data = load(fullfile(life_folder_path, data_path));
    life_enclave_time(:,j) = data(:,3) ./ 1e3; % enclave time in [ms]
end

% compute avg and std
life_mean_enclave_time = mean(life_enclave_time,2);
std_enclave_time = std(life_enclave_time,[],2);


%% parse OCR data
% load data
ocr_enclave_time = zeros(num_cycles,num_measurements);

% gather measurements
for j=1:num_measurements
    data_path = sprintf(...
        ocr_file_format,1,(j-1)... % 50x50 matrix size
    );
    data = load(fullfile(ocr_folder_path, data_path));
    ocr_enclave_time(:,j) = data(:,3) ./ 1e6; % enclave time in [ms]
end

% compute avg and std
ocr_mean_enclave_time = mean(ocr_enclave_time,2);
ocr_enclave_time = std(ocr_enclave_time,[],2);


%% compute enclave calls
num_enclave_calls = ones(length(ocr_mean_enclave_time),1);
for i=1:length(ocr_mean_enclave_time)
    num_enclave_calls(i) = ceil(total_iter / (10*i));
end


%% plot
log_scale = 0;
smooth_plot = 1;

enclave_calls_fig(num_enclave_calls, life_mean_enclave_time,...
    'enclave time [ms]', {'Game of Life'}, log_scale, smooth_plot);

enclave_calls_fig(num_enclave_calls, ocr_mean_enclave_time,...
    'enclave time [s]', {'OCR'}, log_scale, smooth_plot);





