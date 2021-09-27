#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void triangle(int n, double (*a)[n], double *b) {
    for (int j = 0; j < n - 1; j++) {
        for (int i = j + 1; i < n; i++) {
            double r = - a[i][j] / a[j][j];
            for (int k = j; k < n; k++) {
                a[i][k] += r * a[j][k];
            }
            b[i] += r * b[j];
        }
    }
}


void wildHunt(FILE* in, FILE* out) {
    int n;
    fscanf(in, "%d", &n);
    double *upper = malloc((n - 1) * sizeof(double));
    double *middle = malloc(n * sizeof(double));
    double *lower = malloc((n - 1) * sizeof(double));
    double *f = malloc(n * sizeof(double));
    double *p = malloc(n * sizeof(double));
    double *q = malloc(n * sizeof(double));
    double *x = malloc(n * sizeof(double));
    for (int i = 0; i < n - 1; i++) {
        fscanf(in, "%lf", &upper[i]);
    }
    for (int i = 0; i < n; i++) {
        fscanf(in, "%lf", &middle[i]);
    }
    for (int i = 0; i < n - 1; i++) {
        fscanf(in, "%lf", &lower[i]);
    }
    for (int i = 0; i < n; i++) {
        fscanf(in, "%lf", &f[i]);
    }


    p[0] = upper[0] / middle[0];
    q[0] = f[0] / middle[0];
    for (int i = 1; i < n - 1; i++) {
        double y = middle[i] - p[i - 1] * lower[i - 1];
        p[i] = upper[i] / y;
        q[i] = (f[i] - q[i - 1] * lower[i - 1]) / y;
    }
    q[n - 1] = (f[n - 1] - q[n - 2] * lower[n - 2]) /
            (middle[n - 1] - p[n - 2] * lower[n - 2]);


    x[n - 1] = q[n - 1];
    for (int i = n - 2; i >= 0; i--) {
        x[i] = q[i] - p[i] * x[i + 1];
    }

    for (int i = 0; i < n; i++) {
        fprintf(stdout, "%.8lf ", x[i]);
    }
    free(upper);
    free(middle);
    free(lower);
    free(f);
    free(p);
    free(x);
    free(q);
}

double triangleDet(int n, double (*a)[n]) {
    double det = 1;
    for (int i = 0; i < n; i++) {
        det *= a[i][i];
    }
    return det;
}

void reverse(int n, double (*a)[n], const double *b, double * x) {
    for (int i = n - 1; -1 < i; i--) {
        double sum = 0;
        for (int j = n - 1; i < j; j--) sum += x[j] * a[i][j];
        x[i] = (b[i] - sum) / a[i][i];
    }
}

void find_inverse(int n, double (*a)[n], double (*clear)[n], double * b, double (*inv)[n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (j == i) b[j] = 1;
            else b[j] = 0;
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                a[i][j] = clear[i][j];
            }
        }

        triangle(n, a, b);
        reverse(n, a, b, inv[i]);
    }
}

int main() {
    FILE *in;
    FILE *out;
    in = fopen("Z:/codes/vych/gauss/input.txt", "r");
    out = fopen("Z:/codes/vych/gauss/output.txt", "w+");
    int check;
    fscanf(in, "%d", &check);
    if (check == 1) {
        wildHunt(in, out);
        fclose(in);
        fclose(out);
        return 0;
    }
    int n;
    fscanf(in, "%d", &n);
    double (*a)[n] = malloc(n * n * sizeof(double *));
    double (*clear_a)[n] = malloc(n * n * sizeof (double *));
    double *b = (double*) malloc(n * sizeof(double));
    double *x = (double*) malloc(n * sizeof(double));
    double (*inverse)[n] = malloc(n * n * sizeof(double *));
    if (a == NULL || b == NULL || x == NULL || inverse == NULL) {
        exit(-1);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fscanf(in, "%lf", &a[i][j]);
            clear_a[i][j] = a[i][j];
        }
    }

    for (int i = 0; i < n; i++) {
        fscanf(in, "%lf", &b[i]);
    }

    fclose(in);

    triangle(n, a, b);

    double det = triangleDet(n, a);

    reverse(n, a, b, x);

    // print
    fprintf(out, "Triangle form:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fprintf(out, "%.8lf ", a[i][j]);
        }
        fprintf(out, "\n");
    }
    fprintf(out, "\n");

    for (int i = 0; i < n; i++) {
        fprintf(out, "%.8lf ", x[i]);
    }
    fprintf(out, "\n\nDeterminant: %.8lf\n\n", det);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a[i][j] = clear_a[i][j];
        }
    }
    find_inverse(n, a, clear_a, b, inverse);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fprintf(out, "%.8lf ", inverse[j][i]);
        }
        fprintf(out, "\n");
    }
    fprintf(out, "\n");

    fclose(out);

    free(a);
    free(clear_a);
    free(b);
    free(x);
    free(inverse);
    return 0;
}