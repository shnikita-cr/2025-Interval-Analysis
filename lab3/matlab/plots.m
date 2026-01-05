% Загрузка данных из файла
data = load('../data/results.txt');

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

% % ========== ГРАФИК 3: 2D SCATTER ==========
% figure(3);
% scatter(x, y, 100, f, 'filled', 's');
% colorbar;
% title('2D Scatter Plot with Color Markers');
% xlabel('x');
% ylabel('y');
% colormap(jet);

% % Расположим все графики на экране
% for i = 1:3
%     figure(i);
%     set(gcf, 'Position', [100+(i-1)*350, 100, 350, 350]);
% end

% Сохранение графиков 
% saveas(figure(1), 'pcolor_plot.png');
% saveas(figure(2), '3d_scatter.png');
% saveas(figure(3), '2d_scatter.png');