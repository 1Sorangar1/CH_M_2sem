#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define N 10
#define P 10000

double f1(double x) {
	return sqrt(x) - cos(x);
}

double f2(double x) {

	if (x > 0) { 
		return pow(x, 4) - 18 * pow(x, 2) + 2;
	}
	else {
		return  (-1) * pow(x, 4) - 18 * pow(x, 2) + 2;
	}
}


double** differences_building(double* x, double* y, int n) {
	double** res = (double**)malloc(n * sizeof(double));
	for (int i = 0; i < n; i++) {
		res[i] = (double*)malloc((n - i) * sizeof(double));
	}

	for (int i = 0; i < n; i++) {
		res[i][0] = y[i];
	}

	for (int k = 1; k < n; k++) {
		for (int i = 0; i < n - k; i++) {
			res[i][k] = (res[i + 1][k - 1] - res[i][k - 1]) / (x[k + i] - x[i]);
		}
	}
	//printf("differences_result\n");
	//for (int i = 0; i < n; i++) {
	//	for (int j = 0; j < n - i; j++) {
	//		printf("%lf ", res[i][j]);
	//	}
	//	printf("\n");
	//}
	//printf("\n");

	return res;

}

double* table_h(double start, double finish, int n) {
	double step = (finish - start) / (n - 1);
	double* res = (double*)malloc(n * sizeof(double));
	for (int i = 0; i < n; i++) {
		res[i] = start + i * step;
	}

	return res;

}

double* table_chebish(double start, double finish, int n) {
	double* res = (double*)malloc(n * sizeof(double));
	for (int i = 0; i < n; i++) {
		res[n - i - 1] = (start + finish) * 0.5 + (finish - start) * 0.5 * cos((2 * i + 1) * M_PI / (2 * n));
	}
	return res;
}



double Newton_method_rtl(double const x, double const* x_list, double const** differences, int const n) {
	double res = differences[n - 1][0], current_x = 1;

	for (int i = 1; i < n; i++) {
		current_x *= (x - x_list[n - i]);
		res += differences[n - 1 - i][i] * current_x;
	}

	return res;
}


