/**
 * Copyright (C) 2016 Adrien Devresse
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
*/
#ifndef _HADOKEN_RANGE_HPP_
#define _HADOKEN_RANGE_HPP_


#include <cassert>
#include <algorithm>
#include <vector>

namespace hadoken {


template <typename Container>
class range{
public:
    typedef typename Container::iterator iterator;

    inline range(const iterator & first_elem, const iterator & end_elem) : first_(first_elem), end_(end_elem){
        //assert(first_elem <= end_elem);
    }

    inline iterator & begin(){
        return first_;
    }

    inline iterator & end(){
        return end_;
    }

    ///
    /// \brief size
    /// \return number of element in the range
    ///
    inline size_t size(){
        return std::distance(first_, end_);
    }

    inline std::vector<range<Container> > split(std::size_t number_parts, std::size_t min_size=1){
        assert(number_parts > 0);
        assert(min_size >= 1);

        std::vector<range<Container> > ranges;
        const std::size_t size_range = size();
        const std::size_t avg_size= size_range/number_parts;
        std::size_t remain_elems = size_range%number_parts;
        ranges.reserve(number_parts);

        iterator first = begin(), last = begin();

        do{
            std::size_t segment_size = avg_size + ((remain_elems > 0)?1:0);
            segment_size = std::max<std::size_t>(segment_size, min_size);
            segment_size = std::min<std::size_t>(segment_size, std::distance(first, end()));
            std::advance(last, segment_size);
            ranges.push_back( range(first, last));
            first = last;
            remain_elems = ((remain_elems > 0)?(remain_elems-1):0);
        } while(ranges.size() < number_parts);

        return ranges;
    }


private:
    iterator first_, end_;
};

}

#endif // RANGE_HPP
