% ====== B1: 2D plots for Add / Mul with max point ======

folderId = 'b1';
ttl1 = 'F(a)';
ttl2 = 'F(t)';

dataAdd = load(['../data/', folderId, '/add_val.txt']);
dataMul = load(['../data/', folderId, '/mul_val.txt']);
xAdd = dataAdd(:,1);
fAdd = dataAdd(:,2);
xMul = dataMul(:,1);
fMul = dataMul(:,2);

mxAdd   = load(['../data/', folderId, '/add_max.txt']);
mxMul   = load(['../data/', folderId, '/mul_max.txt']);
mxAdd_x = mxAdd(1,1);
mxAdd_f = mxAdd(1,2);
mxMul_x = mxMul(1,1);
mxMul_f = mxMul(1,2);

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
title([folderId, 'a+X=Y: ', ttl1]);
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
title([folderId, 't*X=Y: ', ttl2]);
hold off;
