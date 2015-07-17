/* 
 * File:   matrix_diagonalization.h
 * Author: winckler
 *
 * Created on July 15, 2015, 11:11 PM
 */

#ifndef MATRIX_DIAGONALIZATION_H
#define	MATRIX_DIAGONALIZATION_H

// std
#include <complex>

// boost
#include <boost/numeric/ublas/symmetric.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

// bindings
#include "boost/numeric/bindings/lapack/syev.hpp"
#include "boost/numeric/bindings/lapack/gesvd.hpp"
#include "boost/numeric/bindings/lapack/gesdd.hpp"
#include "boost/numeric/bindings/traits/ublas_matrix.hpp"
#include "boost/numeric/bindings/traits/ublas_vector.hpp"
#include <boost/numeric/bindings/lapack/geev.hpp>


/*
 
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/bindings/lapack/geev.hpp>
#include <boost/numeric/bindings/traits/vector_traits.hpp>
#include <boost/numeric/bindings/traits/ublas_matrix.hpp>
#include <boost/numeric/bindings/traits/ublas_vector2.hpp>
#include <boost/numeric/bindings/traits/c_array.hpp>
#include <complex>
 */


namespace ublas  = boost::numeric::ublas;
namespace lapack = boost::numeric::bindings::lapack;

namespace bear
{
    template<typename T>
    inline int diagonalize_gen(
                            ublas::matrix<T, ublas::column_major>& A, 
                            ublas::vector<std::complex<T> >& eigen_values,
                            ublas::matrix<std::complex<T>, ublas::column_major>* eigen_vectors_inv,
                            ublas::matrix<std::complex<T>, ublas::column_major>* eigen_vectors
                          )
    {
        int i_err=lapack::geev(A, eigen_values, eigen_vectors_inv, eigen_vectors, lapack::optimal_workspace());
        return i_err;
    }
    
    template<typename M, typename V>
    inline int diagonalize_sym(M& eigen_vectors, V& eigen_values) 
    {
        int i_err = lapack::syev( 'V', 'U', eigen_vectors, eigen_values, lapack::minimal_workspace() );
        return i_err;
    }
    
} // bear namespace

#endif	/* MATRIX_DIAGONALIZATION_H */
