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
#pragma once

#include <numeric>
#include <limits>


namespace hadoken{



///
/// @brief compare two floating point number, with absolute range precision check
/// @param f1: first float to compare
/// @param f2: second float to compare
/// @return true if f1 and f2 are close enought, otherwise, false
///
/// default precision is setup to 100 times the system epsilon value
///
template <typename FloatType>
bool close_to_abs(const FloatType f1, const FloatType f2,
                  const FloatType epsilon_abs = std::numeric_limits<FloatType>::epsilon()*100){
    return (std::abs<FloatType>(f1 -f2) <= epsilon_abs);

}

///
/// @brief compare two floating point number, with relative range precision check
/// @param f1: first float to compare
/// @param f2: second float to compare
/// @return true if f1 and f2 are close enought, otherwise, false
///
template <typename FloatType>
bool close_to_rel(const FloatType f1, const FloatType f2,
                  const FloatType epsilon_rel = std::numeric_limits<FloatType>::epsilon()){
    return ( (std::abs<FloatType>(f1 -f2) <=  ((std::max(std::abs(f1), std::abs(f2)))) * epsilon_rel) );
}




template <typename FloatType>
inline bool delta_less(const FloatType f1, const FloatType f2, const FloatType delta = std::numeric_limits<FloatType>::epsilon()*100){
    return (f1 < (f2 + delta)) && ( (f1 + delta) < f2);
}


///
/// use absolute and relative comparison
///
template <typename FloatType>
bool close_to(const FloatType f1, const FloatType f2,
                  const FloatType epsilon_rel  = std::numeric_limits<FloatType>::epsilon()*10,
                  const FloatType epsilon_abs = std::numeric_limits<FloatType>::epsilon()*10){
    return f1 == f2 /* ideal */
            || close_to_abs(f1, f2, epsilon_abs)  /* absolute */
            || close_to_rel(f1, f2, epsilon_rel) ; /* relative */

}



} // hadoken
