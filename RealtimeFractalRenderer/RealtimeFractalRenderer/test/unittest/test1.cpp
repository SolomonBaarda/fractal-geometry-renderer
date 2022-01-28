#define BOOST_TEST_MODULE MyTest1

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_1) 
{
    BOOST_CHECK_EQUAL(1, 0);

    printf("TEST FILE 1");
}

