#define BOOST_TEST_MODULE cryptoTests
#define BOOST_TEST_MAIN

#include <boost/test/included/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include <string>
#include <boost/array.hpp>
#include <hadoken/crypto/crypto.hpp>


BOOST_AUTO_TEST_CASE( sha1_basic_tests )
{
    std::string str("hello world");
    std::string hash_str("2aae6c35c94fcfb415dbe95f408b9ce91ee846ed");

    hadoken::sha1 sha_compute;

    sha_compute.process_block(str.c_str(), str.size());

    std::string res = sha_compute.to_string();

    BOOST_CHECK_EQUAL(hash_str, res);


    std::cout << "sha1_hello_world:" << res << "\n";

    // give a second try to check corruption protection
    res = sha_compute.to_string();

    BOOST_CHECK_EQUAL(hash_str, res);


}


BOOST_AUTO_TEST_CASE( sha1_basic_tests2 )
{
    std::string str("The quick brown fox jumps over the lazy cog");
    std::string hash_str("de9f2c7fd25e1b3afad3e85a0bd17d9b100db4b3");

    hadoken::sha1 sha_compute;

    sha_compute.process_block(str.c_str(), str.size());

    std::string res = sha_compute.to_string();

    BOOST_CHECK_EQUAL(hash_str, res);


}


BOOST_AUTO_TEST_CASE( hash_integer )
{
    int value = 42;

    {
        hadoken::sha1 sha_compute;
        std::string res = sha_compute.to_string();

        std::cout << "empty_hash " << res << "\n";
        BOOST_CHECK_EQUAL("da39a3ee5e6b4b0d3255bfef95601890afd80709", res);
    }

    {
        hadoken::sha1 sha_compute;

        sha_compute.process(boost::uint32_t(value));

        std::string res = sha_compute.to_string();


        std::cout << "integer_hash " << res << "\n";
        BOOST_CHECK_EQUAL("25f0c736f1fad0770bbb9a265ded159517c1e68c", res);
    }

}

