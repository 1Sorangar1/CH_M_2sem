#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>

typedef struct {
	double y1;
	double y2;
}vector;

double f(double x) {
	return 1 - cos(x);
}

double p(double x) {
	return -sin(x);
}
double q(double x) {
	return cos(x);
}
double F_correct(double x) {
	return cos(x);
}

double F(double x, vector y) {
	return f(x) - p(x) * y.y2 - q(x) * y.y1;
}

double F_0(double x, vector y) {
	return -p(x) * y.y2 - q(x) * y.y1;
}

vector F_vect(double x, vector y) {
	vector ans = { y.y2,F(x,y) };
	return ans;
}
vector F_vect_0(double x, vector y) {
	vector ans = { y.y2,F_0(x,y) };
	return ans;
}

vector* find_initials(double A, double alpha_0, double alpha_1) {
	vector u = { alpha_0 * A / (pow(alpha_0,2) + pow(alpha_1,2)),alpha_1 * A / (pow(alpha_0,2) + pow(alpha_1,2)) };
	vector v = { alpha_1,-alpha_0 };

	vector* ans = malloc(sizeof(vector) * 2);
	ans[0] = u;
	ans[1] = v;
	return ans;
}


vector* euler_method(vector y0, double a, double b, vector(*F)(double, vector), int n) {
	double h = (b - a) / (n - 1);
	vector* y = malloc(sizeof(vector) * n);
	if (y == NULL) {
		exit(1);
	}
	y[0] = y0;

	for (int i = 0; i < n - 1; i++) {
		vector res = F(a + i * h, y[i]);
		y[i + 1].y1 = y[i].y1 + h * res.y1;
		y[i + 1].y2 = y[i].y2 + h * res.y2;
	}
	return y;
}

double* solve(vector* initials, double betta_0, double betta_1, double B, double a, double b, vector(*F)(double, vector), vector(*F_0)(double, vector), int n) {
	vector u0 = initials[0];
	vector v0 = initials[1];

	vector* u = euler_method(u0, a, b, F, n);
	vector* v = euler_method(v0, a, b, F_0, n);

	double* y = malloc(sizeof(double) * n);
	double C = (B - betta_0 * u[n - 1].y1 - betta_1 * u[n - 1].y2) / (betta_0 * v[n - 1].y1 + betta_1 * v[n - 1].y2);


	for (int i = 0; i < n; i++) {


		y[i] = u[i].y1 + C * v[i].y1;

	}
	return y;
}


