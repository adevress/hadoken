#ifndef _HADOKEN_POINT_BASE_HPP
#define _HADOKEN_POINT_BASE_HPP

#include <type_traits>
#include <array>
#include <limits>
#include <algorithm>

#include <hadoken/math/math_floating_point.hpp>

namespace hadoken{

namespace geometry{

namespace cartesian{


namespace {

    template<typename CoordType, std::size_t N>
    std::array<CoordType, N> assign_array(CoordType x, CoordType y, CoordType z){
        static_assert(N >= 1, "array need to be of dimension > 1");
        std::array<CoordType, N> arr;
        if(N > 0)
            arr[0] = x;
        if(N > 1)
            arr[1] = y;
        if(N > 2)
            arr[2] = z;
        return arr;
    }

}


template<typename CoordType, std::size_t N>
class point_base{
public:
    typedef CoordType value_type;
    typedef typename std::array<CoordType, N>::iterator iterator;
    typedef typename std::array<CoordType, N>::const_iterator const_iterator;
    typedef point_base<CoordType, N> point_type;


    /// default constructor
    /// build an empty point with N times 0
    inline point_base() : _point(assign_array<CoordType, N>(0,0,0)) {
        static_assert(N > 0, "N should be > 0");
    }

    /// build a point out of an initializor list
    inline point_base(const std::initializer_list<value_type> & init_list) : _point(){
        std::copy_n(init_list.begin(), std::min(init_list.size(), _point.size()), _point.begin());
    }


    /// construct a point out of cartesian coordinate (x,y,z)
    inline point_base(const value_type & x, const value_type & y, const value_type & z) :
        _point(assign_array<value_type, N>(x, y, z)) {}

    /// constructo an empty point out of a std::array
    inline point_base(const std::array<value_type, N> & array) :
        _point(array) {}

    /// default destructor
    inline ~point_base() {}


    const_iterator begin() const{
        return _point.begin();
    }

    iterator begin(){
        return _point.begin();
    }

    const_iterator end() const{
        return _point.end();
    }

    iterator end(){
        return _point.end();
    }

    /// access operator
    inline value_type operator()(const std::size_t i) const{
        assert(i < N);
        return _point[i];
    }

    /// set operator
    inline value_type& operator()(const std::size_t i){
        assert(i < N);
        return _point[i];
    }


    /// floating point equal
    inline bool close_to(const point_type & other, CoordType delta = std::numeric_limits<CoordType>::epsilon()*100 ) const {
        bool res = true;
        for(std::size_t i =0; i < N; ++i){
            res = res && hadoken::math::close_to(_point[i], other._point[i], delta, delta);
        }
        return res;
    }

    /// operator plus overload for point / point
    point_type operator+(const point_type & other) const{
        point_type res;
        std::transform(begin(), end(), other.begin(), res.begin(), std::plus<value_type>());
        return res;
    }

    /// operator minus overlaod for point / point
    point_type operator-(const point_type & other) const{
        point_type res;
        std::transform(begin(), end(), other.begin(), res.begin(), std::minus<value_type>());
        return res;
    }

    /// operator plus overload for point / scalar
    point_type operator*(const value_type scalar) const{
        point_type res;
        std::transform(begin(), end(), res.begin(), [scalar](const value_type v){ return scalar * v; });
        return res;
    }


    /// operator minus equal overload for point / point
    point_type& operator-=(const point_type & other){
        *this = *this - other;
        return *this;
    }


    /// operator plus equal overload for point / point
    point_type& operator+=(const point_type & other){
        *this = *this + other;
        return *this;
    }

    /// operator /= overload for point / scalar
    point_type& operator/=(const value_type scalar){
        for(auto & v : _point){
            v /= scalar;
        }
        return *this;
    }

    /// operator *= overload for point / scalar
    point_type& operator*=(const value_type scalar){
        *this = *this * scalar;
        return *this;
    }

    point_type& operator= (const point_type & other){
        if(this == &other)
            return *this;
        std::copy(other.begin(), other.end(), begin());
        return *this;
    }

    const value_type* data() const {
        return _point.data();
    }


protected:
        std::array<CoordType, N> _point;
};


template<typename CoordType, std::size_t N>
std::ostream & operator <<(std::ostream & ostream, const point_base<CoordType, N> & point){
    ostream << "(";
    ostream << point(0);
    for(std::size_t i =1; i < N;++i){
        ostream << "," << point(i);
    }
    ostream << ")";
    return ostream;
}


} //cartesian

} // geometry

} //hadoken


//
//
//
// register the type as valid boost.geometry point type
//
//
//
namespace boost { namespace geometry
{
#ifndef DOXYGEN_NO_TRAITS_SPECIALIZATIONS
namespace traits
{


// Assign the point-tag, preventing arrays of points getting a point-tag
template <typename CoordinateType, std::size_t DimensionCount>
struct tag<hadoken::geometry::cartesian::point_base<CoordinateType, DimensionCount> >{
    typedef point_tag type;
};

template <typename CoordinateType, std::size_t DimensionCount>
struct coordinate_type<hadoken::geometry::cartesian::point_base<CoordinateType, DimensionCount> >
{
    typedef CoordinateType type;
};


template <typename CoordinateType, std::size_t DimensionCount>
struct dimension<hadoken::geometry::cartesian::point_base<CoordinateType, DimensionCount> >: boost::mpl::int_<DimensionCount> {};


template <typename CoordinateType, std::size_t DimensionCount, std::size_t Dimension>
struct access<hadoken::geometry::cartesian::point_base<CoordinateType, DimensionCount>, Dimension>
{
    static inline CoordinateType get(hadoken::geometry::cartesian::point_base<CoordinateType, DimensionCount> const& a)
    {
        return a(Dimension);
    }

    static inline void set(hadoken::geometry::cartesian::point_base<CoordinateType, DimensionCount>& a,
        CoordinateType const& value)
    {
        a(Dimension) = value;
    }
};

template <class T, std::size_t N>
struct coordinate_system<hadoken::geometry::cartesian::point_base<T, N> >
{
    typedef boost::geometry::cs::cartesian type;
};

} // namespace traits

#endif // DOXYGEN_NO_TRAITS_SPECIALIZATIONS

}} // namespace boost::geometry



#endif // POINT_BASE_HPP
