#include "field.h"

#include <math.h>
#include <stdlib.h>

Field1D *field_create(const AdvectionParams *p)
{
    Field1D *f = calloc(1, sizeof(*f));
    if (!f) {
        return NULL;
    }

    f->n = p->n_points;
    f->dx = params_dx(p);
    f->x = malloc((size_t)f->n * sizeof(double));
    f->u = calloc((size_t)f->n, sizeof(double));
    f->u_next = calloc((size_t)f->n, sizeof(double));

    if (!f->x || !f->u || !f->u_next) {
        field_free(f);
        return NULL;
    }

    for (int i = 0; i < f->n; ++i) {
        f->x[i] = p->x_min + (double)i * f->dx;
    }
    return f;
}

void field_free(Field1D *f)
{
    if (!f) {
        return;
    }
    free(f->x);
    free(f->u);
    free(f->u_next);
    free(f);
}

void field_set_initial_pulse(Field1D *f)
{
    for (int i = 0; i < f->n; ++i) {
        const double x = f->x[i];
        if (fabs(x) <= 0.5) {
            f->u[i] = 0.25 - x * x;
        } else {
            f->u[i] = 0.0;
        }
    }
    field_apply_dirichlet_zero(f);
}

void field_apply_dirichlet_zero(Field1D *f)
{
    f->u[0] = 0.0;
    f->u[f->n - 1] = 0.0;
}
