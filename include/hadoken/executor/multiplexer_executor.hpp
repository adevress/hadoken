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

#include <thread>
#include <mutex>
#include <future>
#include <functional>
#include <unordered_map>

namespace hadoken {


template<typename Key, typename RetType>
class multiplexer{
public:
    using result_type = RetType;
    using key_type = Key;
    using work_item = typename std::function<result_type ()>;

    inline explicit multiplexer(){}

    inline std::shared_future<result_type> execute(const key_type & key,
                                                   work_item && item){
        // try to fetch
        {
            std::unique_lock<std::mutex> guard(_lock);
            auto shared_fut_it = _map.find(key);
            if(shared_fut_it != _map.end()){
                return shared_fut_it->second;
            }else{
                std::promise<result_type> prom;
                std::shared_future<result_type> sfut  = prom.get_future().share();
                _map[key] = sfut;

                guard.unlock();
                internal_execute(prom, std::move(item));
                guard.lock();
                _map.erase(key);
                return sfut;
            }
        }

    }

private:
    void internal_execute(std::promise<result_type> & p, work_item item) noexcept{
        try{
            p.set_value(item());
        }catch(...){
            try {
                p.set_exception(std::current_exception());
            } catch(...) {}
        }
    }

    std::mutex _lock;
    std::unordered_map<key_type, std::shared_future<result_type>> _map;

};


} // namespace hadoken