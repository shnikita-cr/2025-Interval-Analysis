x_h =  load('..\data\x_h.txt');
y_f_linear =  load('..\data\y_f.txt');


plot(x_h,y_f_linear, '-');
set(gca, 'XAxisLocation', 'origin')
grid on
hold on
legend('y\_f')