#ifndef HADOKEN_URI_IMPL_HPP
#define HADOKEN_URI_IMPL_HPP

#include <algorithm>
#include <locale>
#include <tuple>
#include <string>
#include <cstring>

#include <hadoken/format/format.hpp>

#ifndef HADOKEN_URI_HPP
#include "../uri.hpp"
#endif

namespace hadoken {

namespace {

    inline const char* rfc3986_gen_delims_chars(){
        return (":" "/"  "?"  "#"  "["  "]"  "@") ;
    }

    inline const char* rfc3986_sub_delims_chars(){
        return ("!"  "$"  "&"  "'"  "("  ")"
         "*"  "+"  ","  ";"  "=");
    }

    inline const char* rfc_unreserved_part_chars(){
        return ("-"  "."  "_" "~");
    }

    inline const char* rfc_scheme_parts_chars(){
        return "+" "-" "." ;
    }

    inline bool is_rfc3986_reserved_char(char c){
        return (std::strchr(rfc3986_gen_delims_chars(), c) != NULL
                || std::strchr(rfc3986_sub_delims_chars(), c) != NULL );
    }

    inline bool is_rfc3986_unreserved_char(char c){
        return (std::isalnum(c) || std::strchr(rfc_unreserved_part_chars(), c) != NULL);
    }


    inline bool is_rfc3986_scheme_char(char c){
        return (std::isalnum(c) || std::strchr(rfc_unreserved_part_chars(), c) != NULL);
    }


    inline std::string string_from_view(const uri::uri_view & v){
        auto begin = std::get<0>(v);
        auto end = std::get<1>(v);

        return std::string(begin, end);
    }

    inline void check_validity(const std::string & uri, uri::state validity){
        if(validity != uri::state::valid){
            throw std::invalid_argument(format::scat("Invalid URI ", uri));
        }
    }

    inline uri::uri_view make_view(uri::iterator_type begin, uri::iterator_type end){
        return std::make_tuple(begin, end);
    }

    inline uri::uri_view make_empty_view(std::string  & str){
         return std::make_tuple<uri::iterator_type, uri::iterator_type>(str.end(), str.end());
    }

}

uri::uri(std::string uri_string) :
    _str_uri(std::move(uri_string)),
    _state(state::invalid),
    _scheme(make_empty_view(_str_uri)),
    _userinfo(make_empty_view(_str_uri)),
    _host(make_empty_view(_str_uri)),
    _port(make_empty_view(_str_uri)),
    _path(make_empty_view(_str_uri)),
    _query(make_empty_view(_str_uri)),
    _fragment(make_empty_view(_str_uri))
{
    parse_uri(_str_uri.begin(), _str_uri.begin(), _str_uri.end(), state::scheme);
}

uri::~uri(){

}


bool uri::is_valid() const{
    return _state == state::valid;
}

std::string uri::get_uri() const{
    check_validity(_str_uri, _state);
    return _str_uri;
}

std::string uri::get_scheme() const{
    check_validity(_str_uri, _state);
    return string_from_view(_scheme);
}



std::string uri::get_userinfo() const{
    check_validity(_str_uri, _state);
    return string_from_view(_userinfo);
}



std::string uri::get_host() const{
    check_validity(_str_uri, _state);
    return string_from_view(_host);
}

int uri::get_port() const{
    check_validity(_str_uri, _state);
    if(std::get<0>(_port) == std::get<1>(_port)){
        return 0;
    }
    return std::stoi(string_from_view(_port));
}

std::string uri::get_path() const{
    check_validity(_str_uri, _state);
    return string_from_view(_path);
}

std::string uri::get_query() const{
    check_validity(_str_uri, _state);
    return string_from_view(_query);
}

std::string uri::get_fragment() const{
    check_validity(_str_uri, _state);
    return string_from_view(_fragment);
}


int uri::parse_uri(iterator_type begin, iterator_type last, iterator_type end, state pstate){
    const char delim_query = '?';
    const char delim_fragment = '#';
    const char userinfo_delim = '@';

    if(pstate == state::invalid){
        _state = state::invalid;
        return -1;
    }

    if(pstate == state::valid){
        _state = state::valid;
        return 0;
    }


    if(last >= end && ( pstate == state::scheme)){
        return parse_uri(end, end, end, state::invalid);
    }

    // parse scheme
    if(pstate == state::scheme){
        if( is_rfc3986_scheme_char(*last)){
            return parse_uri(begin, last+1, end, state::scheme);
        }
        if( *last == ':'){ // normal scheme case
            _scheme = make_view(begin, last);
            return parse_uri(last+1, last+1, end, state::authority_delim);
        }
        return parse_uri(end, end, end, state::invalid);
    }

    // parse authority delimiter "://"
    if(pstate == state::authority_delim){
        if( (last+1) < end && *last == '/' && *(last+1) == '/' ){
           return parse_uri(begin+2, last +2, end, state::userinfo);
        }
        return parse_uri(begin, last, end, state::path);
    }

    // parse userinfo if any
    if(pstate == state::userinfo){
        auto it = std::find(begin, end, userinfo_delim);
        if( it == end){
            _userinfo = make_view(end, end);
            return parse_uri(begin, begin, end, state::host);
         } else {
            _userinfo = make_view(begin, it);
            return parse_uri(it+1, it+1, end, state::host);
        }
    }

    // parse host
    if(pstate == state::host){
        // TODO: handle IPv6 URI [::]
        if(*last == ':'){
            _host = make_view(begin, last);
            return parse_uri(last+1, last+1, end, state::port);
        }
        if(*last == '/' || last == end){
            _host = make_view(begin, last);
            return parse_uri(last, last, end, state::path);
        }
        return parse_uri(begin, last+1, end, state::host);
    }

    // parse port
    if(pstate == state::port){
        if(std::isdigit(*last)){
            return parse_uri(begin, last+1, end, state::port);
        }
        _port = make_view(begin, last);
        return parse_uri(last, last, end, state::path);
    }

    // parse path
    if(pstate == state::path){
        if(last >= end){
            _path = make_view(begin, end);
            return parse_uri(end, end, end, state::valid);
        }else if( *last == delim_query){
            _path = make_view(begin, last);
            return parse_uri(last+1, last+1, end, state::query);
        }else if (*last == delim_fragment){
            _path = make_view(begin, last);
             return parse_uri(last+1, last+1, end, state::fragment);
        }
        return parse_uri(begin, last+1, end, state::path);
    }

    // parse query
    if(pstate == state::query){
        if(last >= end){
            _query = make_view(begin, end);
            return parse_uri(end, end, end, state::valid);
        }else if( *last == delim_fragment){
            _query = make_view(begin, last);
             return parse_uri(last+1, last+1, end, state::fragment);
        }
        return parse_uri(begin, last+1, end, state::query);
    }

    // parse fragment
    if(pstate == state::fragment){
        if(last >= end){
            _fragment = make_view(begin, end);
            return parse_uri(end, end, end, state::valid);
        }
        return parse_uri(begin, last+1, end, state::fragment);
    }


    _state = state::invalid;
    return -1;
}






} //hadoken

#endif // URI_IMPL_HPP
