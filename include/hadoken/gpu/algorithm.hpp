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
#ifndef HADOKEN_GPU_ALGORITHM_HPP
#define HADOKEN_GPU_ALGORITHM_HPP



#include <hadoken/config/platform_config.hpp>

namespace hadoken {

namespace gpu {


//
// simplified implementation of copy for GPU
//
template <class InputIt, class OutputIt>
HADOKEN_DECORATE_HOST_DEVICE OutputIt copy(InputIt first, InputIt last, OutputIt d_first) {
    while (first < last) {
        *d_first = *first;
        first++;
        d_first++;
    }
    return d_first;
}


//
// simplified implementation of accumulate for GPU
//
template <class InputIt, class T, class BinaryOperation>
HADOKEN_DECORATE_HOST_DEVICE T accumulate(InputIt first, InputIt last, T init, BinaryOperation op) {
    T res = init;
    while (first < last) {
        op(res, *first);
        first++;
    }
    return res;
}


//
// simplified implementation of transform for GPU
//
template <class InputIt1, class InputIt2, class OutputIt, class BinaryOperation>
HADOKEN_DECORATE_HOST_DEVICE OutputIt transform(InputIt1 first1, InputIt1 last1, InputIt2 first2, OutputIt d_first,
                                                BinaryOperation binary_op) {
    while (first1 < last1) {
        *d_first = binary_op(*first1, *first2);
        first1++;
        d_first++;
        first2++;
    }
    return d_first;
}


//
// binary functions
//
template <typename T>

struct bit_xor {
    HADOKEN_DECORATE_HOST_DEVICE
    T operator()(const T& x, const T& y) const { return x ^ y; }
};


template <typename T>
struct plus {
    HADOKEN_DECORATE_HOST_DEVICE
    T operator()(const T& x, const T& y) const { return x + y; }
};

} // namespace gpu

} // namespace hadoken

#endif // HADOKEN_GPU_ALGORITHM_HPP
