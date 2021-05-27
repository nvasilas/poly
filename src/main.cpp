#include "coprime_factorization.h"
#include "polynomial.h"
#include "polynomial_fraction.h"
#include "polynomial_fraction_matrix.h"
#include "polynomial_matrix.h"

#include <iostream>
#include <math.h>
#include <ostream>

using namespace poly;

int main()
{
    auto mf = (PolynomialFractionMatrix(3, 2)
                   << PolynomialFraction<double>{{1}, {2, 1}},
               PolynomialFraction<double>{{0}, {1}},
               PolynomialFraction<double>{{1}, {3, 1}},
               PolynomialFraction<double>{{1}, {-3, 1}},
               PolynomialFraction<double>{{1}, {2, -3, 1}},
               PolynomialFraction<double>{{0}, {1}})
                  .finished();
    coprime_factorization(mf);

    return 0;
}
