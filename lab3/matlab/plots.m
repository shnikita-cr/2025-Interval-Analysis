% Загрузка данных из файла
% data = load('../data/1/check_tolerance_set.txt');
data = load('../data/1/no_cor_tol.txt');

mx = load('../data/1/no_cor_max.txt');
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
figure(1);
pcolor(X, Y, Z);
shading interp;
colorbar;
title('2D Color Plot - PCOLOR');
xlabel('x');
ylabel('y');
colormap(parula);
hold on;
plot(mx_x, mx_y, 'kx', 'MarkerSize', 14, 'LineWidth', 2);  % all points in the file


% ========== ГРАФИК 2: 3D SCATTER ==========
figure(2);
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