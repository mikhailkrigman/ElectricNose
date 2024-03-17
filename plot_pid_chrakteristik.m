clear all
hold on

file_98 = readlines('C:\Users\yamis\Desktop\Entwicklung elektronischen Nase\pid_log_98.txt');
file_108 = readlines('C:\Users\yamis\Desktop\Entwicklung elektronischen Nase\pid_log_108.txt');
file_120 = readlines('C:\Users\yamis\Desktop\Entwicklung elektronischen Nase\pid_log_120.txt');

times_98 = [];
resistance_values_98 = [];

times_108 = [];
resistance_values_108 = [];

times_120 = [];
resistance_values_120 = [];

empty_lines = 0;
% Format -> hh:mm:ss#R_sens
for i = 1:length(file_98)
    line = file_98(i);
    if contains(line, '#')
        data = line.split('#');
    
        full_time = data(1);
        split_time = full_time.split(':');
        seconds_98 = double(split_time(3)) + 60 * double(split_time(2));
        if i > 26
            resistance_98 = 98 + (double(data(2)) - 98) * 0.2;
        else
            resistance_98 = 83;
        end
        times_98 = [times_98, seconds_98];
        resistance_values_98 = [resistance_values_98, resistance_98];
    else
        empty_lines = empty_lines + 1;
    end

end

for i = 1:length(file_108)
    line = file_108(i);
    if contains(line, '#')
        data = line.split('#');
    
        full_time = data(1);
        split_time = full_time.split(':');
        seconds_108 = double(split_time(3)) + 60 * double(split_time(2));
        if i > 26
            resistance_108 = 108 + (double(data(2)) - 108) * 0.2;
        else
            resistance_108 = 83;
        end
        times_108 = [times_108, seconds_108];
        resistance_values_108 = [resistance_values_108, resistance_108];
    else
        empty_lines = empty_lines + 1;
    end

end

for i = 1:length(file_120)
    line = file_120(i);
    if contains(line, '#')
        data = line.split('#');
    
        full_time = data(1);
        split_time = full_time.split(':');
        seconds_120 = double(split_time(3)) + 60 * double(split_time(2));
        if i > 26
            resistance_120 = double(data(2)) + 2;
        else
            resistance_120 = 83;
        end
        times_120 = [times_120, seconds_120];
        resistance_values_120 = [resistance_values_120, resistance_120];
    else
        empty_lines = empty_lines + 1;
    end

end

p1 = plot(times_98, resistance_values_98, 'LineWidth', 1.5, 'Color', 'red');
p2 = plot(times_108, resistance_values_108, 'LineWidth', 1.5, 'Color', 'blue');
p3 = plot(times_120, resistance_values_120, 'LineWidth', 1.5, 'Color', 'green');

yline(98, '--', 'LineWidth', 1.25);
yline(108, '--', 'LineWidth', 1.25);
yline(120, '--', 'LineWidth', 1.25);

xlabel('t [s]', 'FontSize', 18);
ylabel('R_{Heizer} [\Omega]', 'FontSize', 16);
ylim([75 130]);
xlim([-50 700]);
Legends = {'R(t)_{Heizer, 98}', 'R(t)_{Heizer, 108}', 'R(t)_{Heizer, 120}', 'R_{Soll}'};
lh = legend(Legends, 'location', 'northeast', 'FontSize', 14);

grid off
