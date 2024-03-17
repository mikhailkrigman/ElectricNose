clear all
hold on

file = readlines('C:\Users\yamis\Desktop\Entwicklung elektronischen Nase\data_log.txt');

times = [];
r_sens_values = [];

empty_lines = 0;
% Format -> hh:mm:ss#R_sens#R_heater
for i = 1:length(file)
    line = file(i);
    if contains(line, '#')
        data = line.split('#');
    
        full_time = data(1);
        split_time = full_time.split(':');

        seconds = double(split_time(3)) + 60 * double(split_time(2)) + 60 * 60 * double(split_time(1));
        r_sens = double(data(2));

        times = [times, seconds];
        r_sens_values = [r_sens_values, r_sens];
    else
        empty_lines = empty_lines + 1;
    end

end

p1 = plot(times, r_sens_values, 'LineWidth', 1.5, 'Color', 'red');
%xlim([13000 50088])
xlabel('t [s]', 'FontSize', 18);
ylabel('R_{Sens}(t) [\Omega]', 'FontSize', 16);
Legends = {'R_{Sens}'};
lh = legend(Legends, 'location', 'northeast', 'FontSize', 14);

grid off
