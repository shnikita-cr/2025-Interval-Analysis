folderId = '1.2'; 
dataDir = ['../data/', folderId, '/'];

xi = load([dataDir, 'x_k_iter_ratio.txt']);
iter = xi(:,1);
x_iter = xi(:,2);

figure(2);
plot(iter, x_iter, 'ko--');
hold on; 
grid on;
xlabel('iteration');
ylabel('Normalized \Sigma len(X_i)');
legend('x_c=0.0', 'x_c=0.5', 'x_c=1.0', 'x_c=1.2');
title(['Normalized \Sigma len(X_i) by iter on x_c']);



% saveas(gcf,['../report/images/',folderId,'/x_k_iter.png'])