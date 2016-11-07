#ifndef _HADOKEN_GEOMETRY_OBJECTS_HPP
#define _HADOKEN_GEOMETRY_OBJECTS_HPP

#include <boost/geometry/geometry.hpp>

#include <hadoken/geometry/objects/sphere.hpp>
#include <hadoken/geometry/objects/point_base.hpp>


namespace hadoken{

namespace geometry{

namespace cartesian {
    // namespace alias for internal use
    namespace bg =  boost::geometry;

    /// import boost geometry for existing algorithms
    using namespace boost::geometry;



    /// object models

    /// default point type
    template<typename CoordType, std::size_t N>
    using point = point_base<CoordType, N>;

    /// default point model with type specification
    template<typename CoordType>
    using point3 = point_base<CoordType, 3>;

    /// default point in double precision
    using point3d = point3<double>;

    ///default point in simple precision
    using point3f = point3<float>;

    /// vector representation
    template<typename CoordType>
    using vector3 = point3<CoordType>;

    /// default vector in double precision
    using vector3d = vector3<double>;

    ///default vector in simple precision
    using vector3f = vector3<float>;

    /// default multi_point model with type specification
    template<typename CoordType>
    using multi_point3 = model::multi_point<point3<CoordType>>;

    /// default point in double precision
    using multi_point3d = multi_point3<double>;

    ///default point in simple precision
    using multi_point3f = multi_point3<float>;

    /// default linestring
    template<typename CoordType>
    using linestring3 = model::linestring<point3<CoordType>>;

    /// default point in double precision
    using linestring3d = linestring3<double>;

    ///default point in simple precision
    using linestring3f = linestring3<float>;

    /// default sphere object
    template<typename CoordType>
    using sphere = sphere_base<point3<CoordType>, CoordType>;

    using sphere3d = sphere<double>;

    using sphere3f = sphere<float>;

    /// default circle object
    template<typename CoordType>
    using circle = circle_base<point3<CoordType>, vector3<CoordType>, CoordType>;

    using circle3d = circle<double>;

    using circle3f = circle<float>;///


} // cartesian


} // geometry

} // hadoken


#endif // OBJECTS_HPP
