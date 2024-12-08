#include <math.h>
#include <stdio.h>
#include <stdlib.h>

double f(double x, double y)
{
    return 3 * x * x + x * y + 2 * y * y - x - 4 * y;
}

double dfdx(double x, double y)
{
    return 6 * x + y - 1;
}

double dfdy(double x, double y)
{
    return x + 4 * y - 4;
}

void grad_descent(double* x0, double* y0)
{
    double lr = 1e-3;
    double eps = 1e-9;

    int max_iter = 10000;

    double f0 = f(*x0, *y0);
    for (int i = 0; i < max_iter; ++i) {
        double grad_x = dfdx(*x0, *y0);
        double grad_y = dfdy(*x0, *y0);

        *x0 = *x0 - lr * grad_x;
        *y0 = *y0 - lr * grad_y;
        if ((fabs(grad_x) < eps && fabs(grad_y) < eps) || (fabs(f(*x0, *y0) - f0) < eps)) {
            break;
        }
    }
}

double round_to_zero(double value, double threshold)
{
    if (fabs(value) < threshold) {
        return 0.0;
    }
    return value;
}

int main()
{
    double x0, y0;
    if (scanf("%lf %lf", &x0, &y0) != 2) {
        return 1;
    }
    grad_descent(&x0, &y0);
    // -0.000 ???? :)))
    x0 = round_to_zero(x0, 1e-9);
    y0 = round_to_zero(y0, 1e-9);
    printf("%.3lf %.3lf\n", x0, y0);
    return 0;
}
