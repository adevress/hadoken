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
#ifndef HADOKEN_URI_HPP
#define HADOKEN_URI_HPP

#include <string>
#include <tuple>
#include <vector>

#include <hadoken/string/string_view.hpp>


namespace hadoken {

class uri{
public:
    enum class state{
        valid,
        invalid,

        // internal
        scheme,
        authority_delim,
        userinfo,
        host,
        port,
        path,
        query,
        fragment
    };

    typedef std::string::iterator iterator_type;
    typedef std::tuple<iterator_type, iterator_type> uri_view;

    typedef std::tuple<std::string, std::string> query_pair;

    inline explicit uri(std::string uri_string);
    inline explicit uri(const uri &) = default;
    inline explicit uri(uri &&) = default;
    inline ~uri();

    inline bool is_valid() const;

    inline std::string get_uri() const;

    inline std::string get_scheme() const;

    inline std::string get_userinfo() const;

    inline std::string get_host() const;

    inline int get_port() const;

    inline std::string get_path() const;

    inline std::string get_query() const;

    inline std::string get_fragment() const;

private:
    std::string _str_uri;
    state _state;
    uri_view _scheme, _userinfo, _host, _port, _path, _query, _fragment;


    inline int parse_uri(iterator_type begin, iterator_type last,
                          iterator_type end);
};


std::string percent_encode(hadoken::string_view decoded_origin);


std::string percent_decode(hadoken::string_view encoded_string);

} //hadoken


#include "impl/uri_impl.hpp"


#endif // URI_HPP
