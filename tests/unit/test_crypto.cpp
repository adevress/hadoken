#define BOOST_TEST_MODULE cryptoTests
#define BOOST_TEST_MAIN

#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include <string>
#include <boost/array.hpp>
#include <hadoken/crypto/crypto.hpp>


BOOST_AUTO_TEST_CASE( sha1_tests )
{
    std::string str("hello world");
    std::string hash_str("2aae6c35c94fcfb415dbe95f408b9ce91ee846ed");

    hadoken::sha1 sha_compute;

    sha_compute.process_block(str.c_str(), str.size());

    std::string res = sha_compute.to_string();

    BOOST_CHECK_EQUAL(hash_str, res);


    std::cout << "sha1_hello_world:" << res << "\n";
}
