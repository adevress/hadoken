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
#ifndef _HADOKEN_STRING_ALGO_
#define _HADOKEN_STRING_ALGO_

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include <hadoken/string/string_view.hpp>



namespace hadoken {

namespace string {


/////////////////////////////
///
/// \brief split string from a vector of delimited token
///
/// Parse str and split the string on each char contained in the delimiter string
/// return a vector of string
///
/// \param str
/// \param delimiter
/// \param callback for every fragment
///
template <typename Func>
inline void split_string_view(const std::string& str, const std::string& delimiter, Func fragment_callback) {
    std::string::const_iterator it_prev, it_cur;
    for (it_prev = it_cur = str.begin(); it_cur < str.end(); ((it_prev != str.end()) ? (it_prev++) : (it_prev))) {
        it_cur = std::find_first_of(it_prev, str.end(), delimiter.begin(), delimiter.end());
        if (it_prev != it_cur) {
            fragment_callback(hadoken::make_string_view(it_prev, it_cur));
        }
        it_prev = it_cur;
    }
}



/////////////////////////////
///
/// \brief split string from a vector of delimited token
///
/// Parse str and split the string on each char contained in the delimiter string
/// return a vector of string
///
/// \param str
/// \param delimiter
/// \return vector of delimited string
///
inline std::vector<std::string> split_string(const std::string& str, const std::string& delimiter) {
    std::vector<std::string> res;
    split_string_view(str, delimiter, [&](hadoken::string_view fragment) { res.emplace_back(to_string(fragment)); });
    return res;
}

[[deprecated]] inline std::vector<std::string> tokenize(const std::string& str, const std::string& delimiter) {
    return split_string(str, delimiter);
}



} // namespace string

} // namespace hadoken

#endif // STRINGUTILS_H