double** solve_runge(vector* initials, double betta_0, double betta_1, double B, double a, double b, vector(*F)(double, vector), vector(*F_0)(double, vector), double eps, int* n_, int* len_h) {

	double* x = malloc(sizeof(double));
	long double* h_all = malloc(sizeof(long double));
	long double* h_made = malloc(sizeof(long double));



	long double h = (b - a) / 2;
	x[0] = a;
	h_all[0] = h;

	vector res;
	double c1, c2;


	vector* u1 = malloc(sizeof(vector) * 2), * v1 = malloc(sizeof(vector) * 2), u2, v2;
	u1[0] = initials[0];
	v1[0] = initials[1];
	u2 = initials[0];
	v2 = initials[1];


	int i = 1, k = 1, n = 10000;

	vector* u = malloc(sizeof(vector));
	vector* v = malloc(sizeof(vector));
	u[0] = initials[0];
	v[0] = initials[1];
	eps = pow(eps, 3.0 / 2);
	double difu, difv, dif;
	while (x[i - 1] + h <= b) {
		h_all = realloc(h_all, sizeof(long double) * (k + 1));
		h_all[k] = h;

		u1[1] = euler_method(u1[0], x[i - 1], x[i - 1] + h, F, 3)[2];
		v1[1] = euler_method(v1[0], x[i - 1], x[i - 1] + h, F_0, 3)[2];

		u2 = euler_method(u1[0], x[i - 1], x[i - 1] + h, F, 5)[4];
		v2 = euler_method(v1[0], x[i - 1], x[i - 1] + h, F_0, 5)[4];

		difu = fabs(u1[1].y1 - u2.y1);
		difv = fabs(v1[1].y1 - v2.y1);

		//printf("difv = %e, v1[1].y1 = %e, v2.y1 = %e, i = %i,h = %e\n", difv, v1[1].y1, v2.y1,i,h);

		if (difu > eps / 100 || difv > eps / 100) {
			h /= 2;

		}
		else if (difu < eps / 500 && difv < eps / 500) {
			x = realloc(x, sizeof(double) * (i + 1));
			x[i] = x[i - 1] + h;
			u = realloc(u, sizeof(vector) * (i + 1));
			u[i] = u2;
			v = realloc(v, sizeof(vector) * (i + 1));
			v[i] = v2;
			h_made = realloc(h_made, sizeof(long double) * (i + 1));
			h_made[i - 1] = h;
			i += 1;
			u1[0] = u2;
			v1[0] = v2;
			if (x[i] + h * 2 <= b) {
				h *= 2;
			}
		}
		else {
			x = realloc(x, sizeof(double) * (i + 1));
			x[i] = x[i - 1] + h;
			u = realloc(u, sizeof(vector) * (i + 1));
			u[i] = u2;
			v = realloc(v, sizeof(vector) * (i + 1));
			v[i] = v2;
			h_made = realloc(h_made, sizeof(long double) * (i + 1));
			h_made[i - 1] = h;
			i += 1;
			u1[0] = u2;
			v1[0] = v2;
		}
		k++;
	}
	h_all[k - 1] = h;
	h_made[i - 1] = h;
	*len_h = k;
	*n_ = i;

	double C = (B - betta_0 * u[i - 1].y1 - betta_1 * u[i - 1].y2) / (betta_0 * v[i - 1].y1 + betta_1 * v[i - 1].y2);
	double* y = malloc(sizeof(double) * i);
	for (int l = 0; l < i; l++) {
		y[l] = u[l].y1 + C * v[l].y1;
	}


	double** ans = malloc(sizeof(double*) * 4);
	ans[0] = x;
	ans[1] = y;
	ans[2] = h_all;
	ans[3] = h_made;

	return ans;
}




int main() {
	double A = 1, B = -1, alpha_0 = 1, alpha_1 = 0, betta_0 = 1, betta_1 = 1, a = 0, b = M_PI_2;

	FILE* file;
	vector* initials = find_initials(A, alpha_0, alpha_1);
	char path[100];

	//task1
	int n_list[] = {3};
	for (int i = 0; i < 1; i++) {
		int n = n_list[i];
		double h = (b - a) / (n - 1);
		double* task1 = solve(initials, betta_0, betta_1, B, a, b, F_vect, F_vect_0, n);
		sprintf(path, "%i.txt", n);
		file = fopen(path, "w+");
		for (int i = 0; i < n; i++) {
			fprintf(file, "%.20lf %.20lf %.20lf %.20lf", a + i * h, task1[i], fabs(F_correct(a + i * h) - task1[i]), F_correct(a + i * h));
			if (i != n - 1) {
				fprintf(file, "\n");
			}
		}
		fclose(file);
		free(task1);
	}



	//task2
	double eps;
	int n_ = 0, len_h = 0;
	double** ans;

	FILE* file_err, * file_h;
	sprintf(path, "runge_err.txt");
	file_err = fopen(path, "w+");
	sprintf(path, "runge_h.txt");
	file_h = fopen(path, "w+");

	for (int power = 0; power <= 5; power++) {
		eps = pow(10, -power);
		double max_h = 1000, max_err = 0, current_err = 0, current_h = 1000;
		ans = solve_runge(initials, betta_0, betta_1, B, a, b, F_vect, F_vect_0, eps, &n_, &len_h);

		for (int i = 0; i < n_; i++) {
			current_err = fabs(F_correct(ans[0][i]) - ans[1][i]);
			if (current_err > max_err) max_err = current_err;

		}
		fprintf(file_err, "%.20lf %.10lf\n", max_err, eps);

		for (int k = 0; k < len_h; k++) {
			current_h = ans[2][k];
			if (current_h < max_h) max_h = current_h;
			if (max_h == 0) printf("%d %d\n", power, k);
		}
		fprintf(file_h, "%.20lf %.10lf\n", max_h, eps);
	}


	free(ans[0]);
	free(ans[1]);
	free(ans[2]);
	free(ans[3]);
	free(ans);
	fclose(file_err);
	fclose(file_h);

	return 0;
}