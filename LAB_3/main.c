#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define S 20.13333333333

typedef struct {
	double value;
	int degree;
} integral;

double f(double x) {
	return pow(x, 5) - 3.2 * pow(x, 3) + 1.5 * pow(x, 2) - 7 * x;
}

integral square(int n, double a, double b, double prev, double delta, int counter) {
	integral result;
	result.value = 0;
	double h = (b-a)/n;
	for (int i = 0; i < n; i++) {
		result.value += f(a + h/2 + h * i);
	}
	result.value *= h;
	if (fabs(result.value - prev) < delta) {
		result.degree = counter;
		return result;
	}
	else return square(2 * n, a, b, result.value, delta, counter+1);
}


int main() {
	int a = -2;
	int b = 0;
	integral output;

	FILE* f = fopen("output.txt", "w");

	//printf("%lf", square(1, a, b, 0, 0.5));
	for (int i = 3; i < 12; i++) {
		output = square(1, a, b, 0, pow(10, -i), 1);
		fprintf(f, "%.20lf %d %.20lf %.20lf\n", output.value-S, output.degree,(double)(b-a)/pow(2,output.degree)*2, pow(10,-i));
	}
	fclose(f);
}