void Newton_cycle_mistake(double x_start_1, double x_finish_1, double x_start_2, double x_finish_2, int cycle_start, int cycle_finish, double current_x, int check) {

	//Максимальная ошибка 
	FILE* f_1 = fopen("output1.txt", "w");
	FILE* f_2 = fopen("output2.txt", "w");

	double m1 = 0, m2 = 0, m3 = 0, m4 = 0;
	double c1, c2, c3, c4;

	double f1_current_x = f1(current_x);
	double f2_current_x = f2(current_x);

	//установка проверочной сетки

	double* x_check_1 = table_h(x_start_1, x_finish_1, check);
	double* x_check_2 = table_h(x_start_2, x_finish_2, check);

	double* y_check_1 = (double*)malloc(check * sizeof(double));
	double* y_check_2 = (double*)malloc(check * sizeof(double));

	for (int i = 0; i < check; i++) {
		y_check_1[i] = f1(x_check_1[i]);
		y_check_2[i] = f2(x_check_2[i]);
	}



	//цикл с перебором велечин по n

	for (int j = cycle_start; j <= cycle_finish; j++) {

		//Установка сетки
		double* x1h = table_h(x_start_1, x_finish_1, j);
		double* x1chebish = table_chebish(x_start_1, x_finish_1, j);
		double* x2h = table_h(x_start_2, x_finish_2, j);
		double* x2chebish = table_chebish(x_start_2, x_finish_2, j);

		double* y1h = (double*)malloc(j * sizeof(double));
		double* y1chebish = (double*)malloc(j * sizeof(double));
		double* y2h = (double*)malloc(j * sizeof(double));
		double* y2chebish = (double*)malloc(j * sizeof(double));

		//заполнение сетки
		for (int i = 0; i < j; i++) {
			y1h[i] = f1(x1h[i]);
		}

		for (int i = 0; i < j; i++) {
			y1chebish[i] = f1(x1chebish[i]);
		}

		for (int i = 0; i < j; i++) {
			y2h[i] = f2(x2h[i]);
		}

		for (int i = 0; i < j; i++) {
			y2chebish[i] = f2(x2chebish[i]);
		}

		double** differencesh1 = differences_building(x1h, y1h, j);
		double** differencesh2 = differences_building(x2h, y2h, j);
		double** differencesChebish1 = differences_building(x1chebish, y1chebish, j);
		double** differencesChebish2 = differences_building(x2chebish, y2chebish, j);

		//макс ошибка

		for (int i = 0; i < check; i++) {
			c1 = Newton_method_rtl(x_check_1[i], x1h, differencesh1, j) - y_check_1[i];
			if (c1 > m1) {
				m1 = c1;
			}
			else if (-c1 > m1) {
				m1 = -c1;
			}
		}

		for (int i = 0; i < check; i++) {
			c2 = Newton_method_rtl(x_check_2[i], x2h, differencesh2, j) - y_check_2[i];
			if (c2 > m2) {
				m2 = c2;
			}
			else if (-c2 > m2) {
				m2 = -c2;
			}
		}
		for (int i = 0; i < check; i++) {
			c3 = Newton_method_rtl(x_check_1[i], x1chebish, differencesChebish1, j) - y_check_1[i];
			if (c3 > m3) {
				m3 = c3;
			}
			else if (-c3 > m3) {
				m3 = -c3;
			}
		}
		for (int i = 0; i < check; i++) {
			c4 = Newton_method_rtl(x_check_2[i], x2chebish, differencesChebish2, j) - y_check_2[i];
			if (c4 > m4) {
				m4 = c4;
			}
			else if (-c4 > m4) {
				m4 = -c4;
			}
		}

		//ошибка в точке

		double current_x_mistake_h1 = Newton_method_rtl(current_x, x1h, differencesh1, j) - f1_current_x;
		if (current_x_mistake_h1 < 0) {
			current_x_mistake_h1 = -current_x_mistake_h1;
		}
		double current_x_mistake_h2 = Newton_method_rtl(current_x, x2h, differencesh2, j) - f2_current_x;
		if (current_x_mistake_h2 < 0) {
			current_x_mistake_h2 = -current_x_mistake_h2;
		}
		double current_x_mistake_c1 = Newton_method_rtl(current_x, x1chebish, differencesChebish1, j) - f1_current_x;
		if (current_x_mistake_c1 < 0) {
			current_x_mistake_c1 = -current_x_mistake_c1;
		}
		double current_x_mistake_c2 = Newton_method_rtl(current_x, x2chebish, differencesChebish2, j) - f2_current_x;
		if (current_x_mistake_c2 < 0) {
			current_x_mistake_c2 = -current_x_mistake_c2;
		}

		//запись + чистка

		fprintf(f_2, "%.20lf %.20lf %.20lf %.20lf\n", current_x_mistake_h1, current_x_mistake_h2, current_x_mistake_c1, current_x_mistake_c2);

		free(x1h);
		free(x2h);
		free(x1chebish);
		free(x2chebish);
		free(y1h);
		free(y2h);
		free(y1chebish);
		free(y2chebish);
		free(differencesh1);
		free(differencesh2);
		free(differencesChebish1);
		free(differencesChebish2);

		fprintf(f_1, "%.20lf %.20lf %.20lf %.20lf\n", m1, m2, m3, m4);

		m1 = 0;
		m2 = 0;
		m3 = 0;
		m4 = 0;
	}

	fclose(f_1);
	fclose(f_2);

}



