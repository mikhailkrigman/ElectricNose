R = [83, 120];
T = [20, 350];

Legends = {'T_{Sens}(R_{Heizer})'};
p1= plot(R, T, 'LineWidth', 1.5, 'Color', 'red', 'Marker', '.','MarkerSize', 24);
lh = legend(p1, Legends, 'location', 'northwest', 'FontSize', 12);
title('Abhängigkeit der Sensortemperatur vom Heizerwiderstand', 'FontSize', 16);
ylabel('T_{Sensor} [^{\circ}C]', 'FontSize', 14);
xlabel('R_{Heizer} [\Omega]', 'FontSize', 14);
axis([70 130 -50 400]);
grid on