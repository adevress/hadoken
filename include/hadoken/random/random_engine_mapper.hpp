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
#ifndef _HADOKEN_RANDOM_ENGINE_MAPPER_HPP_
#define _HADOKEN_RANDOM_ENGINE_MAPPER_HPP_


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

#endif // _HADOKEN_RANDOM_ENGINE_MAPPER_HPP_