int main() {
	//задача необходимых переменных
	double x_start_1 = 1;
	double x_finish_1 = 6;
	double x_start_2 = -2;
	double x_finish_2 = 2;

	//Установка сетки
	double* x1h = table_h(x_start_1, x_finish_1, N);
	double* x1chebish = table_chebish(x_start_1, x_finish_1, N);
	double* x2h = table_h(x_start_2, x_finish_2, N);
	double* x2chebish = table_chebish(x_start_2, x_finish_2, N);

	double* y1h = (double*)malloc(N * sizeof(double));
	double* y1chebish = (double*)malloc(N * sizeof(double));
	double* y2h = (double*)malloc(N * sizeof(double));
	double* y2chebish = (double*)malloc(N * sizeof(double));

	double* N1h = (double*)malloc(P * sizeof(double));
	double* N1chebish = (double*)malloc(P * sizeof(double));
	double* N2h = (double*)malloc(P * sizeof(double));
	double* N2chebish = (double*)malloc(P * sizeof(double));

	//заполнение сетки
	for (int i = 0; i < N; i++) {
		y1h[i] = f1(x1h[i]);
	}

	for (int i = 0; i < N; i++) {
		y1chebish[i] = f1(x1chebish[i]);
	}

	for (int i = 0; i < N; i++) {
		y2h[i] = f2(x2h[i]);
	}

	for (int i = 0; i < N; i++) {
		y2chebish[i] = f2(x2chebish[i]);
	}

	//установка проверочной сетки

	double* x_check_1 = table_h(x_start_1, x_finish_1, P);
	double* x_check_2 = table_h(x_start_2, x_finish_2, P);

	double* y_check_1 = (double*)malloc(P * sizeof(double));
	double* y_check_2 = (double*)malloc(P * sizeof(double));

	for (int i = 0; i < P; i++) {
		y_check_1[i] = f1(x_check_1[i]);
		y_check_2[i] = f2(x_check_2[i]);
	}

	//сетка через метод Ньютона
	double** differencesh1 = differences_building(x1h, y1h, N);
	double** differencesh2 = differences_building(x2h, y2h, N);
	double** differencesChebish1 = differences_building(x1chebish, y1chebish, N);
	double** differencesChebish2 = differences_building(x2chebish, y2chebish, N);



	for (int i = 0; i < P; i++) {
		N1h[i] = Newton_method_rtl(x_check_1[i], x1h, differencesh1, N);
		N2h[i] = Newton_method_rtl(x_check_2[i], x2h, differencesh2, N);
		N1chebish[i] = Newton_method_rtl(x_check_1[i], x1chebish, differencesChebish1, N);
		N2chebish[i] = Newton_method_rtl(x_check_2[i], x2chebish, differencesChebish2, N);
	}


	//занесение сетки в файл
	FILE* f = fopen("output.txt", "w");
	for (int i = 0; i < N; i++) {

		fprintf(f, "%.20lf %.20lf %.20lf %.20lf %.20lf %.20lf %.20lf %.20lf\n", x1h[i], y1h[i], x1chebish[i], y1chebish[i], x2h[i], y2h[i], x2chebish[i], y2chebish[i]);
	}
	fclose(f);

	FILE* f2 = fopen("outputPoints.txt", "w");

	for (int i = 0; i < P; i++) {
		fprintf(f2, "%.20lf %.20lf %.20lf %.20lf %.20lf %.20lf %.20lf %.20lf\n", x_check_1[i], y_check_1[i], N1h[i], N1chebish[i], x_check_2[i], y_check_2[i], N2h[i], N2chebish[i]);
	}
	fclose(f2);




	free(x1h);
	free(x2h);
	free(x1chebish);
	free(x2chebish);
	free(y1h);
	free(y2h);
	free(y1chebish);
	free(y2chebish);
	free(N1h);
	free(N2h);
	free(N1chebish);
	free(N2chebish);
	free(differencesh1);
	free(differencesh2);
	free(differencesChebish1);
	free(differencesChebish2);

	Newton_cycle_mistake(x_start_1, x_finish_1, x_start_2, x_finish_2, 5, 100, 1.0024, P);

	return 0;
}



#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define E 2.718281828459045

typedef struct {
	double y;
	double h;
	double R;
} current;


double f(double x, double y) {
	return (-y * y * (log(x) + 2) * log(x)) / x;
	//return 2 * x * (x * x + y);
	//return x * x - sin(2 * x);
}


double F(double x) {
	return x / (log(x) * log(x) * x + 1);
	//return (E + 2) * pow(E, pow(x, 2) - 1) - x * x - 1;
	//return x * x * x / 3 + cos(2 * x) / 2+(double)3/2;
}


double KuttaMersonMethodConstantStep(double x, double y, double h) {
	double k1, k2, k3, k4, k5, ynext, R;
	k1 = f(x, y);
	k2 = f(x + h / 3, y + h * k1 / 3);
	k3 = f(x + h / 3, y + h * (k1 + k2) / 6);
	k4 = f(x + h / 2, y + h * (k1 + 3 * k3) / 8);
	k5 = f(x + h, y + h * ((k1 - 3 * k3) / 2 + 2 * k4));
	ynext = y + h * (k1 + 4.0 * k4 + k5) / 6;

	return ynext;
}


