/**
 * Copyright (C) 2016 Adrien Devresse
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
*/
#ifndef _HADOKEN_GEOMETRY_HPP_
#define _HADOKEN_GEOMETRY_HPP_


#include <cmath>
#include <boost/array.hpp>

namespace hadoken{

namespace geometry{


    /// square of distance between two points in 3D
    template<typename T>
    inline T distanceSquared3D(const T* point1, const T* point2){
        const T
           x = {point1[0] - point2[0]},
           y = {point1[1] - point2[1]},
           z = {point1[2] - point2[2]};
        return x*x + y*y + z*z;
    }

    /// square of distance between two points in 3D
    /// dim by dim
    template<typename T>
    inline T distanceSquared3D(const T x1, const T y1, const T z1,
                               const T x2, const T y2, const T z2){
        const T c1[] = { x1, y1, z1 };
        const T c2[] = { x2, y2, z2 };
        return distanceSquared3D(c1, c2);
    }

    /// distance between two points in 3D
    template<typename T>
    inline T distance3D(const T* point1, const T* point2){
        return std::sqrt(distanceSquared3D(point1, point2));

    }

    /// distance between two points in 3D
    /// dim by dim
    template<typename T>
    inline T distance3D(const T x1, const T y1, const T z1,
                               const T x2, const T y2, const T z2){
        const T c1[] = { x1, y1, z1 };
        const T c2[] = { x2, y2, z2 };
        return distance3D(c1, c2);
    }


    namespace quaternion{

        /// execute an hamilton product on two quaternions
        /// the quaternions have to be in order (x,y,z,w)
        /// @return product  result quaternion in order (x,y,z,w)
        template<typename QuadRes, typename Quad1, typename Quad2>
        inline QuadRes product(const Quad1 & q1, const Quad2 & q2, QuadRes res = QuadRes()){
            const typename QuadRes::value_type w1 = q1[3], x1 = q1[0], y1 = q1[1], z1 = q1[2];
            const typename QuadRes::value_type w2 = q2[3], x2 = q2[0], y2 = q2[1], z2 = q2[2];

            res[0] = w1*x2 + x1*w2 + y1*z2 - z1*y2;
            res[1] = w1*y2 - x1*z2 + y1*w2 + z1*x2;
            res[2] = w1*z2 + x1*y2 - y1*x2 + z1*w2;
            res[3] = w1*w2 - x1*x2 - y1*y2 - z1*z2;
            return res;
        }

        ///
        /// return the conjugate of a quaternion
        ///
        template<typename QuadRes, typename Quad1>
        inline QuadRes conjugate(const Quad1 & q1, QuadRes res = QuadRes()){
            for(int i=0; i <3;++i )
                res[i]= - q1[i];
            res[3] = q1[3];
            return res;
        }

    }



    /// rotate a point in a 3D space using a quaternion
    /// - quaternion can be any 1D array of size 4 in order (x,y,z,w )
    /// - point can be any 1D array of size 3 in order (x,y,z)
    template<typename ValueType, typename Quaternion, typename Point>
    inline void rotate(const Quaternion & q_rotation, Point & point){
        typedef boost::array<ValueType, 4> QuatType;
        using namespace quaternion;

        QuatType q_input;
        for(int i=0; i <3;++i)
            q_input[i] = point[i];
        q_input[3] = 0;

        const QuatType res = product<QuatType>(product<QuatType>(q_rotation, q_input), conjugate<QuatType>(q_rotation));
        for(int i =0; i < 3;++i)
            point[i] = res[i];
    }

}

}

#endif // _HADOKEN_GEOMETRY_HPP_
