#ifndef _SMALL_VECTOR_BITS_HPP_
#define _SMALL_VECTOR_BITS_HPP_

#include <algorithm>
#include <cassert>
#include <limits>
#include <stdexcept>

#include "../small_vector.hpp"

namespace hadoken {

namespace containers {

namespace impl {


// boundary check
template <typename Iterator>
void _range_check(Iterator it1, Iterator it2, std::size_t pos) {
    if (pos >= static_cast<std::size_t>(it2 - it1)) {
        throw std::out_of_range("out of range in small_vector");
    }
}

// untyped deallocator
void inline _deallocate(void* ptr) { ::operator delete(ptr); }

// destroy object without deallocate
// specialize for improved efficiency with POD types
template <typename Iterator, typename ObType, typename Extra = void>
struct destroyer_func {
    inline void operator()(Iterator it) {
        static_assert(std::is_pod<ObType>::value, "invalid pod type");
        (void)it;
    }
};

template <typename Iterator, typename ObType>
struct destroyer_func<Iterator, ObType, typename std::enable_if<std::is_pod<ObType>::value == false>::type> {
    inline void operator()(Iterator it) {
        static_assert(std::is_pod<ObType>::value == false, "should be pod type");
        it->~ObType();
    }
};

// move object to an area of unitialized memory
// then destroy the old copy
template <typename Iterator, typename ObType, typename Extra = void>
struct unitialize_move_func {

    inline void operator()(Iterator begin, Iterator end, Iterator output) {
        static_assert(std::is_pod<ObType>::value == false, "invalid pod type");

        destroyer_func<Iterator, ObType> _destroyer;

        for (; begin < end; begin++, output++) {
            output = new (output) ObType(std::move(*begin));
            _destroyer(begin);
        }
    }
};

template <typename Iterator, typename ObType>
struct unitialize_move_func<Iterator, ObType, typename std::enable_if<std::is_pod<ObType>::value == true>::type> {

