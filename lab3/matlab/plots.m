% Загрузка данных из файла
folderId = 1;
% data = load(['../data/', num2str(folderId), '/_init_tolS.txt']);

% data = load(['../data/', num2str(folderId), '/before_cor_tolF_val.txt']);
% mx = load(['../data/', num2str(folderId), '/before_cor_tolF_max.txt']);

% data = load(['../data/', num2str(folderId), '/cb__tolF_val.txt']);
% mx = load(['../data/', num2str(folderId), '/cb__tolF_max.txt']);
% 
data = load(['../data/', num2str(folderId), '/cA__tolF_val.txt']);
mx = load(['../data/', num2str(folderId), '/cA__tolF_max.txt']);
% 
% data = load(['../data/', num2str(folderId), '/cAb_tolF_val.txt']);
% mx = load(['../data/', num2str(folderId), '/cAb_tolF_max.txt']);


mx_x = mx(:,1);
mx_y = mx(:,2);
mx_f = mx(:,3);

% Разделение данных на столбцы
x = data(:, 1);
y = data(:, 2);
f = data(:, 3);

% Определение уникальных значений x и y
x_unique = unique(x);
y_unique = unique(y);

% Создание сетки для построения
[X, Y] = meshgrid(x_unique, y_unique);

% Преобразование данных в матрицу
Z = zeros(length(y_unique), length(x_unique));

% Заполнение матрицы Z значениями f(x,y)
for i = 1:length(x_unique)
    for j = 1:length(y_unique)
        idx = find(x == x_unique(i) & y == y_unique(j));
        if ~isempty(idx)
            Z(j, i) = f(idx(1));
        end
    end
end

% ========== ГРАФИК 1: PCOLOR ==========
figure;
pcolor(X, Y, Z);
shading interp;
colorbar;
title('2D Color Plot - PCOLOR');
xlabel('x');
ylabel('y');
colormap(parula);
hold on;
mask = Z >= 0;
plot(mx_x, mx_y, 'kx', 'MarkerSize', 14, 'LineWidth', 2);  % all points in the file

[C, h_contour] = contourf(X, Y, mask, [0.5, 0.5]);
set(h_contour, 'FaceColor', 'none', 'EdgeColor', 'r', 'LineWidth', 2);

% ========== ГРАФИК 2: 3D SCATTER ==========
figure;
scatter3(x, y, f, 50, f, 'filled');
colorbar;
title('3D Scatter Plot');
xlabel('x');
ylabel('y');
zlabel('f(x,y)');
grid on;
colormap(parula);

% Сохранение графиков 
% saveas(figure(1), 'pcolor_plot.png');
% saveas(figure(2), '3d_scatter.png');