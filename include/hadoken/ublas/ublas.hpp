/**
 * Copyright (c) 2016, Adrien Devresse <adrien.devresse@epfl.ch>
 *
 * Boost Software License - Version 1.0
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */
#ifndef _HADOKEN_UBLAS_HPP_
#define _HADOKEN_UBLAS_HPP_

#include <boost/numeric/ublas/matrix.hpp>

namespace hadoken {

namespace ublas {

using namespace boost::numeric::ublas;

///
/// Execute a predicate on every element of the matrix
///
template <typename T, typename Fun>
inline void for_each(boost::numeric::ublas::matrix<T>& mat, Fun f) {
    for (auto it1 = mat.begin1(); it1 < mat.end1(); ++it1) {
        for (auto it = it1.begin(); it < it1.end(); ++it) {
            f(*it);
        }
    }
}


///
/// set all element of the matrix to zero
///
template <typename T>
inline void zero(boost::numeric::ublas::matrix<T>& mat) {
    for_each(mat, [](T& val) {
        constexpr T null_value = 0;
        val = null_value;
    });
}




} // namespace ublas


} // namespace hadoken

#endif // UBLAS_HPP
