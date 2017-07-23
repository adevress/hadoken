#ifndef SPMD_ARRAY_HPP
#define SPMD_ARRAY_HPP



#include <algorithm>
#include <cassert>
#include <cmath>
#include <ctgmath>
#include <functional>
#include <initializer_list>
#include <sstream>
#include <string>
#include <type_traits>



#ifndef HADOKEN_FORCE_INLINE
#define HADOKEN_FORCE_INLINE inline
#endif

namespace hadoken {

namespace spmd {

namespace {

template<typename NumType, std::size_t N, typename Fun>
inline void simd_for_each(const NumType* a, NumType* res, Fun ops = Fun()){
    #pragma omp simd
    for(std::size_t i = 0; i < N; i++){
        res[i] = ops(a[i]);
    }
}

template<typename NumType, std::size_t N, typename Fun>
inline void simd_transform(const NumType* a, const NumType* b, NumType* c, Fun ops = Fun()){
    #pragma omp simd
    for(std::size_t i = 0; i < N; i++){
        c[i] = ops(a[i], b[i]);
    }
}

template<typename NumType, typename ResType, std::size_t N, typename Fun>
inline void simd_transform2(const NumType* a, const NumType* b, ResType* c, Fun ops = Fun()){
    #pragma omp simd
    for(std::size_t i = 0; i < N; i++){
        c[i] = ops(a[i], b[i]);
    }
}


template <typename NumType, std::size_t N, typename Fun >
inline bool simd_all_of(const NumType* a, const Fun & ops = Fun()){
    std::array<bool, N> all_bool;

    #pragma omp simd
    for(std::size_t i = 0; i < N; i++){
	all_bool[i] = ops(a[i]);
    }
    return std::all_of(all_bool.begin(), all_bool.end(), [](bool v){ return v;});
}




} // 


template<typename NumType, std::size_t N, std::size_t Aligment = (sizeof(NumType) * N) >
class alignas(Aligment) spmd_array{
public:
    typedef spmd_array<NumType, N, Aligment> spmd_array_t;
    typedef NumType* iterator;
    typedef const NumType* const_iterator;

    spmd_array() = default;
    spmd_array(const spmd_array_t & ) = default;
    spmd_array& operator = (const spmd_array_t & ) = default;

    HADOKEN_FORCE_INLINE spmd_array(std::initializer_list<NumType> init_list){
        std::copy_n(init_list.begin(), N, begin());
    }



    HADOKEN_FORCE_INLINE iterator begin(){
        return &(_d[0]);
    }

    HADOKEN_FORCE_INLINE const_iterator begin() const{
        return &(_d[0]);
    }

    HADOKEN_FORCE_INLINE iterator end(){
        return begin() + N;
    }

    HADOKEN_FORCE_INLINE const_iterator end() const{
        return begin() + N;
    }


    ///
    /// simple packed / packed sum
    ///
    HADOKEN_FORCE_INLINE spmd_array_t operator + (const spmd_array_t & other) const{
        spmd_array_t res;
        simd_transform<NumType, N, std::plus<NumType> >(_d, other._d, res._d);
        return res;
    }


    ///
    /// simple packed /scalar sum
    ///
    HADOKEN_FORCE_INLINE spmd_array_t operator + ( NumType scalar) const{
        spmd_array_t res;
        simd_for_each<NumType, N>(_d, res._d, [scalar](NumType v){
            return v + scalar;
        });
        return res;
    }


    ///
    /// simple packed/packed sub
    ///
    HADOKEN_FORCE_INLINE spmd_array_t operator - (const spmd_array_t & other) const{
        spmd_array_t res;
        simd_transform<NumType, N, std::minus<NumType> >(_d, other._d, res._d);
        return res;
    }

    ///
    /// simple packed/scalar  sub
    ///
    HADOKEN_FORCE_INLINE spmd_array_t operator - ( NumType scalar) const{
        spmd_array_t res;
        simd_for_each<NumType, N>(_d, res._d, [scalar](NumType v){
            return v - scalar;
        });
        return res;
    }


    ///
    /// simple packed multiplication
    ///
    HADOKEN_FORCE_INLINE spmd_array_t operator * (const spmd_array_t & other) const{
        spmd_array_t res;
        simd_transform<NumType, N, std::multiplies<NumType> >(_d, other._d, res._d);
        return res;
    }

    ///
    /// simple packed/scalar mul
    ///
    HADOKEN_FORCE_INLINE spmd_array_t operator * ( NumType scalar) const{
        spmd_array_t res;
        simd_for_each<NumType, N>(_d, res._d, [scalar](NumType v){
            return v * scalar;
        });
        return res;
    }


