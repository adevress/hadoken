#ifndef _SMALL_VECTOR_BITS_HPP_
#define _SMALL_VECTOR_BITS_HPP_

#include <stdexcept>
#include <limits>
#include <cassert>

#include  "../small_vector.hpp"

namespace hadoken{

namespace containers{

namespace {



template<typename Iterator>
void _range_check(Iterator it1, Iterator it2, std::size_t pos){
    if( pos >= static_cast<std::size_t>(it2 - it1) ){
        throw std::out_of_range("out of range in small_vector");
    }
}


}


template<typename T, std::size_t N>
small_vector<T,N>::~small_vector(){
    if(_begin != _end_memory){
        delete [] _begin;
    }
}



template<typename T, std::size_t N>
typename small_vector<T,N>::size_type
small_vector<T,N>::max_size() const noexcept{
        return std::numeric_limits<std::size_t>::max();
}


template<typename T, std::size_t N>
void small_vector<T,N>::push_back(const_reference v){
    const std::size_t pos = (_end - _begin);
    if(  pos >= std::max<std::size_t>(N, _end_memory - _begin)){
        _resize_to_fit(pos+1);
    }

    *_end = v;
    _end++;
}


template<typename T, std::size_t N>
void small_vector<T,N>::emplace_back(value_type && v){
    const std::size_t pos = (_end - _begin);
    if( _is_static() == false || pos >= N){
        _resize_to_fit(pos+1);
    }

    *_end = std::move(v);
    _end++;
}


template<typename T, std::size_t N>
typename small_vector<T,N>::reference small_vector<T,N>::front(){
   assert((_end - _begin) >= 1);
   return *_begin;
}


template<typename T, std::size_t N>
typename small_vector<T,N>::const_reference small_vector<T,N>::front() const noexcept{
   assert((_end - _begin) >= 1);
   return *_begin;
}


template<typename T, std::size_t N>
typename small_vector<T,N>::pointer
small_vector<T,N>::data() noexcept{
    return &(*_begin);
}



template<typename T, std::size_t N>
bool small_vector<T,N>::empty() const noexcept{
    return size() == 0;
}

template<typename T, std::size_t N>
std::size_t small_vector<T,N>::capacity() const noexcept{
    return static_cast<std::size_t>(_end_memory - _begin);
}


template<typename T, std::size_t N>
typename small_vector<T,N>::reference
small_vector<T,N>::operator[] (std::size_t pos) noexcept{
    assert(pos < (_end - _begin));
    return *( _begin + static_cast<std::ptrdiff_t>(pos)) ;
}


template<typename T, std::size_t N>
typename small_vector<T,N>::const_reference
small_vector<T,N>::operator[] (std::size_t pos) const noexcept{
    assert(pos < (_end - _begin));
    return  _begin + static_cast<std::ptrdiff_t>(pos);
}


template<typename T, std::size_t N>
typename small_vector<T,N>::reference
small_vector<T,N>::at (std::size_t pos){
    _range_check(_begin, _end, pos);
    return (*this)[pos];
}


template<typename T, std::size_t N>
typename small_vector<T,N>::const_reference
small_vector<T,N>::at (std::size_t pos) const{
    _range_check(_begin, _end, pos);
    return (*this)[pos];
}

template<typename T, std::size_t N>
void small_vector<T,N>::swap(small_vector<T,N> & other){
    using namespace std;

    if(_is_static()){
        swap(_internal_array, other._internal_array);
    }

    swap(_begin, other._begin);
    swap(_end, other._end);
    swap(_end_memory, other._end_memory);
}



template<typename T, std::size_t N>
void small_vector<T,N>::_resize_to_fit(std::size_t s){
    const std::size_t size_alloc = static_cast<std::size_t>(_end_memory - _begin);

    if(size_alloc <= s){
        const std::size_t n_elems = static_cast<std::size_t>(_end - _begin);
        const std::size_t required_size= std::max((size_alloc << 1), std::max(s, N*2));

        // first reallocation from short buffer to extensible buffer needed
        pointer pdata_old = ( _is_static() ) ? nullptr : _begin;
        pointer pdata = static_cast<pointer>(::operator new[](required_size * sizeof(T)));

        std::move(_begin, _end, pdata);

        delete[] pdata_old;

        _begin = pdata;
        _end = _begin + n_elems;
        _end_memory = _begin + required_size;
    }

}

template<typename T, std::size_t N>
bool small_vector<T,N>::_is_static() const noexcept{
    return (_end_memory == _begin);
}

} //containers



} //hadoken

#endif // SMALL_VECTOR_BITS_HPP
