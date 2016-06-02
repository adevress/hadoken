/**
 * Copyright (C) 2016 Adrien Devresse
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
*/
#ifndef _HADOKEN_RANDOM_HPP_
#define _HADOKEN_RANDOM_HPP_


#include <cassert>
#include <algorithm>
#include <vector>

#include <boost/random.hpp>
#include <boost/noncopyable.hpp>
#include <boost/integer.hpp>
#include <boost/scoped_ptr.hpp>

namespace hadoken {

// internals
namespace impl {
    class abstract_engine;
}



///
/// runtime abstraction layer for the C++11 / boost.Random
/// random engine system
///
/// Allow to abstract different random generators behind a single
/// interface at runtime
///
///
class random_engine_mapper : private boost::noncopyable {
public:


    typedef boost::uint32_t result_type;

    ///
    /// map a specialized random generator in the C++11 / boost format
    ///  to a generic random_engine_mapper that can be used in any distribution
    ///
    template<typename Engine>
    inline random_engine_mapper(const Engine & intern);

    /// reset to defautl seed, mapping
    inline void seed();

    /// reset to seed X, mapping
    inline void seed(result_type seed);


    /// generator operation
    inline result_type operator ()();

    /// minimum value returned by engine
    /// map to minimum value of the type
    static inline result_type min(){
        return std::numeric_limits<result_type>::min();
    }

    /// minimum value returned by engine
    /// map to maximum value of the type
    static inline result_type max(){
        return std::numeric_limits<result_type>::max();
    }
    
private:    

    boost::scoped_ptr<impl::abstract_engine> _engine;
};


}

#include "impl/random_engine_mapper_misc.hpp"

#endif // _HADOKEN_RANDOM_HPP_
