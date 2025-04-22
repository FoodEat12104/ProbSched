#ifndef DISTRIBUTIONS_H
#define DISTRIBUTIONS_H

int poisson_distribution(double lambda);
double exponential_distribution(double lambda);
int uniform_distribution(int min, int max);
int normal_distribution(int mean, int stddev);

#endif