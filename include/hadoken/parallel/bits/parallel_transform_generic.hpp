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
#ifndef PARALLEL_TRANSFORM_GENERIC_BITS_HPP
#define PARALLEL_TRANSFORM_GENERIC_BITS_HPP

#include <algorithm>
#include <tuple>

#include <boost/iterator/zip_iterator.hpp>
#include <boost/tuple/tuple.hpp>

#include <hadoken/parallel/algorithm.hpp>


#include "parallel_generic_utils.hpp"


namespace hadoken {


namespace parallel {

template <class ExecutionPolicy, class InputIterator1, class InputIterator2, class OutputIterator, class BinaryOperation>
OutputIterator transform(ExecutionPolicy&& policy, InputIterator1 first1, InputIterator1 last1, InputIterator2 first2,
                         OutputIterator d_first, BinaryOperation binary_op) {
    if (detail::is_parallel_policy(policy)) {
        InputIterator1 d_last = d_first;

        hadoken::parallel::for_range(policy, first1, last1, [&](InputIterator1 local_begin, InputIterator2 local_end) {
            const std::size_t pos = std::distance(first1, local_begin);

            InputIterator2 local_first2 = first2;
            std::advance(local_first2, pos);
            OutputIterator d_local_first = d_first;
            std::advance(d_local_first, pos);

            std::transform(local_begin, local_end, local_first2, d_local_first, binary_op);
        });

        std::advance(d_last, std::distance(first1, last1));
        return d_last;
    } else {
        return std::transform(first1, last1, first2, d_first, binary_op);
    }
}



template <class ExecutionPolicy, class InputIt, class OutputIt, class UnaryOperation>
OutputIt transform(ExecutionPolicy&& policy, InputIt first1, InputIt last1, OutputIt d_first, UnaryOperation unary_op) {
    using value_type = typename std::iterator_traits<InputIt>::value_type;
    return transform(policy, first1, last1, first1, d_first, [&unary_op](const value_type& v1, const value_type& v2) {
        (void)v2;
        return unary_op(v1);
    });
}

} // namespace parallel

} // namespace hadoken

#endif // PARALLEL_ALGORITHM_GENERICS_HPP