    ///
    /// simple packed division
    ///
    HADOKEN_FORCE_INLINE spmd_array_t operator/ (const spmd_array_t & other) const{
        spmd_array_t res;
        simd_transform<NumType, N, std::divides<NumType> >(_d, other._d, res._d);
        return res;
    }

    ///
    /// simple packed/scalar mul
    ///
    HADOKEN_FORCE_INLINE spmd_array_t operator / ( NumType scalar) const{
        spmd_array_t res;
        simd_for_each<NumType, N>(_d, res._d, [scalar](NumType v){
            return v / scalar;
        });
        return res;
    }


    ///
    ///
    ///
    HADOKEN_FORCE_INLINE bool operator==(const spmd_array_t & other) const{
        spmd_array<bool, N> res;
        simd_transform2<NumType, bool, N, std::equal_to<NumType> >(begin(), other.begin(), res.begin());
        return simd_all_of<bool, N>(res.begin(), [](bool status){ return status;});
    }


    ///
    ///
    ///
    HADOKEN_FORCE_INLINE NumType operator[](std::size_t pos) const {
        assert( pos < N );
        return _d[pos];
    }

    ///
    ///
    ///
    HADOKEN_FORCE_INLINE NumType& operator[](std::size_t pos){
        assert( pos < N );
        return _d[pos];
    }


    ///
    ///
    ///
    HADOKEN_FORCE_INLINE NumType* data(){
        assert( pos < N );
        return &(_d[0]);
    }


    ///
    /// compile time range-checked getter
    ///
    template<std::size_t Pos>
    HADOKEN_FORCE_INLINE NumType get() const{
        static_assert( Pos < N, "get<Pos>: pos should be inferior to array size N ");
        return _d[Pos];
    }

    ///
    /// compile time range-checked setter
    ///
    template<std::size_t Pos>
    HADOKEN_FORCE_INLINE void set(const NumType v) const{
        static_assert( Pos < N, "get<Pos>: pos should be inferior to array size N ");
        _d[Pos] = v;
    }



    ///
    /// get size
    ///
    HADOKEN_FORCE_INLINE constexpr std::size_t size(){
        return N;
    }


private:

    NumType _d[N];

};



///
/// ostream serlializer
///
template<typename NumType, std::size_t N, std::size_t Aligment>
std::ostream& operator<<(std::ostream& os, const spmd_array<NumType, N, Aligment> & arr)
{
    os << '{';
    bool delimiter = false;
    std::for_each(arr.begin(), arr.end(), [&](const NumType & v){
       os <<  ( (delimiter) ? (",") : ("") );

       if(sizeof(NumType) == sizeof(char)){
            // convert char with a numerical formating
            os << (static_cast<int>(v) & 0xff );
       } else {
            os << (static_cast<int>(v) & 0xff );
       }
       delimiter = true;
    });
    os << '}';

    return os;
}


///
/// to string
///
template<typename NumType, std::size_t N, std::size_t Aligment>
std::string to_string(const spmd_array<NumType, N, Aligment> & arr){
    std::ostringstream res;
    res << arr;
    return res.str();
}



///
/// Check if each element of two same sized array are equal or close to equal in a given delta
///
template<typename NumType, std::size_t N, std::size_t Aligment>
typename std::enable_if<std::is_integral<NumType>::value, bool>::type
close_to_abs(const spmd_array<NumType, N, Aligment> & arr1, const spmd_array<NumType, N, Aligment> & arr2, NumType max_delta){
    spmd_array<bool, N> res;
    simd_transform2<NumType, bool, N>(arr1.begin(), arr2.begin(), res.begin(), [max_delta](NumType a, NumType b){
        const NumType biggest = std::max<NumType>(a, b);
        const NumType smallest = std::min<NumType>(a, b);
        return (std::abs<NumType>(biggest - smallest) <= max_delta);
    });
    return simd_all_of<bool, N>(res.begin(), [](bool status){ return status;});
}


template<typename NumType, std::size_t N, std::size_t Aligment>
typename std::enable_if<std::is_floating_point<NumType>::value, bool>::type
close_to_abs(const spmd_array<NumType, N, Aligment> & arr1, const spmd_array<NumType, N, Aligment> & arr2, NumType max_delta){
    spmd_array<bool, N> res;
    simd_transform2<NumType, bool, N>(arr1.begin(), arr2.begin(), res.begin(), [max_delta](NumType a, NumType b){
        return (std::fabs<NumType>(a - b) <= max_delta);
    });
    return simd_all_of<bool, N>(res.begin(), [](bool status){ return status;});
}



} // spmd


} // hadoken

#endif // SPMD_ARRAY_HPP

