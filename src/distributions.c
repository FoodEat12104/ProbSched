#include "distributions.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

static int initialized = 0;

void init_random() {
    if (!initialized) {
        srand(time(NULL));
        initialized = 1;
    }
}

int poisson_distribution(double lambda) {
    init_random();
    double L = exp(-lambda);
    double p = 1.0;
    int k = 0;
    
    do {
        k++;
        p *= (double)rand() / RAND_MAX;
    } while (p > L);
    
    return k - 1;
}

double exponential_distribution(double lambda) {
    init_random();
    return -log(1.0 - (double)rand() / RAND_MAX) / lambda;
}

int uniform_distribution(int min, int max) {
    init_random();
    return min + rand() % (max - min + 1);
}

int normal_distribution(int mean, int stddev) {
    init_random();
    double u1 = (double)rand() / RAND_MAX;
    double u2 = (double)rand() / RAND_MAX;
    double z = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
    return mean + stddev * z;
}