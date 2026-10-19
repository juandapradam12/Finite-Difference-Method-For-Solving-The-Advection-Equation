#include "error.h"

#include <math.h>

double exact_pulse(double x, double t, double c)
{
    const double xi = x - c * t;
    if (fabs(xi) <= 0.5) {
        return 0.25 - xi * xi;
    }
    return 0.0;
}

void error_norms(const Field1D *f, double t, double c,
                 double *l2, double *linf)
{
    double sum_sq = 0.0;
    double max_abs = 0.0;

    for (int i = 0; i < f->n; ++i) {
        const double e = f->u[i] - exact_pulse(f->x[i], t, c);
        const double ae = fabs(e);
        sum_sq += e * e;
        if (ae > max_abs) {
            max_abs = ae;
        }
    }

    if (l2) {
        *l2 = sqrt(f->dx * sum_sq);
    }
    if (linf) {
        *linf = max_abs;
    }
}
