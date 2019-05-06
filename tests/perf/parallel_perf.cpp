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


#include <future>
#include <mutex>
#include <set>
#include <thread>
#include <vector>

#include <boost/test/floating_point_comparison.hpp>


#include <boost/chrono.hpp>
#include <boost/random.hpp>

#include <hadoken/format/format.hpp>

#include <hadoken/parallel/algorithm.hpp>


using namespace boost::chrono;
namespace fmt = hadoken::format;

typedef system_clock::time_point tp;
typedef system_clock cl;


template <typename Value>
Value dummy_operation(Value& v) {
    Value res = v;
    for (std::size_t n = 0; n < 10; ++n) {
        res = std::pow(res, 99);
        res = std::sqrt(res);
        res = std::pow(res, 11);
        res = std::log(res);
    }
    return res;
}

template <typename ForEach>
std::size_t for_each_vector(std::size_t s_vector, std::size_t n_exec, const std::string& executor_name) {

    tp t1, t2;

    int val = 0;


    std::vector<double> values(s_vector, 0);

    std::size_t n = 0;
    for (auto& v : values) {
        v += n++;
    }

    auto fops = [](double& v) { v = dummy_operation<double>(v); };

    std::size_t cumulated_time = 0;

    for (std::size_t i = 0; i < n_exec; ++i) {

        t1 = cl::now();

        ForEach f;

        f.for_each(values.begin(), values.end(), fops);

        t2 = cl::now();

        cumulated_time += boost::chrono::duration_cast<microseconds>(t2 - t1).count();
    }

    val += int(std::accumulate(values.begin(), values.end(), 0.0, std::plus<double>()));

    std::cout << "" << executor_name << "; vector;  " << s_vector << "; " << double(cumulated_time) / n_exec << ";"
              << std::endl;

    return val;
}




template <typename ForEach>
std::size_t for_each_set(std::size_t s_set, std::size_t n_exec, const std::string& executor_name) {

    tp t1, t2;

    double val = 0;


    std::set<double> values;
    std::vector<double> res_values(s_set, 0);


    for (std::size_t i = 0; i < s_set; ++i) {
        values.insert(i);
    }

    auto fops = [&res_values](const double v) {
        double v_copy = v;
        res_values[v] = dummy_operation<double>(v_copy);
    };

    std::size_t cumulated_time = 0;

    for (std::size_t i = 0; i < n_exec; ++i) {

        t1 = cl::now();

        ForEach f;

        f.for_each(values.begin(), values.end(), fops);

        t2 = cl::now();

        cumulated_time += boost::chrono::duration_cast<microseconds>(t2 - t1).count();
    }

    val += int(std::accumulate(values.begin(), values.end(), 0.0, std::plus<double>()));

    std::cout << "" << executor_name << "; set;  " << s_set << "; " << double(cumulated_time) / n_exec << ";" << std::endl;

    return val;
}



struct std_for_each {

    template <typename Iter, typename Fun>
    void for_each(Iter iter1, Iter iter2, Fun fun) {
        std::for_each(iter1, iter2, fun);
    }
};



struct hadoken_parallel_for_each {

    template <typename Iter, typename Fun>
    void for_each(Iter iter1, Iter iter2, Fun fun) {
        using namespace hadoken;
        parallel::for_each(parallel::parallel_execution_policy(), iter1, iter2, fun);
    }
};



int main() {
    std::string parallel_mode = "";
#ifdef HADOKEN_PARALLEL_USE_OMP
    parallel_mode = "openmp";
#elif (defined HADOKEN_PARALLEL_USE_TBB)
    parallel_mode = "tbb";
#else
    parallel_mode = "pthread";
#endif
    const std::size_t ncore = std::thread::hardware_concurrency();


    const std::size_t n_exec = 100, max_size_vector = 200000000, limit_size_iter = 20000;
    std::size_t junk = 0;

    hadoken::format::scat(std::cout, "\n# test algorithms for vectors with ", n_exec, " iterations \n");
    hadoken::format::scat(std::cout, "theading; cores; executor; container; size; time; \n");


    std::size_t local_n_exec = n_exec;
    for (std::size_t i = 1; i < max_size_vector; i *= 10) {
        junk += for_each_vector<std_for_each>(i, local_n_exec, fmt::scat(parallel_mode, "; ", ncore, "; ", "serial_for_each"));

        junk += for_each_vector<hadoken_parallel_for_each>(i, local_n_exec,
                                                           fmt::scat(parallel_mode, "; ", ncore, "; ", "parallel_for_each"));

        if (i >= limit_size_iter) {
            local_n_exec /= 10;
            local_n_exec = std::max<decltype(local_n_exec)>(local_n_exec, 1);
        }
    }


    /*

    #ifndef HADOKEN_PARALLEL_USE_OMP
        hadoken::format::scat(std::cout, "\n# test algorithms for sets with ", n_exec, " iterations \n");

        junk += for_each_set<std_for_each>(small_size_vector, n_exec, "serial_for_each");

        junk += for_each_set<hadoken_parallel_for_each>(small_size_vector, n_exec, "parallel_for_each");

        junk += for_each_set<std_for_each>(avg_size_vector, n_exec, "serial_for_each");

        junk += for_each_set<hadoken_parallel_for_each>(avg_size_vector, n_exec, "parallel_for_each");

        junk += for_each_set<std_for_each>(avg_size_vector*10, n_exec/10, "serial_for_each");

        junk += for_each_set<hadoken_parallel_for_each>(avg_size_vector*10, n_exec/10, "parallel_for_each");

    #endif
    */
    std::cout << "# end junk " << junk << std::endl;
}
