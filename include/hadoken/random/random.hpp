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
/// generate in a deterministic way a new
///
boost::uint32_t generate_deterministic_seed(boost::uint32_t origin_seed, boost::uint32_t key);


///
/// runtime abstraction layer for the C++11 / boost.Random
/// random engine system
///
/// Allow to abstract different random generators behind a single
/// interface at runtime
///
///
class random_engine_mapper {
public:


    typedef boost::uint32_t result_type;

    /// default constructor
    /// generate empty mapper
    random_engine_mapper();
    ///
    /// map a specialized random generator in the C++11 / boost format
    ///  to a generic random_engine_mapper that can be used in any distribution
    ///
    template<typename Engine>
    inline random_engine_mapper(const Engine & intern);
    inline random_engine_mapper(const random_engine_mapper & other);


    /// reset to defautl seed, mapping
    inline void seed();

    /// reset to seed X, mapping
    inline void seed(result_type seed);

    /// generator operation
    inline result_type operator ()();

    /// derivate create a random engine
    ///  derivated from the current random engine
    ///  seed and the key.
    ///
    ///  The derivation respect the following rules
    ///  - The operation is always deterministic for a given key and seed
    ///  - The new random engine do not have statistical correlation with the old one
    ///  - Two different keys, even close in range guarantee two independent random streams
    ///
    ///
    inline random_engine_mapper derivate(result_type key);

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
