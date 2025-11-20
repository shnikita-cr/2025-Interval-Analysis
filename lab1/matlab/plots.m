x_h =  load('..\data\x_h.txt');
y_h =  load('..\data\y_h.txt');

plot(x_h,y_h, '-o');
legend('f');
set(gca, 'XAxisLocation', 'origin')
grid on