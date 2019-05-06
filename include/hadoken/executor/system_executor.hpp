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


#include <future>

#include <hadoken/executor/thread_pool_executor.hpp>
#include <hadoken/utility/singleton.hpp>


namespace hadoken {


///
/// \brief Executor implementation for a simple thread
///
class system_executor {
  public:
    template <typename T>
    using future = std::future<T>;

    template <typename T>
    using promise = std::promise<T>;

    inline system_executor() { singleton<thread_pool_executor>::init(); }

    inline ~system_executor() {}

    inline void execute(std::function<void(void)> task) {
        singleton<thread_pool_executor>::instance().execute(std::move(task));
    }

    template <typename Function>
    inline future<decltype(std::declval<Function>()())> twoway_execute(Function func) {
        return singleton<thread_pool_executor>::instance().twoway_execute(func);
    }


  private:
    singleton<thread_pool_executor> _s;
};


} // namespace hadoken
