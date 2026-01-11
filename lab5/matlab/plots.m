% ====== B1: 2D plots for Add / Mul with max point ======
folderId = '1.0';
ttl1 = 'f_1(X_1,X_2), f_2(X_1,X_2)';
ttl2 = 'f_1(),f_2()';

dataAdd = load(['../data/', folderId, '/plot.txt']);
dataMul = load(['../data/', folderId, '/lines.txt']);
xAdd = dataAdd(:,1);
fAdd = dataAdd(:,2);
xMul = dataMul(:,1);
fMul = dataMul(:,2);

addInt  = load(['../data/', folderId, '/add_int.txt']);  % 2 numbers: left right
mulInt  = load(['../data/', folderId, '/mul_int.txt']);  % 2 numbers: left right
aL = addInt(1); aR = addInt(2);
tL = mulInt(1); tR = mulInt(2);

% -------- FIGURE 1: ADD --------
figure; clf;
plot(xAdd, fAdd, 'LineWidth', 1.5); grid on; hold on;
yl = ylim;
plot([aL aL], yl, 'k--', 'LineWidth', 1.2);
plot([aR aR], yl, 'k--', 'LineWidth', 1.2);
plot(mxAdd_x, mxAdd_f, 'ko', 'MarkerSize', 8, 'LineWidth', 1.5); % black circle
xlabel('a');
ylabel('F(a)');
title([folderId, ': a+X=Y: ', ttl1]);
hold off;

% -------- FIGURE 2: MUL --------
figure; clf;
plot(xMul, fMul, 'LineWidth', 1.5); grid on; hold on;
yl = ylim;
plot([tL tL], yl, 'k--', 'LineWidth', 1.2);
plot([tR tR], yl, 'k--', 'LineWidth', 1.2);
plot(mxMul_x, mxMul_f, 'ko', 'MarkerSize', 8, 'LineWidth', 1.5); % black circle
xlabel('t');
ylabel('F(t)');
title([folderId, ': t*X=Y: ', ttl2]);
hold off;
