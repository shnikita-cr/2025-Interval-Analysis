% ====== Kravchik task: visualize f1=0, f2=0 and boxes X(k) ======
% Expected files in ../data/<folderId>/ :
%   f1_grid.txt   : columns [x1 x2 f1(x1,x2)]
%   f2_grid.txt   : columns [x1 x2 f2(x1,x2)]
%   boxes.txt     : columns [x1 x2], rectangles separated by NaN NaN
%
% This script draws:
%   - FIGURE 1: contour f1=0 + f2=0 + all boxes

folderId = '0.5';                % e.g. '0.0', '0.5', '1.0', '1.2'
dataDir = ['../data/', folderId, '/'];

% ---------- Load grids ----------
g1 = load([dataDir, 'f1_grid.txt']);   % x1 x2 f1
g2 = load([dataDir, 'f2_grid.txt']);   % x1 x2 f2

x1 = g1(:,1);
x2 = g1(:,2);
f1 = g1(:,3);
f2 = g2(:,3);

% Build regular mesh from scattered-by-row grid (like your old script)
x1u = unique(x1);
x2u = unique(x2);
[X1, X2] = meshgrid(x1u, x2u);

Z1 = nan(length(x2u), length(x1u));
Z2 = nan(length(x2u), length(x1u));

% Fast fill assuming the file was generated row-major: for each x2 row, all x1 columns
% (that’s exactly how saveFunctionGrid2d writes it)
n1 = length(x1u);
n2 = length(x2u);
if size(g1,1) ~= n1*n2
    % fallback safe fill (slower)
    for i = 1:length(x1u)
        for j = 1:length(x2u)
            idx = find(x1 == x1u(i) & x2 == x2u(j));
            if ~isempty(idx)
                Z1(j,i) = f1(idx(1));
                Z2(j,i) = f2(idx(1));
            end
        end
    end
else
    Z1(:) = reshape(f1, [n1, n2])';   % transpose because meshgrid uses rows for x2
    Z2(:) = reshape(f2, [n1, n2])';
end

% ---------- Load boxes ----------
boxes = load([dataDir, 'boxes.txt']);   % x y with NaN separators
bx = boxes(:,1);
by = boxes(:,2);

% ---------- FIGURE 1: contour f1=0 and f2=0 + boxes ----------
figure(1); clf; hold on; grid on;

% Level sets
contour(X1, X2, Z1, [0 0], 'LineWidth', 2);          % f1=0
contour(X1, X2, Z2, [0 0], 'LineWidth', 2, 'LineStyle', '--'); % f2=0

% Boxes as dashed polylines (NaNs split segments automatically)
plot(bx, by, 'k--', 'LineWidth', 1.0);

xlabel('x_1');
ylabel('x_2');
title([folderId, ': level sets f_1=0 (solid), f_2=0 (dashed); X(k)']);
axis equal;
hold off;

saveas(gcf,['../report/images/',folderId,'/plot.png'])
