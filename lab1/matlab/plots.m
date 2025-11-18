
x_h =  load('..\data\x_h.txt');
y_h =  load('..\data\y_h.txt');


% subplot(2,1,1);
plot(x_h,y_h, '-o');
legend('f');
set(gca, 'XAxisLocation', 'origin')
grid on

% subplot(2,1,2);
% plot(x_h,y_h,'-o');
% hold on
% plot(x_h,y_h_true,'-o');
% legend('y\_h','y\_h\_true');
% set(gca, 'XAxisLocation', 'origin')
% grid on