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




//#include <boost/test/floating_point_comparison.hpp>


//#include <boost/random.hpp>
//#include <boost/chrono.hpp>

#include <chrono>
#include <random>
#include <iostream>

#include <hadoken/config/platform_config.hpp>
#include <hadoken/random/counter_engine.hpp>
#include <hadoken/random/threefry.hpp>

#include  <cuda.h>
#include  <curand_kernel.h>


typedef  std::chrono::system_clock::time_point tp;
typedef  std::chrono::system_clock cl;


std::size_t test_random_mersenne_twister(std::size_t iter){



    std::size_t res =0;

    tp t1, t2;

    std::uniform_int_distribution<std::size_t> dist;
    std::mt19937 twister_engine;

    t1 = cl::now();

    for(std::size_t i =0; i < iter; ++i){
        res += dist(twister_engine);
    }


    t2 = cl::now();

    std::cout << "mersenne_twister_host: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 -t1).count() << std::endl;
    return res;

}


std::size_t test_random_threefry(std::size_t iter){

    std::size_t res =0;

    tp t1, t2;

    std::uniform_int_distribution<std::size_t> dist;

    hadoken::counter_engine<hadoken::threefry4x64> threefry_engine;

    t1 = cl::now();

    for(std::size_t i =0; i < iter; ++i){
        res += dist(threefry_engine);
    }


    t2 = cl::now();

    std::cout << "threefry_host: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 -t1).count() << std::endl;
    return res;

}

#ifdef HADOKEN_COMPILER_IS_NVCC


__global__
void kernel_rand_device_hadoken(std::size_t iter, std::size_t * res, std::size_t* canary){


//    thrust::uniform_int_distribution<int> dist;
 //   std::uniform_int_distribution<std::size_t> dist;

//
    typedef hadoken::counter_engine<hadoken::threefry4x64> rengine_t;


    std::size_t start_pos =  (blockIdx.x * blockDim.x + threadIdx.x) * iter;

    for(std::size_t i = start_pos; i < iter; ++i){

        
        hadoken::counter_engine<hadoken::threefry4x64> threefry_engine;
        threefry_engine.seed(i);
        res[start_pos+i] = threefry_engine();
    }

    *canary = iter;


}

std::size_t test_random_threefry_device_hadoken(std::size_t iter){

    std::size_t res =0, c=0;

    tp t1, t2;


    t1 = cl::now();

    std::size_t *tmp_res, *canary;

    cudaMallocManaged( &tmp_res, sizeof(std::size_t) * iter);
    cudaMallocManaged( &canary, sizeof(std::size_t));

    std::size_t piter = iter / 256 /256 ;

    kernel_rand_device_hadoken<<<256, 256>>>(piter, tmp_res, canary);
    cudaDeviceSynchronize();

    c = *canary;

    cudaFree( canary );

    t2 = cl::now();

    std::cout << "threefry device: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 -t1).count() << " and canary " << c << std::endl;
    return res;

}


#endif



#ifdef HADOKEN_COMPILER_IS_NVCC


__global__  void  setup_kernel(curandState *state, std::size_t pos)
{
    curand_init (1234 , pos, 0, &state[pos]);
}

__global__  void  generate_kernel_curand(std::size_t iter, curandState *state , std::size_t* res,
std::size_t* canary)
{
    
    std::size_t start_pos =  (blockIdx.x * blockDim.x + threadIdx.x) * iter;
    
    /* Copy  state  to local  memory  for  efficiency  */

    /*  Generate  pseudo -random  unsigned  ints */
    for(int n = 0; n < iter; n++) {
        std::size_t pos = n+start_pos;
        curandState  localState = state[start_pos];        
        res[pos] = curand (& localState);
    }
    
    *canary = iter;
}


std::size_t test_random_cuda_device_curand(std::size_t iter){

    std::size_t c, res;
    tp t1, t2;


    t1 = cl::now();

    std::size_t *tmp_res, *canary;
    curandState * state;

    cudaMallocManaged( &state, sizeof(curandState) * iter);
    cudaMallocManaged( &tmp_res, sizeof(std::size_t) * iter);
    cudaMallocManaged( &canary, sizeof(std::size_t));

    std::size_t piter = iter / 256 /256 ;

    setup_kernel<<<256, 256>>>(state, 256*256);
    generate_kernel_curand<<<256, 256>>>(piter, state, tmp_res, canary);
    cudaDeviceSynchronize();

    c = *canary;

    cudaFree( canary );

    t2 = cl::now();
    
    res = tmp_res[0];

    std::cout << "curand device: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 -t1).count() << " and canary " << c << std::endl;
    return res;
}


#endif

std::size_t test_random_threefry_block_fake(std::size_t iter){

    std::size_t res =0;

    tp t1, t2;

    std::uniform_int_distribution<std::size_t> dist;

    hadoken::counter_engine<hadoken::threefry4x64> threefry_engine;

    t1 = cl::now();

    const std::size_t size_block = sizeof(typename hadoken::counter_engine<hadoken::threefry4x64>::ctr_type) / sizeof(std::size_t);

    for(std::size_t i =0; i < (iter/size_block); ++i){
        auto block = threefry_engine.generate_block();
        for(std::size_t i = 0; i < size_block; ++i){
            res += block[0];
        }
    }


    t2 = cl::now();

    std::cout << "threefry block gen host: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 -t1).count() << std::endl;
    return res;

}


int main(){

    const std::size_t n_exec = 1 << 26;
    std::size_t junk=0;

    std::cout << "generation of " << n_exec << " random numbers" << std::endl;

    junk += test_random_mersenne_twister(n_exec);

    junk += test_random_threefry(n_exec);


    junk += test_random_threefry_block_fake(n_exec);

#ifdef HADOKEN_COMPILER_IS_NVCC
    junk += test_random_threefry_device_hadoken(n_exec);
    
    junk += test_random_cuda_device_curand(n_exec);
#endif

    std::cout << "end junk " << junk << std::endl;

}
