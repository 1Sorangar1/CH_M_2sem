table = dlmread("output.txt");

%Считываем данные
x1h = table(:,1);
y1h = table(:,2);
x1chebish = table(:,3);
y1chebish = table(:,4);

x2h = table(:,5);
y2h = table(:,6);
x2chebish = table(:,7);
y2chebish = table(:,8);

tablePoints = dlmread("outputPoints.txt");
xx1 = tablePoints(:,1);
yy1 = tablePoints(:,2);
N1h = tablePoints(:,3);
N1chebish = tablePoints(:,4);
xx2 = tablePoints(:,5);
yy2 = tablePoints(:,6);
N2h = tablePoints(:,7);
N2chebish = tablePoints(:,8);


%Строим разности 
yy1hdif = abs(N1h-yy1);
yy1chebishdif = abs(N1chebish-yy1);

yy2hdif = abs(N2h-yy2);
yy2chebishdif = abs(N2chebish-yy2);


%Строим графики

%1h
figure;
plot(xx1, yy1);
hold on
plot(xx1, N1h, "--g");
plot(x1h, y1h, "or");
grid on;
xlabel("X");
ylabel("Y");
legend("График функции", "График полинома для n = 10", "узлы", "Location", "south")

figure;
plot(xx1, yy1hdif)
hold on;
plot(xx1, abs(f1(xx1)-N1h), "--r");
xlabel("X");
ylabel("Величина ошибки");
legend("График поточечной ошибки", "Теоретическая ошибка для n = 10 узлов")



%1chebish
figure;
plot(xx1, yy1);
hold on
plot(xx1, N1chebish, "--g");
plot(x1chebish, y1chebish, "or");
grid on;
xlabel("X");
ylabel("Y");
legend("График функции", "График полинома для n = 10", "узлы", "Location", "south")

figure;
plot(xx1, yy1chebishdif)
hold on;
plot(xx1, abs(f1(xx1)-N1chebish), "--r");
xlabel("X");
ylabel("Величина ошибки");
legend("График поточечной ошибки", "Теоретическая ошибка для n = 10 узлов")

%2h
figure;
plot(xx2, yy2);
hold on
plot(xx2, N2h, "--g");
plot(x2h, y2h, "or");
grid on;
xlabel("X");
ylabel("Y");
legend("График функции", "График полинома для n = 10", "узлы", "Location", "south")

figure;
plot(xx2, yy2hdif)
hold on;
plot(xx2, abs(f2(xx2)-N2h), "--r");
xlabel("X");
ylabel("Величина ошибки");
legend("График поточечной ошибки", "Теоретическая ошибка для n = 10 узлов")

%2chebish
figure;
plot(xx2, yy2);
hold on
plot(xx2, N2chebish, "--g");
plot(x2chebish, y2chebish, "or");
grid on;
xlabel("X");
ylabel("Y");
legend("График функции", "График полинома для n = 10", "узлы", "Location", "south")

figure;
plot(xx2, yy2chebishdif)
hold on;
plot(xx2, abs(f2(xx2)-N2chebish), "--r");
xlabel("X");
ylabel("Величина ошибки");
legend("График поточечной ошибки", "Теоретическая ошибка для n = 10 узлов")

%пункт 4

table1 = dlmread("output1.txt");

mistakeh1 = table1(:,1);
mistakeh2 = table1(:,2);
mistakechebish1 = table1(:,3);
mistakechebish2 = table1(:,4);

n = linspace(5,100,96);

figure;
semilogy(n,mistakeh1);
xlabel("n");
ylabel("Величина максимальной ошибки");
legend("График максимальной ошибки");
grid on;

figure;
semilogy(n,mistakeh2);
xlabel("n");
ylabel("Величина максимальной ошyибки");
legend("График максимальной ошибки");
grid on;

figure;
semilogy(n,mistakechebish1);
xlabel("n");
ylabel("Величина максимальной ошибки");
legend("График максимальной ошибки");
grid on;

figure;
semilogy(n,mistakechebish2);
xlabel("n");
ylabel("Величина максимальной ошибки");
legend("График максимальной ошибки");
grid on;


%исследования в точке

table2 = dlmread("output2.txt");

mistakeh1_point = table2(:,1);
mistakeh2_point = table2(:,2);
mistakechebish1_point = table2(:,3);
mistakechebish2_point = table2(:,4);

figure;
semilogy(n,mistakeh1_point);
xlabel("n");
ylabel("Величина ошибки в точке");
legend("График ошибки в точке");
title("x = 1.0024")
grid on;

figure;
semilogy(n,mistakeh2_point);
xlabel("n");
ylabel("Величина ошибки в точке");
legend("График ошибки в точке");
title("x = 1.9995")
grid on;

figure;
semilogy(n,mistakechebish1_point);
xlabel("n");
ylabel("Величина ошибки в точке");
legend("График ошибки в точке");
title("x = 1.0024")
grid on;

figure;
semilogy(n,mistakechebish2_point);
xlabel("n");
ylabel("Величина ошибки в точке");
legend("График ошибки в точке");
title("x = 1.0024") 
grid on;


function y2 = f2(x)
y2 = sign(x).*(x.^4)-18.*(x.^2) + 2;
end

function y1 = f1(x)
y1 = sqrt(x)-cos(x);
end