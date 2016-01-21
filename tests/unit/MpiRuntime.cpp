#define BOOST_TEST_MODULE mpiTests
#define BOOST_TEST_MAIN

#include <boost/test/included/unit_test.hpp>

#include "functionalizer/utils/mpiRuntime.h"

int argc = boost::unit_test::framework::master_test_suite().argc;
char ** argv = boost::unit_test::framework::master_test_suite().argv;



boost::scoped_ptr<MpiRuntime> runtime;


MpiRuntime & getRuntime(){
    if(!runtime){
        runtime.reset(new MpiRuntime());
    }
    return *runtime;
}



BOOST_AUTO_TEST_CASE( mpiTests )
{

    MpiRuntime::setArgv(&argc, &argv);
    const int rank = getRuntime().rank();
    const int size = getRuntime().size();

    BOOST_CHECK(rank >=0);
    BOOST_CHECK(size > 0);
    BOOST_CHECK(rank < size);

    std::cout << " rank:" << rank << " size:" << size;

    getRuntime().syncBarrier();

}


BOOST_AUTO_TEST_CASE( mpiMax)
{

    MpiRuntime::setArgv(&argc, &argv);
    const int rank = getRuntime().rank();
    const int size = getRuntime().size();

    int proc_number = (rank+1)*10;
    int max_proc_number = 0;

    max_proc_number = getRuntime().allMax(proc_number);

    BOOST_CHECK(max_proc_number == (size)*10);




}


BOOST_AUTO_TEST_CASE( mpiSum)
{

    MpiRuntime::setArgv(&argc, &argv);

    const int rank = getRuntime().rank();
    const int size = getRuntime().size();

    int proc_number = (rank+1)*10;
    int sum_proc_number=0;
    for(int i =1; i < getRuntime().size()+1; ++i)
        sum_proc_number+=i*10;

    int sum_all = getRuntime().allSum(proc_number);


    size_t sum_size = getRuntime().allSum(static_cast<size_t>(size));


    BOOST_CHECK(sum_proc_number == sum_all);
    BOOST_CHECK(sum_size == static_cast<size_t>(size)* static_cast<size_t>(size));


}
