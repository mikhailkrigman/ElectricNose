r_measured_values = sort([8237.017, 8000; ...
                          10015.021, 10000; ...
                          12032.505, 12000; ...
                          15174.884, 15000; ...
                          20239.047, 20000; ...
                          26961.685, 27000; ...
                          39088.213, 39000; ...
                          46746.840, 47000; ...
                          55369.405, 56000; ...
                          81910.363, 82000]);

r_should_values = [8, 10, 12, 15, 20, 27, 39, 47, 56, 82];

ph1 = bar(r_measured_values);
yticks(r_measured_values(:, 1))
yticklabels(r_measured_values(:, 1));
xticklabels(r_should_values);

xlabel('Testwiderstand [k\Omega]', 'FontSize', 16);
ylabel('Gemessener Widerstand [\Omega]', 'FontSize', 16);

legends = {'Gemessener Widerstand', 'Testwiderstand'};
lh = legend(ph1, legends, 'Location','northwest');
fontsize(lh, 14, 'points')