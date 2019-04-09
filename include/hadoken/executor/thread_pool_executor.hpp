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
#pragma once

#include <atomic>
#include <thread>
#include <chrono>
#include <functional>
#include <vector>
#include <type_traits>
#include <bitset>


#include <hadoken/thread/future_helpers.hpp>
#include <hadoken/threading/std_thread_model.hpp>
#include <hadoken/containers/concurrent_queue.hpp>


namespace hadoken{


namespace details{

class worker_thread{
public:
    inline worker_thread(concurrent_queue<std::function<void ()>> & queue) :
                    _queue_ref(queue),
                    exec(),
                    finished(false) {
        std::thread runner([this]() { run();});

        exec.swap(runner);
    }


    inline ~worker_thread(){
        finished.store(true);
        if(exec.joinable()){
            exec.join();
        }
    }


    inline void run(){
        while(!finished){
            auto work_item = _queue_ref.try_pop(std::chrono::milliseconds(10));
            if(work_item){
                work_item.get()();
            }

        }
    }

private:
    worker_thread(const worker_thread &) = delete;

    concurrent_queue<std::function<void ()>> & _queue_ref;

    std::thread exec;

    std::atomic<bool> finished;
};


}

///
/// \brief Executor implementation for a simple thread
///
class thread_pool_executor : public std_thread_model{
public:

    enum class flags : std::size_t {
        complete_all_before_delete = 0
    };

    template<typename T>
    using future = std::future<T>;

    template<typename T>
    using promise = std::promise<T>;

    inline thread_pool_executor(std::size_t n_thread =0) :
        _flags(0),
        _work_queue(),
        _executors(){
        pthread_key_create(&_recursive_key, NULL);

        const std::size_t n_workers = (n_thread > 0) ? n_thread : (std::thread::hardware_concurrency());
        for(std::size_t i =0; i < n_workers; ++i){
            _executors.emplace_back( new details::worker_thread(_work_queue));
        }
    }

    inline ~thread_pool_executor(){
        pthread_key_delete(_recursive_key);
        if(_flags[static_cast<std::size_t>(flags::complete_all_before_delete)]){
            wait();
        }
    }

    inline void execute(std::function<void (void)> task){
        _work_queue.push(std::move(task));
    }

    template<typename Function>
    inline future<decltype(std::declval<Function>()())> twoway_execute(Function func){

        // if our current thread is not part of the pool
        // we execute in the pool
        // if it is already a pooled_thread, we do not to avoid deadlock
        if(pthread_getspecific(_recursive_key) == NULL){

            auto prom = std::make_shared<promise<decltype(std::declval<Function>()()) > >();
            auto future_result = prom->get_future();

            _work_queue.push([prom, func, this]() mutable -> void{
                pthread_setspecific(this->_recursive_key, (void*)1);
                try{
                    set_promise_from_result(*prom, func);
                } catch(...) {
                    try {
                        prom->set_exception(std::current_exception());
                    } catch(...) {
                        std::cerr << "error during set_exception in executor" << std::endl;
                    }
                }
            });
            return future_result;
        } else{
            return std::async(std::launch::deferred, [func]() {
                return func();
            });
        }
    }

    inline void set_flags(flags flag, bool value){
        _flags[static_cast<std::size_t>(flag)] = value;
    }

    inline bool get_flag(flags flag) const {
        return _flags[static_cast<std::size_t>(flag)];
    }

    inline void wait(){
        while(_work_queue.empty() == false){
            std::this_thread::yield();
        }
    }

private:
    std::bitset<32> _flags;
    concurrent_queue<std::function<void ()>> _work_queue;
    std::vector<std::unique_ptr<details::worker_thread> > _executors;
    pthread_key_t _recursive_key;

};


}