    inline void operator()(Iterator begin, Iterator end, Iterator output) { std::uninitialized_copy(begin, end, output); }
};

} // namespace impl

template <typename T, std::size_t N>
small_vector<T, N>::small_vector()
    : _begin(reinterpret_cast<T*>(_internal_array)), _end(reinterpret_cast<T*>(_internal_array)), _end_memory(_begin + N),
      _internal_array() {}


template <typename T, std::size_t N>
small_vector<T, N>::~small_vector() {
    if (_is_static() == false) {

        // destroy object if not simple layout
        for (auto it = _begin; it < _end; ++it) {
            impl::destroyer_func<pointer, T> _destroyer;
            _destroyer(it);
        }
        impl::_deallocate(_begin);
    }
}


template <typename T, std::size_t N>
void small_vector<T, N>::resize(size_type new_size) {
    const size_type current_size = size();
    if (new_size == current_size) {
        return;
    } else if (new_size >= current_size) {
        _resize_to_fit(new_size);
        const size_type required_elems = new_size - current_size;

        for (size_type i = 0; i < required_elems; ++i) {
            emplace_back(T());
        }
    } else {
        const size_type required_shrink = current_size - new_size;

        for (size_type i = 0; i < required_shrink; ++i) {
            pop_back();
        }
    }
}

template <typename T, std::size_t N>
typename small_vector<T, N>::size_type small_vector<T, N>::max_size() const noexcept {
    return std::numeric_limits<std::size_t>::max();
}

template <typename T, std::size_t N>
typename small_vector<T, N>::size_type small_vector<T, N>::size() const noexcept {
    return static_cast<size_type>(_end - _begin);
}

template <typename T, std::size_t N>
void small_vector<T, N>::push_back(const_reference v) {
    _resize_to_fit(size() + 1);

    (void)new (_end) T(v);
    _end++;
}


template <typename T, std::size_t N>
void small_vector<T, N>::emplace_back(value_type&& v) {
    _resize_to_fit(size() + 1);

    (void)new (_end) T(std::move(v));
    _end++;
}


template <typename T, std::size_t N>
void small_vector<T, N>::pop_back() {
    assert(empty() == false);
    _end -= 1;

    impl::destroyer_func<pointer, T> _destroyer;
    _destroyer(_end);
}



template <typename T, std::size_t N>
typename small_vector<T, N>::reference small_vector<T, N>::front() {
    assert((_end - _begin) >= 1);
    return *_begin;
}



template <typename T, std::size_t N>
typename small_vector<T, N>::const_reference small_vector<T, N>::front() const noexcept {
    assert((_end - _begin) >= 1);
    return *_begin;
}


template <typename T, std::size_t N>
typename small_vector<T, N>::reference small_vector<T, N>::back() {
    assert((_end - _begin) >= 1);
    return *((std::max(_begin, _end - 1)));
}

template <typename T, std::size_t N>
typename small_vector<T, N>::const_reference small_vector<T, N>::back() const noexcept {
    assert((_end - _begin) >= 1);
    return *((std::max(_begin, _end - 1)));
}

template <typename T, std::size_t N>
typename small_vector<T, N>::pointer small_vector<T, N>::data() noexcept {
    return &(*_begin);
}



template <typename T, std::size_t N>
bool small_vector<T, N>::empty() const noexcept {
    return size() == 0;
}

template <typename T, std::size_t N>
std::size_t small_vector<T, N>::capacity() const noexcept {
    return static_cast<std::size_t>(_end_memory - _begin);
}


template <typename T, std::size_t N>
typename small_vector<T, N>::reference small_vector<T, N>::operator[](std::size_t pos) noexcept {
    assert(std::ptrdiff_t(pos) < (_end - _begin));
    return *(_begin + static_cast<std::ptrdiff_t>(pos));
}


template <typename T, std::size_t N>
typename small_vector<T, N>::const_reference small_vector<T, N>::operator[](std::size_t pos) const noexcept {
    assert(std::ptrdiff_t(pos) < (_end - _begin));
    return *(_begin + static_cast<std::ptrdiff_t>(pos));
}


template <typename T, std::size_t N>
typename small_vector<T, N>::reference small_vector<T, N>::at(std::size_t pos) {
    impl::_range_check(_begin, _end, pos);
    return (*this)[pos];
}


template <typename T, std::size_t N>
typename small_vector<T, N>::const_reference small_vector<T, N>::at(std::size_t pos) const {
    impl::_range_check(_begin, _end, pos);
    return (*this)[pos];
}

template <typename T, std::size_t N>
void swap(small_vector<T, N>& first, small_vector<T, N>& second) {
    using namespace std;

    using size_type = typename small_vector<T, N>::size_type;

    // if both small vector are static
    // just swap the element
    if (first._is_static() && second._is_static()) {
        for (std::size_t i = 0; i < N; ++i) {
            swap(first._internal_array[i], second._internal_array[i]);
        }

        const size_type second_size = second.size();
        const size_type local_size = first.size();
        first._end = first._begin + second_size;
        second._end = second._begin + local_size;

        return;
    }

    // if not
    // force both vector to allocate on the heap
    // before swapping them

    if (first._is_static()) {
        first._resize_to_fit(2 * N);
    }

    if (second._is_static()) {
        second._resize_to_fit(2 * N);
    }

    swap(first._begin, second._begin);
    swap(first._end, second._end);
    swap(first._end_memory, second._end_memory);
}



template <typename T, std::size_t N>
void small_vector<T, N>::_resize_to_fit(std::size_t required_size) {
    if (capacity() >= required_size) {
        return;
    }

    const std::size_t current_size = size();
    const bool static_allocated = _is_static();


    std::size_t allocation_size = 1;
    while (allocation_size < required_size) {
        allocation_size = allocation_size << 1;
    }

    // first reallocation from short buffer to extensible buffer needed
    pointer pdata_old = _begin;

    pointer pdata = static_cast<pointer>(static_cast<void*>(::operator new(allocation_size * sizeof(T))));

    impl::unitialize_move_func<T*, T> _mover;

    _mover(_begin, _end, pdata);

    if (static_allocated == false) {
        impl::_deallocate(pdata_old);
    }

    _begin = pdata;
    _end = _begin + current_size;
    _end_memory = _begin + allocation_size;
}

template <typename T, std::size_t N>
bool small_vector<T, N>::_is_static() const noexcept {
    return (_begin == reinterpret_cast<const T*>(&_internal_array));
}

} // namespace containers



} // namespace hadoken

#endif // SMALL_VECTOR_BITS_HPP
