table = dlmread("10000.txt");
table1 = dlmread("1000.txt");
table_err = dlmread("runge_err.txt");
table_h = dlmread("runge_h.txt");

%task 1

%n = 10000
x = table(:,1);
y = table(:,2);
err = table(:,3);
y_correct = table(:,4);

plot(x,y)
hold on;
plot(x,y_correct, "--r");
legend("Приближённое решение", "Точное решение");

figure
semilogy(x, err);
legend("Поточечная ошибка", "Location", "east");

%n = 1000

x1 = table1(:,1);
y1 = table1(:,2);
err1 = table1(:,3);
y_correct1 = table1(:,4);

figure;
plot(x1,y1)
hold on;
plot(x1,y_correct1, "--r");
legend("Приближённое решение", "Точное решение");

figure
semilogy(x1, err1);
legend("Поточечная ошибка", "Location", "east");

%task 2

%step
zero_index = find(table_step(:, 1) == 0, 1);
x_current = table_step(1:zero_index-1, 2);
step = table_step(1:zero_index-1, 1);
figure;
semilogy(x_current, log10(step));

legend("Величина шага в логарифмическом масштабе 10", "Location", "west")

eps = table_err(:,2);
error = table_err(:,1);
h = table_h(:,1);

figure;
loglog(eps,error)
legend("Фактическая ошибка")

