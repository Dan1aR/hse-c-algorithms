#include <float.h>
#include <math.h>
#include <stdio.h>

double f(double x)
{
    if (x == 0.0)
        return NAN;
    return sin(x) - 1.0 / x;
}

double newton(double a, double b)
{
    double fa = f(a);
    double fb = f(b);

    if (isnan(fa) || isnan(fb)) {
        return NAN;
    }

    if (fa * fb > 0) {
        return NAN;
    }

    double x = (a + b) * 0.5;
    double eps = 1e-14;
    int max_iter = 1000;
    double lr = 1e-6;
    int found = 0;

    for (int iter = 0; iter < max_iter; iter++) {
        double fx = f(x);
        double fpx = (f(x + lr) - f(x - lr)) / (2.0 * lr);
        double x_new = x - fx / fpx;
        if (fabs(x_new - x) < eps) {
            x = x_new;
            found = 1;
            break;
        }
        x = x_new;
    }

    if (!found) {
        return NAN;
    }

    double fx_final = f(x);
    if (fabs(fx_final) > lr) {
        return NAN;
    }

    return x;
}

int main()
{
    double a, b;
    if (scanf("%lf %lf", &a, &b) != 2) {
        return 1;
    }

    double res = newton(a, b);
    if (isnan(res)) {
        printf("no root\n");
    } else {
        printf("%.4f\n", res);
    }

    return 0;
}
