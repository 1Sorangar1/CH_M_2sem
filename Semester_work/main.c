#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define S 20.13333333333333333333

double f(double x) {
    return pow(x, 5) - 3.2 * pow(x, 3) + 1.5 * pow(x, 2) - 7 * x;
}

typedef struct {
    double x;
    double fx;
} Node;

typedef struct {
    Node* cache;
    int cache_size;
    int function_calls;
} lobatto_data;

double get_from_cache(Node* cache, int cache_size, double x) {
    for (int i = 0; i < cache_size; i++) {
        if (fabs(cache[i].x - x) < 1e-9) {
            return cache[i].fx;
        }
    }
    return NAN;
}

void add_to_cache(Node** cache, int* cache_size, double x, double fx) {
    *cache = realloc(*cache, (*cache_size + 1) * sizeof(Node));
    if (*cache == NULL) {
        fprintf(stderr, "Memory error\n");
        exit(1);
    }
    (*cache)[*cache_size].x = x;
    (*cache)[*cache_size].fx = fx;
    (*cache_size)++;
}

double cached_f(lobatto_data* data, double x) {
    double fx = get_from_cache(data->cache, data->cache_size, x);
    if (!isnan(fx)) {
        return fx;
    }
    data->function_calls++;
    fx = f(x);
    add_to_cache(&data->cache, &data->cache_size, x, fx);
    return fx;
}

double lobatto_4(lobatto_data* data, double a, double b) {
    double x0 = a;
    double x3 = b;
    double x1 = (a + b) / 2 - (b - a) / (2 * sqrt(3));
    double x2 = (a + b) / 2 + (b - a) / (2 * sqrt(3));

    double w0 = 1.0 / 6;
    double w1 = 5.0 / 6;

    return (b - a) / 2 * (w0 * cached_f(data, x0) + w1 * cached_f(data, x1) + w1 * cached_f(data, x2) + w0 * cached_f(data, x3));
}

double lobatto_5(lobatto_data* data, double a, double b) {
    double x0 = a;
    double x4 = b;
    double x1 = (a + b) / 2 - (b - a) * sqrt(3.0 / 7) / 2;
    double x3 = (a + b) / 2 + (b - a) * sqrt(3.0 / 7) / 2;
    double x2 = (a + b) / 2;

    double w0 = 1.0 / 10;
    double w1 = 49.0 / 90;
    double w2 = 32.0 / 45;

    return (b - a) / 2 * (w0 * cached_f(data, x0) + w1 * cached_f(data, x1) + w2 * cached_f(data, x2) + w1 * cached_f(data, x3) + w0 * cached_f(data, x4));
}

double adaptive_lobatto(lobatto_data* data, double a, double b, double delta) {
    double I4 = lobatto_4(data, a, b);
    double I5 = lobatto_5(data, a, b);
    //printf("%lf %.20lf %.20lf %.20lf\n", delta, fabs(I5 - I4), a, b);
    if (fabs(I5 - I4) < delta) {
        //printf("!!! %.20lf\n", I5);
        return I5;
    } else {
        double mid = (a + b) / 2;
        return adaptive_lobatto(data, a, mid, delta/2) + adaptive_lobatto(data, mid, b, delta/2);
    }
}

typedef struct {
    double value;
    int degree;
} integral;

integral square(int n, double a, double b, double prev, double delta, int counter) {
    integral result;
    result.value = 0;
    double h = (b - a) / n;
    for (int i = 0; i < n; i++) {
        result.value += f(a + h / 2 + h * i);
    }
    result.value *= h;
    if (fabs(result.value - prev) < (delta / counter / 2)) {
        result.degree = counter;
        return result;
    } else {
        return square(2 * n, a, b, result.value, delta, counter + 1);
    }
}

int main() {
    double a = -2;
    double b = 0;

    FILE* error_delta = fopen("error_delta.txt", "w");
    FILE* iter_delta = fopen("iter_delta.txt", "w");
    FILE* error_iter = fopen("error_iter.txt", "w");

    double delta, result_lobatto;
    int function_calls_square;
    integral result_square;
    for (int i = 1; i < 7; i++) {
        delta = pow(10, -i);
        
        lobatto_data data = { NULL, 0, 0 };
        result_lobatto = adaptive_lobatto(&data, a, b, delta);

        result_square = square(1, a, b, 0, pow(10, -i), 1);
        function_calls_square = pow(2, result_square.degree) - 1;

        fprintf(error_delta, "%.10lf %.20lf %.20lf\n", delta, fabs(S - result_square.value), fabs(S - result_lobatto));
        fprintf(iter_delta, "%.15lf %d %d\n", delta, function_calls_square, data.function_calls);
        fprintf(error_iter, "%d %.20lf %d %.20lf\n", function_calls_square, fabs(S - result_square.value), data.function_calls, fabs(S - result_lobatto));

        free(data.cache);
    }

    fclose(error_delta);
    fclose(iter_delta);
    fclose(error_iter);

    return 0;
}
