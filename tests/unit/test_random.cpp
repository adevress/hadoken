#define BOOST_TEST_MODULE randomTests
#define BOOST_TEST_MAIN

#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>


#include <boost/random.hpp>

#include <hadoken/random/random.hpp>


BOOST_AUTO_TEST_CASE( simple_random_tests )
{
    const std::size_t n_vals = 1000;

    boost::random::mt11213b twister_engine;

    boost::random::uniform_int_distribution<int> dist(0, n_vals);

    std::vector<int> origin_values;
    origin_values.reserve(n_vals);

    // simple silly test to fullfill original twister
    // random generator vector
    for(std::size_t i =0; i < n_vals; ++i){
        const int v = dist(twister_engine);
        origin_values.push_back(v);
        BOOST_CHECK_GE(v, 0);
        BOOST_CHECK_LE(v, n_vals);
    }

    boost::random::mt11213b twister_engine_clone;
    hadoken::random_engine_mapper engine_mapper(twister_engine_clone);

    std::vector<int> mapper_values;
    mapper_values.reserve(n_vals);

    // simple silly test to fullfill original twister
    // random generator vector
    for(std::size_t i =0; i < n_vals; ++i){
        const int v = dist(engine_mapper);
        mapper_values.push_back(v);
        BOOST_CHECK_GE(v, 0);
        BOOST_CHECK_LE(v, n_vals);
    }

    BOOST_CHECK_EQUAL_COLLECTIONS(origin_values.begin(), origin_values.end(),
                                  mapper_values.begin(), mapper_values.end());

}



BOOST_AUTO_TEST_CASE( simple_derivate)
{
    const std::size_t n_vals = 1000;

    boost::random::uniform_int_distribution<std::size_t> dist;

    std::vector<std::size_t> origin_values, derivated_values;
    origin_values.reserve(n_vals);



    const int seed = 0;
    boost::random::mt11213b twister_engine_clone;
    hadoken::random_engine_mapper engine_mapper(twister_engine_clone);
    engine_mapper.seed(seed);


    // create a derivation with a terrible seed init difference  1
    hadoken::random_engine_mapper derivated_engine = engine_mapper.derivate(1);


    // simple silly test to fullfill original twister
    // random generator vector
    for(std::size_t i =0; i < n_vals; ++i){
        unsigned int v1 = dist(engine_mapper);
        origin_values.push_back(v1);

        unsigned int v2= dist(derivated_engine);
        derivated_values.push_back(v2);

        std::cout << "random_num_twins: " << v1 << " " << v2 << std::endl;

        BOOST_CHECK_NE(v1, v2);
    }

}
