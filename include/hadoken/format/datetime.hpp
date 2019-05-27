#pragma once
/**
 * Copyright (c) 2019, Adrien Devresse <adrien.devresse@epfl.ch>
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


#include <chrono>
#include <hadoken/string/string_view.hpp>

namespace hadoken {



template<typename TimePoint>
inline std::string format_datetime(const TimePoint  & p, const std::string & fmt){
    using Clock = typename TimePoint::clock;


    std::array<char, 256> buffer_time;
    std::fill(buffer_time.begin(), buffer_time.end(), '\0');

    std::time_t t = Clock::to_time_t(p);
    struct tm * tmp_info = std::localtime(&t);

    ssize_t ret = strftime(buffer_time.data(), buffer_time.size() -1 , fmt.c_str(), tmp_info);
    if(ret <= 0){
        throw std::invalid_argument("invalid date/time formatting");
    }
    if(ret >= ssize_t(buffer_time.size() -1)){
        throw std::out_of_range("data/time format too large");
    }

    return std::string(buffer_time.data());
}



} // hadoken
