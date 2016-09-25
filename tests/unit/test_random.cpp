/**
 * Copyright (c) 2016, Adrien Devresse <adrien.devresse@epfl.ch>
 * 
 * Boost Software License - Version 1.0 
 * 
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 * 
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
* 
*/
#define BOOST_TEST_MODULE randomTests
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>
#include <boost/mpl/list.hpp>
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
    hadoken::random_engine_mapper_32 engine_mapper(twister_engine_clone);

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



    const int seed = 424242;
    boost::random::mt11213b twister_engine_clone;
    hadoken::random_engine_mapper_32 engine_mapper(twister_engine_clone);
    engine_mapper.seed(seed);


    // create a derivation with a terrible seed init difference  1
    hadoken::random_engine_mapper_32 derivated_engine = engine_mapper.derivate(1);


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




BOOST_AUTO_TEST_CASE( determinism_derivate)
{
    const std::size_t n_vals = 1000;

    boost::random::uniform_int_distribution<std::size_t> dist;

    std::vector<std::size_t> origin_values, derivated_values, derivated_values_same, derivated_values_differ;
    origin_values.reserve(n_vals);


    const int seed = 1234;
    boost::random::mt11213b twister_engine_clone;
    hadoken::random_engine_mapper_32 engine_mapper(twister_engine_clone);
    engine_mapper.seed(seed);



    // create two derivation with the same key, they should behave in the same way
    hadoken::random_engine_mapper_32 derivated_engine = engine_mapper.derivate(42);
    hadoken::random_engine_mapper_32 derivated_engine_same = engine_mapper.derivate(42);
    
    // create a second one as double derivative as reference
    hadoken::random_engine_mapper_32 derivated_engine_differ = derivated_engine_same.derivate(43);

   

    // simple silly test to fullfill original twister
    // random generator vector
    for(std::size_t i =0; i < n_vals; ++i){
        unsigned int v1 = dist(engine_mapper);
        origin_values.push_back(v1);

        unsigned int v2= dist(derivated_engine);
        derivated_values.push_back(v2);

	unsigned int v3 = dist(derivated_engine_same);
	derivated_values_same.push_back(v3);

	unsigned int v4 = dist(derivated_engine_differ);
	derivated_values_differ.push_back(v4);

	
        BOOST_CHECK_NE(v1, v2);
	BOOST_CHECK_NE(v1, v3);
	BOOST_CHECK_NE(v1, v4);

	BOOST_CHECK_EQUAL(v2, v3);

	BOOST_CHECK_NE(v2, v4);
	BOOST_CHECK_NE(v2, v4);

	std::cout << "randum_num_diff: " << v1 << " " << v2 << " " << v3 << " " << v4 << "\n";
    }

}



BOOST_AUTO_TEST_CASE( derivate_counter_based_determinism)
{
    const std::size_t n_vals = 1000;

    boost::random::uniform_int_distribution<std::size_t> dist;

    std::vector<std::size_t> origin_values, derivated_values, derivated_values_same, derivated_values_differ;
    origin_values.reserve(n_vals);


    const int seed = 1234;
    hadoken::counter_engine<hadoken::threefry4x64> engine_counter;
    engine_counter.seed(seed);



    // create two derivation with the same key, they should behave in the same way
    hadoken::counter_engine<hadoken::threefry4x64>  derivated_engine = engine_counter.derivate(42);
    hadoken::counter_engine<hadoken::threefry4x64>  derivated_engine_same = engine_counter.derivate(42);

    // create a second one as double derivative as reference
    hadoken::counter_engine<hadoken::threefry4x64>  derivated_engine_differ = derivated_engine_same.derivate(43);



    // simple silly test to fullfill original twister
    // random generator vector
    for(std::size_t i =0; i < n_vals; ++i){
        unsigned int v1 = dist(engine_counter);
        origin_values.push_back(v1);

        unsigned int v2= dist(derivated_engine);
        derivated_values.push_back(v2);

    unsigned int v3 = dist(derivated_engine_same);
    derivated_values_same.push_back(v3);

    unsigned int v4 = dist(derivated_engine_differ);
    derivated_values_differ.push_back(v4);


        BOOST_CHECK_NE(v1, v2);
    BOOST_CHECK_NE(v1, v3);
    BOOST_CHECK_NE(v1, v4);

    BOOST_CHECK_EQUAL(v2, v3);

    BOOST_CHECK_NE(v2, v4);
    BOOST_CHECK_NE(v2, v4);

    std::cout << "randum_num_diff: " << v1 << " " << v2 << " " << v3 << " " << v4 << "\n";
    }
}


BOOST_AUTO_TEST_CASE( threefry_basic_32)
{

    std::size_t iter = 1001;
    std::size_t res =0;


    boost::random::uniform_int_distribution<boost::uint64_t> dist;

    hadoken::counter_engine<hadoken::threefry2x32> threefry_engine;

    for(std::size_t i =0; i < iter; ++i){
        const boost::uint64_t v = dist(threefry_engine);
        std::cout << "threefry_value: " << i << " " << v << "\n";

        switch(i){
            case 0:{
                BOOST_CHECK_EQUAL(v, static_cast<boost::uint64_t>(5804853139360071474ULL));
            }
            break;
            case 10:{
                BOOST_CHECK_EQUAL(v, static_cast<boost::uint64_t>(7053101028938294423ULL));
            }
            break;
            case 100:{
                BOOST_CHECK_EQUAL(v, static_cast<boost::uint64_t>(12524329080125684850ULL));
            }
            break;
            case 1000:{
                BOOST_CHECK_EQUAL(v, static_cast<boost::uint64_t>(8534186729197965889ULL));
            }
            break;
            default:{

            }

        }
        res += v;
    }



}


