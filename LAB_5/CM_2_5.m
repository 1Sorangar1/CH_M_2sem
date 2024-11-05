table1 = dlmread("output1.txt");
table2 = dlmread("output2.txt");
table3 = dlmread("output3.txt");
table4 = dlmread("output4.txt");

xh1 = table1(:, 1);
yh1Method = table1(:, 2);
Correct = table1(:, 3);
h1Mistake = table1(:, 4);

xh2 = table2(:, 1);
yh2Method = table2(:, 2);
h2Mistake = table2(:, 4);

xh = table3(:,1);
h = table3(:,2);

delta = table4(:,1);
Mistake = table4(:,2);

%график 1 - результат метода
plot(xh1, yh1Method);
hold on;
grid on;
plot(xh2,yh2Method);
plot(xh1, Correct, "y--");
legend("h=0.1", "h = 0.2", "Correct solution")

%график 2 - ошибка метода
figure;
plot(xh1, h1Mistake);
hold on;
grid on;
plot(xh2, h2Mistake);
legend("h=0.1 mistake", "h = 0.2 mistake")

% график 3 - шаг от отрезка
figure;
plot(xh,h);
grid on;
legend("step");

%график 4 - ошибка от точности
figure;
loglog(delta, Mistake);
grid on;
legend("mistake", "Location", "east")






