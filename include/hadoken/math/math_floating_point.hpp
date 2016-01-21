
#ifndef _HADOKEN_FLOATING_POINT_HPP_
#define _HADOKEN_FLOATING_POINT_HPP_

#include <numeric>
#include <limits>


namespace hadoken{

namespace math{


///
/// @brief compare two floating point number, it supports relative and absolute precision range
/// @param f1: first float to compare
/// @param f2: second float to compare
/// @return true if f1 and f2 are close enought, otherwise, false
///
/// default precision is setup to 10 times the system epsilon value
///
template <typename FloatType>
bool almost_equal(const FloatType f1, const FloatType f2,
                  const FloatType epsilon_rel  = std::numeric_limits<FloatType>::epsilon()*10,
                  const FloatType epsilon_abs = std::numeric_limits<FloatType>::epsilon()*10){
    return f1 == f2 /* ideal */
            || (std::abs(f1 -f2) <= epsilon_abs)  /* absolute */
            || ( (std::abs(f1 -f2)/ (std::max(std::abs(f1), std::abs(f2)))) <= epsilon_rel ) ; /* relative */

}


}

}



#endif // _HADOKEN_FLOATING_POINT_HPP_