BOOST_AUTO_TEST_CASE( threefry_basic_64)
{

    std::size_t iter = 1001;
    std::size_t res =0;


    boost::random::uniform_int_distribution<boost::uint64_t> dist;

    hadoken::counter_engine<hadoken::threefry4x64> threefry_engine;

    for(std::size_t i =0; i < iter; ++i){
        const boost::uint64_t v = dist(threefry_engine);
        std::cout << "threefry_value: " << i << " " << v << "\n";

        switch(i){
            case 0:{
                BOOST_CHECK_EQUAL(v, static_cast<boost::uint64_t>(12180738260140386403ULL));
            }
            break;
            case 10:{
                BOOST_CHECK_EQUAL(v, static_cast<boost::uint64_t>(15340443714481834838ULL));
            }
            break;
            case 100:{
                BOOST_CHECK_EQUAL(v, static_cast<boost::uint64_t>(6790623761552653118ULL));
            }
            break;
            case 1000:{
                BOOST_CHECK_EQUAL(v, static_cast<boost::uint64_t>(16214960538048011ULL));
            }
            break;
            default:{

            }

        }
        res += v;
    }



}


typedef boost::mpl::list<hadoken::threefry2x32,
                        hadoken::threefry4x32,
                        hadoken::threefry2x64,
                        hadoken::threefry4x64> threefry_types;

BOOST_AUTO_TEST_CASE_TEMPLATE( threefry_distribute, T, threefry_types )
{
        boost::random::uniform_int_distribution<boost::uint64_t> dist100(0, 100);

        hadoken::counter_engine<T> threefry_engine;

        const std::size_t n_normalize = 100000;
        std::size_t res = 0;
        for(std::size_t i =0; i < n_normalize; ++i){
            res += dist100(threefry_engine);
        }

        const std::size_t mean = res/n_normalize;
        std::cout << "n_normalize_threefry: " << mean << "\n";
        BOOST_CHECK_GE(mean, 49);
        BOOST_CHECK_LE(mean, 51);

}




BOOST_AUTO_TEST_CASE_TEMPLATE( engine_discard, T, threefry_types )
{

    // basic consistency test
    {

        hadoken::counter_engine<T> threefry_engine, threefry_engine_origin, threefry_engine_clone;

        threefry_engine.seed(42);
        threefry_engine_origin.seed(42);
        threefry_engine_clone.seed(42);

        threefry_engine_clone.discard(100);
        threefry_engine_origin.discard(0);


        typename hadoken::counter_engine<T>::result_type res = threefry_engine(),
                res_origin = threefry_engine_origin(), res_clone = threefry_engine_clone();


        BOOST_CHECK_NE(res, res_clone);
        BOOST_CHECK_EQUAL(res, res_origin);
    }


    // discard and verify result with 1 by 1 inc
    {
        hadoken::counter_engine<T> threefry_engine, threefry_engine_clone;
        const std::size_t inc_n = 1000;

        threefry_engine.seed(42);
        threefry_engine_clone.seed(42);

        std::size_t junk =0;

        for(std::size_t i =0; i < inc_n; ++i){
            junk += threefry_engine();
        }

        threefry_engine_clone.discard(inc_n);

        BOOST_CHECK_EQUAL(threefry_engine(), threefry_engine_clone());
    }



    //  same again but with prime number, to test discard by undivisible (size(elem))
    {
        hadoken::counter_engine<T> threefry_engine, threefry_engine_clone;
        const std::size_t inc_n = 1181;

        threefry_engine.seed(42);
        threefry_engine_clone.seed(42);

        std::size_t junk =0;

        for(std::size_t i =0; i < inc_n; ++i){
            junk += threefry_engine();
        }

        threefry_engine_clone.discard(inc_n);

        BOOST_CHECK_EQUAL(threefry_engine(), threefry_engine_clone());
    }

    // test increment that overflow a normal 32 buffers
    {

        hadoken::counter_engine<T> threefry_engine, threefry_engine_clone;

        threefry_engine.seed(42);
        threefry_engine_clone.seed(42);


        typename hadoken::counter_engine<T>::ctr_type ctr, ctr_clone;
        ctr = threefry_engine.getcounter();
        ctr_clone = threefry_engine_clone.getcounter();

        BOOST_CHECK_EQUAL_COLLECTIONS(ctr.begin(), ctr.end(), ctr_clone.begin(), ctr_clone.end());

        // discard and verify result with 1 by 1 inc
        boost::uintmax_t max_val = std::numeric_limits<boost::uintmax_t>::max();

        const std::size_t factor = 50;
        boost::uintmax_t small_inc = max_val / 16 / factor;
        boost::uintmax_t big_inc = small_inc * factor;


        BOOST_CHECK_EQUAL(big_inc, small_inc*factor);


        for(std::size_t i =0; i < factor; ++i){
            threefry_engine.discard(small_inc);
        }
        threefry_engine_clone.discard(big_inc);

        ctr = threefry_engine.getcounter();
        ctr_clone = threefry_engine_clone.getcounter();

        BOOST_CHECK_EQUAL_COLLECTIONS(ctr.begin(), ctr.end(), ctr_clone.begin(), ctr_clone.end());

        BOOST_CHECK_EQUAL(threefry_engine(), threefry_engine_clone());
    }





}

