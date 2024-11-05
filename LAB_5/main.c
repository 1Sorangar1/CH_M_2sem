#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
	return 3 / ((pow(log(x), 3)) + 3 * pow(log(x), 2) + 3);
	//return x / (log(x) * log(x) * x + 1);
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
	k1 = f(x, y);
	k2 = f(x + h / 3, y + h * k1 / 3);
	k3 = f(x + h / 3, y + h * (k1 + k2) / 6);
	k4 = f(x + h / 2, y + h * (k1 + 3 * k3) / 8);
	k5 = f(x + h, y + h * ((k1 - 3 * k3) / 2 + 2 * k4));
	ynext = y + h * (k1 + 4.0 * k4 + k5) / 6;

	R = (2 * k1 - 9 * k3 + 8 * k4 - k5)/30;
	if (fabs(R) >= delta * fabs(y)) return KuttaMersonMethod(x, y, h / 2, delta);
	result.h = h;
	result.y = ynext;
	result.R = R;
	if (32 * fabs(R) <= delta * fabs(y)) {
		result.h = 2 * h;
	}
	return result;
}

void data() {
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
	double delta = pow(10, -7);

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

	i = a;
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

	for (int j = 3; j <= 13; j++) {
		do {		
			result = KuttaMersonMethod(i, y0, h3, pow(10, -j));
			//printf("%.20lf %.20lf %.20lf %.20lf\n", i, h3, result.y, F(i));
			y0 = result.y;
			i += h3;
			h3 = result.h;

		} while (i <= b);
		//printf("%d %lf %lf\n", j, i, h3);
		fprintf(f4, "%.30lf %.30lf\n", pow(10, -j), fabs(result.R));
		i = a;
		y0 = y;
	}


	fclose(f1);
	fclose(f2);
	fclose(f3);
	fclose(f4);
}



int main() {
	data();

	return 0;
}




//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <math.h>
//#include <stdlib.h>
//
//#define E 2.718281828459045
//
//double f(double x, double y) {
//    return (-y * y * (log(x) + 2) * log(x)) / x;
//}
//
//double Integral(double x) {
//    return 3 / ((pow(log(x), 3)) + 3 * pow(log(x), 2) + 3);
//}
//
//double kutta_merson(double (*f)(double, double), double x, double y, double h, double k1, double* R) {
//    double k2;
//    double k3;
//    double k4;
//    double k5;
//    double m = 0;
//    k2 = f(x + h / 3.0, y + h * k1 / 3.0);
//    k3 = f(x + h / 3.0, y + h * (k1 + k2) / 6.0);
//    k4 = f(x + h / 2.0, y + h * (k1 + 3.0 * k3) / 8.0);
//    k5 = f(x + h, y + h * (k1 - 3.0 * k3 + 2.0 * k4) / 2.0);
//    *R = fabs(2 * k1 - 9 * k3 + 8 * k4 - k5) * h / 30;
//    err = fabs(y-Integral(x));
//    fprintf(F,"%lf %.20lf %.20lf\n", x, y,err);
//    return h * (k1 + 4.0 * k4 + k5) / 6.0;
//}
//
//void Tests()
//{
//    double x0 = 1.0;
//    double y0 = 1;
//    double x1 = 3.0;
//    double h1 = 0.1;
//    double h2 = 0.25;
//    double h3 = 1;
//    double h4 = 0.0625;
//    double eps;
//    double err;
//    FILE* F = fopen("data1.txt", "w");
//    FILE* F1 = fopen("data2.txt", "w");
//    FILE* F2 = fopen("data4.txt", "w");
//    FILE* F3 = fopen("data3.txt", "w");
//
//    double x = x0;
//    double y = y0;
//    double k1 = f(x, y);
//    double r = 0.0;
//    double* R = &r;
//    do {
//        k1 = f(x, y);
//        err = fabs(y - Integral(x));
//        fprintf(F, "%lf %.20lf %.20lf\n", x, y, err);
//        y = y + kutta_merson(f, x, y, h1, k1, R);
//        x += h1;
//    } while (x <= x1);
//    err = fabs(y - Integral(x));
//    fprintf(F, "%lf %.20lf %.20lf\n", x, y, err);
//
//    x = x0;
//    y = y0;
//
//    do {
//        /* double k1 = f(x, y);
//         double r = 0.0;
//         double* R = &r;*/
//        k1 = f(x, y);
//        err = fabs(y - Integral(x));
//        fprintf(F1, "%lf %.20lf %.20lf\n", x, y, err);
//        y += kutta_merson(f, x, y, h2, k1, R);
//        x += h2;
//    } while (x <= x1);
//    fprintf(F1, "%lf %.20lf %.20lf\n", x, y, err);
//
//    x = x0;
//    y = y0;
//    for (eps = 1; eps >= 1e-13; eps = eps / 10) {
//
//        /*double k1 = f(x, y);
//        double r = 0.0;
//        double* R=&r;*/
//        R = 0.0;
//
//        double res;
//        do {
//            k1 = f(x, y);
//            res = kutta_merson(f, x, y, h3, k1, R);
//            if (*R > eps)
//            {
//                h3 /= 2;
//            }
//        } while (*R > eps);
//        y += res;
//        x += h3;
//
//        if (*R < eps / 64) {
//            h3 *= 2;
//        }
//
//        fprintf(F2, "%.20lf %.20lf %.20lf\n", eps, h3, *R);
//    }
//    eps = 0.01;
//    double res;
//    x = x0;
//    y = y0;
//    do {
//
//
//        k1 = f(x, y);
//        fprintf(F3, "%.20lf %.20lf %.20lf\n", x, y, h4);
//        do {
//
//            res = kutta_merson(f, x, y, h4, k1, R);
//            if (*R > eps)
//            {
//                h4 /= 2;
//            }
//        } while (*R > eps);
//        y += res;
//        x += h4;
//
//        if (*R < eps / 64) {
//            h4 *= 2;
//        }
//
//    } while (x <= x1);
//}
//
//int main() {
//
//    Tests();
//    return 0;
//}