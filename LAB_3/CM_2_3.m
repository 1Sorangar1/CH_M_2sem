table = dlmread("output.txt");

delta = table(:,1);
counter = table(:,2);
h = table(:,3);
degree = table(:,4);

%фактическая ошибка от точности
loglog(degree, delta, "--r");
hold on;
grid on;
loglog(degree,degree,"b");
legend("Фактическая ошибка от точности", "Биссектриса", "location", "southeast");

%число итераций от точности
figure;
semilogx(degree,counter);
grid on;
legend("Число итераций от точности");

%фактическая ошибка от длины отрезка разбиения
figure;
plot(log2(h), log2(delta));
grid on;
legend("Фактическая ошибка от длины отрезка разбиения")