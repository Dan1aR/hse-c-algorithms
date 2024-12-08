#include <math.h>
#include <stdio.h>
#include <stdlib.h>

double f(double x, double y)
{
    if (x == 0.0) {
        return NAN;
    }
    return y / x - log(x) / x;
}

int main()
{
    double a, b, c;
    if (scanf("%lf %lf %lf", &a, &b, &c) != 3) {
        return 1;
    }

    double h = 1e-6;
    int steps = (int)(fabs(b - a) / fabs(h));
    h = (b - a) / steps;

    double x = a;
    double y = c;
    for (int i = 0; i < steps; i++) {
        double fxy = f(x, y);
        y += h * fxy;
        x += h;
    }

    if (fabs(x - b) > 1e-12) {
        h = b - x;
        y += h * f(x, y);
        x = b;
    }

    printf("%.4lf\n", y);
    return 0;
}