current KuttaMersonMethod(double x, double y, double h, double delta) {
	double k1, k2, k3, k4, k5, ynext, R;
	current result;
	k1 = h * f(x, y);
	k2 = h * f(x + h / 3.0, y + k1 / 3);
	k3 = h * f(x + h / 3.0, y + (k1 + k2) / 6);
	k4 = h * f(x + h / 2.0, y + (k1 + 3 * k3) / 8);
	k5 = h * f(x + h, y + (k1 - 3 * k3) / 2 + 2 * k4);
	ynext = y + k1 / 6 + 2 / 3 * k4 + k5 / 6;

	R = (2 * k1 - 9 * k3 + 8 * k4 - k5) / 30;
	if (fabs(R) >= delta * fabs(y)) return KuttaMersonMethod(x, y, h / 2, delta);
	result.h = h;
	result.y = ynext;
	result.R = R;
	if (32 * fabs(R) <= delta * fabs(y)) {
		result.h = 2 * h;
	}
	return result;
}



int main() {
	double a = 1;
	double b = 3;
	double i = a;
	double y = 1;
	double h1 = 0.1;
	double h2 = 0.2;
	double h3 = 0.1;
	double h4 = 0.1;
	double y0 = y;
	double ResultMethod, ResultAccurate, Mistake;
	ResultMethod = y0;

	current result;
	double delta = pow(10, -5);

	FILE* f1 = fopen("output1.txt", "w");
	FILE* f2 = fopen("output2.txt", "w");
	FILE* f3 = fopen("output3.txt", "w");
	FILE* f4 = fopen("output4.txt", "w");


	ResultAccurate = F(i);
	fprintf(f1, "%lf %.20lf %.20lf %.20lf\n", i, ResultMethod, ResultAccurate, fabs(ResultAccurate - ResultMethod));
	//printf("%lf %.20lf %.20lf\n", i, ResultMethod, ResultAccurate);
	do {
		ResultMethod = KuttaMersonMethodConstantStep(i, y0, h1);
		i += h1;
		ResultAccurate = F(i);
		fprintf(f1, "%lf %.20lf %.20lf %.20lf\n", i, ResultMethod, ResultAccurate, fabs(ResultAccurate - ResultMethod));
		//printf("%lf %.20lf %.20lf\n", i, ResultMethod, ResultAccurate);
		y0 = ResultMethod;

	} while (i <= b);

	i = 0;
	y0 = y;
	ResultMethod = y0;

	ResultAccurate = F(i);
	fprintf(f2, "%lf %.20lf %.20lf %.20lf\n", i, ResultMethod, ResultAccurate, fabs(ResultAccurate - ResultMethod));
	//printf("%lf %.20lf %.20lf\n", i, ResultMethod, ResultAccurate);
	do {
		ResultMethod = KuttaMersonMethodConstantStep(i, y0, h2);
		i += h2;
		ResultAccurate = F(i);
		fprintf(f2, "%lf %.20lf %.20lf %.20lf\n", i, ResultMethod, ResultAccurate, fabs(ResultAccurate - ResultMethod));
		//printf("%lf %.20lf %.20lf\n", i, ResultMethod, ResultAccurate);
		y0 = ResultMethod;

	} while (i <= b);

	i = a;
	y0 = y;
	ResultMethod = y0;

	fprintf(f3, "%.20lf %.20lf\n", i, h3);
	//printf("%.20lf %.20lf %.20lf %.20lf\n", i, h3, y0, F(i));
	do {
		result = KuttaMersonMethod(i, y0, h3, delta);
		fprintf(f3, "%.10lf %.10lf\n", i, h3);
		//printf("%.20lf %.20lf %.20lf %.20lf\n", i, h3, result.y, F(i));
		y0 = result.y;
		i += h3;
		h3 = result.h;
	} while (i <= b);

	i = a;
	y0 = y;

	for (int j = 1; j <= 10; j++) {
		do {
			result = KuttaMersonMethod(i, y0, h3, pow(10, -j));
			//printf("%.20lf %.20lf %.20lf %.20lf\n", i, h3, result.y, F(i));
			y0 = result.y;
			i += h3;
			h3 = result.h;

		} while (i <= b);
		fprintf(f4, "%.20lf %.20lf\n", pow(10, -j), fabs(result.R));
		i = a;
		y0 = y;
	}


	fclose(f1);
	fclose(f2);
	fclose(f3);
	fclose(f4);

	return 0;
}



