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
#pragma once

#include <cmath>
#include <limits>
#include <mutex>
#include <numeric>
#include <stdexcept>
#include <vector>


namespace hadoken {



template <typename Float, typename Mutex = std::mutex>
class logarithmic_histogram {
  public:
    using value_type = Float;
    using mutex_type = Mutex;

    logarithmic_histogram(value_type minimum, value_type maximum, value_type step_value)
        : _bins(), _min(minimum), _max(maximum), _step_value(step_value), _sum(0), _cardinality(0), _histo_lock() {

        if (_max <= _min) {
            throw std::logic_error("invalid minimum value >= maximum value for histogram");
        }

        if (_step_value <= 0) {
            throw std::logic_error("invalid step value for histogram");
        }

        const value_type full_range = _max - _min;

        const value_type n_bins = std::log(full_range) / std::log(_step_value);
        const std::size_t bounded_upper_n_bins_size = static_cast<std::size_t>(std::max<value_type>(0, n_bins)) + 1;
        _bins.resize(bounded_upper_n_bins_size, 0);
    }

    void add_value(value_type v) {
        const value_type absolute_value = std::max<value_type>(0, v - _min);
        const value_type position = std::log(absolute_value) / std::log(_step_value);
        const std::size_t bounded_position = static_cast<std::size_t>(
            std::min<value_type>(std::max<value_type>(0, position), static_cast<value_type>(_bins.size() - 1)));

        {
            std::lock_guard<std::mutex> _l(_histo_lock);
            _bins.at(bounded_position) += 1;
            _cardinality += 1;
            _sum += v;
        }
    }


    std::size_t size() const { return _bins.size(); }

    std::tuple<value_type, value_type, std::uint64_t> get_bin(std::size_t bin_position) const {

        if (bin_position >= _bins.size()) {
            throw std::out_of_range("bin position >= histogram size");
        }

        {
            std::lock_guard<mutex_type> _lock(_histo_lock);

            return std::make_tuple(_boundary_bin(bin_position), _boundary_bin(bin_position + 1), _bins.at(bin_position));
        }
    }

    std::size_t cardinality() const {
        std::lock_guard<mutex_type> _lock(_histo_lock);
        return _cardinality;
    }

    value_type sum() const {
        std::lock_guard<mutex_type> _lock(_histo_lock);
        return _sum;
    }

  private:
    std::vector<std::uint64_t> _bins;
    value_type _min, _max, _step_value, _sum;
    std::uint64_t _cardinality;

    mutable std::mutex _histo_lock;

    value_type _boundary_bin(const std::size_t pos) const {
        if (pos == 0) {
            return std::numeric_limits<value_type>::lowest();
        }
        if (pos >= size()) {
            return std::numeric_limits<value_type>::max();
        }
        return _min + std::pow(_step_value, static_cast<value_type>(pos));
    }
};




} // namespace hadoken
