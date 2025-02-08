#include <float.h>
#include <math.h>
#include <stdio.h>

double f(double x)
{
    if (x <= 0)
        return NAN;
    return 4.0 * (1.0 + sqrt(x)) * log(x) - 1.0;
}

double simple_iter(double a, double b)
{
    if (b <= 0) {
        return NAN;
    }

    double eps = 1e-14;
    if (a <= 0)
        a = 0 + eps;

    double fa = f(a);
    double fb = f(b);
    if (isnan(fa) || isnan(fb)) {
        return NAN;
    }

    if (fa * fb > 0) {
        return NAN;
    }

    int max_iter = 100000;

    double x = (a + b) * 0.5;
    for (int i = 0; i < max_iter; i++) {
        if (x <= 0) {
            x = (a + b) * 0.5;
        }
        double next_x = exp(1.0 / (4.0 * (1.0 + sqrt(x))));
        if (fabs(next_x - x) < eps) {
            x = next_x;
            break;
        }
        x = next_x;
    }

    if (x < a || x > b) {
        return NAN;
    }

    double fv = f(x);
    if (fabs(fv) > 1e-6) {
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

    double res = simple_iter(a, b);
    if (isnan(res)) {
        printf("no root\n");
    } else {
        printf("%.4f\n", res);
    }

    return 0;
}
