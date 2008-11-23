#ifndef KRYLOV_H
#define KRYLOV_H

#include "linalg.h"

/* Apply |start-stop| Householder reflectors in B to z
 * B stores the Householder vectors in row major form
 *
 * Implements the below python
 *
 * for j in range(start,stop,step):
 *   z = z - 2.0*dot(conjugate(B[j,:]), v)*B[j,:]
 */
template<class I, class T, class F>
void apply_householders(T z[], const T B[], const I n, const I start, const I stop, const I step)
{
    for(I i = start; i != stop; i+=step)
    {
        T alpha = dot_prod(&(B[i*n]), &(z[0]), n)*(-2.0);
        axpy(&(z[0]), &(B[i*n]), alpha, n);
    }
}

/* For use after gmres is finished iterating and the least squares
 * solution has been found.  This routine maps the solution back to
 * the original space via the Householder reflectors.
 *
 * Apply |start-stop| Householder reflectors in B to z
 * while also adding in the appropriate value from y, so 
 * that we follow the Horner-like scheme to map our least squares
 * solution in y back to the original space
 * 
 * B stores the Householder vectors in row major form
 *
 * Implements the below python
 *
 *for j in range(inner,-1,-1):
 *  z[j] += y[j]
 *  # Apply j-th reflector, (I - 2.0*w_j*w_j.T)*upadate
 *  z = z - 2.0*dot(conjugate(B[j,:]), update)*B[j,:]
 */
template<class I, class T, class F>
void householder_hornerscheme(T z[], const T B[], const T y[], const I n, const I start, const I stop, const I step)
{
    for(I i = start; i != stop; i+=step)
    {
        z[i] += y[i];
        T alpha = dot_prod(&(B[i*n]), &(z[0]), n)*(-2.0);
        axpy(&(z[0]), &(B[i*n]), alpha, n);
    }
}

#endif