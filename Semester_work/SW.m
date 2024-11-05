table1 = dlmread("error_delta.txt");
table2 = dlmread("iter_delta.txt");
table3 = dlmread("error_iter.txt");

eps = table1(:,1);
square_err = table1(:,2);
lobatto_err = table1(:,3);
square_iter = table2(:,2);
lobatto_iter = table2(:,3);



figure;
loglog(eps, square_err);
hold on;
grid on;
loglog(eps, lobatto_err);
loglog(eps,eps);

legend("Ошибка по методу прямоугольников", "Ошибка по методу Лобатто", "Биссектриса", "Location", "northwest");

figure;
loglog(eps, square_iter);
hold on;
grid on;
loglog(eps, lobatto_iter);
legend("Количество итераций, метод прямоугольников", "Количество итераций, метод Лобатто")

figure;
loglog(square_iter, square_err);
hold on;
grid on;
loglog(lobatto_iter, lobatto_err);
legend("Ошибка по методу прямоугольников", "Ошибка по методу Лобатто");

