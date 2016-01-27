#define BOOST_TEST_MODULE genericTests
#define BOOST_TEST_MAIN

#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/array.hpp>
#include <boost/math/constants/constants.hpp>

#include <hadoken/geometry/geometry.hpp>
#include <hadoken/math/math_floating_point.hpp>
#include <hadoken/utility/range.hpp>

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



template<typename T, typename Mod, typename Check>
void  test_check_range(T vec, size_t partition, const Mod & modifier, const Check & checker){
    using namespace hadoken;

    typedef range<T> range_vec;

    range_vec my_range(vec.begin(), vec.end());


    BOOST_CHECK_EQUAL(vec.size(), my_range.size());

    std::vector< range_vec> ranges = my_range.split(partition);
    BOOST_CHECK_EQUAL(ranges.size(), partition);
    BOOST_CHECK(ranges[0].begin() == vec.begin());
    BOOST_CHECK(ranges[partition-1].end() == vec.end());


    // test validity of all iterators
    for(typename std::vector<range_vec>::iterator it = ranges.begin(); it != ranges.end(); ++it){
        std::for_each(it->begin(), it->end(), modifier);
    }

    std::for_each(vec.begin(), vec.end(), checker);

    // test sequence
    // no range should have a difference of more than 1 elem
    std::size_t min_elem = std::numeric_limits<std::size_t>::max(), max_elem = std::numeric_limits<std::size_t>::min();
    for(typename std::vector<range_vec>::iterator it = ranges.begin(); it < ranges.end(); ++it){
        const std::size_t diff = std::distance(it->begin(), it->end());
        min_elem = std::min(min_elem, diff);
        max_elem = std::min(max_elem, diff);
    }

    BOOST_CHECK_GE(min_elem+1,max_elem);

}

std::vector<int> generate_vector(size_t size_vec){
    std::vector<int> vec(size_vec);
    for(size_t i=0; i< vec.size(); ++i)
        vec[i] = i;
    return vec;
}


void modification_vector(int & i){
    i+= 100;
}

struct check_modification_vector{
    check_modification_vector() : count(0) {}

    void operator ()(int & i){
        BOOST_CHECK_EQUAL(i, count+100);
        count+=1;
    }

    int count;
};


std::map<int, int> generate_map(size_t size_vec){
    std::map<int, int> map;
    for(size_t i=0; i< size_vec; ++i)
        map.insert(std::make_pair(i,i));
    return map;
}


void modification_map(std::pair<int,int>  p){
    BOOST_CHECK_EQUAL(p.first, p.second);
}


struct check_modification_map{
    check_modification_map() : count(0) {}

    void operator ()(std::pair<int,int>  p){
        BOOST_CHECK_EQUAL(p.first, count);
        count+=1;
    }

    int count;
};

// reproduce rotate MVD2 for compat
BOOST_AUTO_TEST_CASE( range_simple_test_vector_int )
{

    {
        // perfect split
        check_modification_vector check;
        test_check_range< std::vector<int>, void (*)(int &), struct check_modification_vector >(generate_vector(800), 4, modification_vector, check);
    }

    {
        // under numbered elements
        check_modification_vector check;
        test_check_range< std::vector<int>, void (*)(int &), struct check_modification_vector  >(generate_vector(1000), 50000, modification_vector, check);
    }

    {
        // divide with rest
        check_modification_vector check;
        test_check_range< std::vector<int>, void (*)(int &), struct check_modification_vector >(generate_vector(700), 30, modification_vector, check);
    }


    {
        // check not random iterator in map structure
        check_modification_map check_map;
        test_check_range< std::map<int, int>, void (*)(std::pair<int,int>), check_modification_map >(generate_map(800), 4, modification_map, check_map);
    }

    {
        // check not random iterator with partition > size
        check_modification_map check_map;
        test_check_range< std::map<int, int>, void (*)(std::pair<int,int>), check_modification_map >(generate_map(1000), 50000, modification_map, check_map);
    }
}


