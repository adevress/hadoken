#define BOOST_TEST_MODULE genericTests
#define BOOST_TEST_MAIN

#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/array.hpp>
#include <boost/math/constants/constants.hpp>

#include <hadoken/geometry/geometry.hpp>
#include <hadoken/math/math_floating_point.hpp>

BOOST_AUTO_TEST_CASE( simple_rotation_tests )
{

    using namespace hadoken::geometry;
    using namespace hadoken::geometry::quaternion;
    using namespace hadoken::math;

    {
        // print eps for debugging on some paltforms
        std::cout <<  "dbl ep: " << std::numeric_limits<double>::epsilon()  << "\n";
        // rotate of pi/2 a point on z axis 2D
        const double angle = boost::math::constants::pi<double>()/2;
        double rotation_quaternion[] = { cos(angle/2), 0, 0, sin(angle/2) };

        double pos[] = { 2, 0, 0};
        rotate<double>(rotation_quaternion, pos);


        double res[] = {0, 2, 0};
        for(int i=0; i < 3; ++i){
            std::cout << "pos["<< i << "]: "<< pos[i] <<" \n";
            std::cout << "res["<< i << "]: "<< res[i] <<" \n";
            BOOST_CHECK( almost_equal(pos[i], res[i]));
        }

    }

    {
        // rotate of pi/4 a point on y axis 2D
        const double angle = boost::math::constants::pi<double>()/4;
        double rotation_quaternion[] = { cos(angle/2), 0, sin(angle/2),0 };

        double pos[] = { 3, 0, 0};
        rotate<double>(rotation_quaternion, pos);

        double res[] = {3*cos(angle),0, -3*sin(angle)};
        for(int i=0; i < 3; ++i){
            std::cout << "pos["<< i << "]: "<< pos[i] <<" \n";
            std::cout << "res["<< i << "]: "<< res[i] <<" \n";
            BOOST_CHECK( almost_equal(pos[i], res[i]));
        }


    }


}


// reproduce rotate MVD2 for compat
BOOST_AUTO_TEST_CASE( rotation_mvd_compat_test )
{
    using namespace hadoken::geometry;
    using namespace hadoken::geometry::quaternion;
    using namespace hadoken::math;

    const double translation_xyz[] = { 40.821401, 1986.506637, 10.788424 };
    const double y_angle_deg = -1.146572;

    const double xyz[] = { -2.90056, -18.005, 5.62 };

    boost::array<float, 3> old_res, res;


    const double DEG_RAD = 0.017453292;

    // legacy MVD2 compute single Y axe rotation
    {
            std::copy(xyz, xyz+3, old_res.begin());

            double angleSin = sin( DEG_RAD*y_angle_deg );
            double angleCos = cos( DEG_RAD*y_angle_deg );

            float tempX = xyz[2]*angleSin + xyz[0]*angleCos;
            float tempZ =  xyz[2]*angleCos - xyz[0]*angleSin;

            old_res[0] = tempX+ translation_xyz[0];
            old_res[1] += translation_xyz[1];
            old_res[2] = tempZ+ translation_xyz[2];
    }


    // quaternion rotation based
    {
            const double deg_rad_r = boost::math::constants::pi<double>()/180;

            boost::array<double, 4> quat;
            std::fill(quat.begin(), quat.end(), 0);
            quat[0] = cos( deg_rad_r*y_angle_deg/2 );
            quat[2] = sin(deg_rad_r*y_angle_deg/2 );

            std::copy(xyz, xyz+3, res.begin());
            rotate<double>(quat, res);

            for(int i=0; i < 3; ++i){
                res[i] += translation_xyz[i];
            }
    }

    for(int i =0; i < 3; ++i){
        std::cout << i << " :" << res[i] << " " << old_res[i] << " \n";
        BOOST_CHECK( almost_equal(old_res[i], res[i]));
    }

}



