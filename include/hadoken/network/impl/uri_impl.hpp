#ifndef HADOKEN_URI_IMPL_HPP
#define HADOKEN_URI_IMPL_HPP

#include <algorithm>
#include <locale>
#include <tuple>
#include <string>
#include <cstring>

#include <hadoken/format/format.hpp>
#include <hadoken/string/string_view.hpp>

#ifndef HADOKEN_URI_HPP
#include "../uri.hpp"
#endif

namespace hadoken {

namespace details{

    constexpr char hexmap_uppercase[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                           '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};


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

    inline bool is_percent_unreserved(char c){
        return     (c >= 'a' && c <= 'z')
                || (c >= 'A' && c <= 'z')
                || (c >= '0' && c <= '9')
                || (c == '-' || c == '_')
                || (c == '?' || c == '~');
    }

    inline unsigned char decode_half_hex(unsigned char in){
        if(in >= '0' && in <= '9')
            return in - '0';
        if(in >= 'a' && in <= 'f')
            return in - 'a' +10;
        if(in >= 'A' && in <= 'F')
            return in - 'A' +10;
        // invalid
        return 0xff;
    }

    inline void decode_percent_rec(string_view sview, std::string & res, unsigned char c = 0x00, int pos = 0){
        if(sview.empty()){
            return;
        }

        unsigned char tmp;
        if(pos == 1 && (tmp = decode_half_hex(sview[0])) != 0xff){
            pos = 2;
            c = tmp << 4;
        }else if(pos == 2 && (tmp = decode_half_hex(sview[0])) != 0xff){
            c |= tmp;
            res.push_back(c);
            pos = 0;
            c = 0;
        }else if( sview[0] == '%'){
            pos = 1;
            c = 0;
        } else {
            res.push_back(sview[0]);
            pos = 0;
            c = 0;
        }
       decode_percent_rec(string_view(sview.data() +1, sview.size()-1), res, c, pos);
    }

}

uri::uri(std::string uri_string) :
    _str_uri(std::move(uri_string)),
    _state(state::invalid),
    _scheme(details::make_empty_view(_str_uri)),
    _userinfo(details::make_empty_view(_str_uri)),
    _host(details::make_empty_view(_str_uri)),
    _port(details::make_empty_view(_str_uri)),
    _path(details::make_empty_view(_str_uri)),
    _query(details::make_empty_view(_str_uri)),
    _fragment(details::make_empty_view(_str_uri))
{
    parse_uri(_str_uri.begin(), _str_uri.begin(), _str_uri.end(), state::scheme);
}

uri::~uri(){

}


bool uri::is_valid() const{
    return _state == state::valid;
}

std::string uri::get_uri() const{
    using namespace details;
    check_validity(_str_uri, _state);
    return _str_uri;
}

std::string uri::get_scheme() const{
    using namespace details;
    check_validity(_str_uri, _state);
    return string_from_view(_scheme);
}



std::string uri::get_userinfo() const{
    using namespace details;
    check_validity(_str_uri, _state);
    return string_from_view(_userinfo);
}



std::string uri::get_host() const{
    using namespace details;
    check_validity(_str_uri, _state);
    return string_from_view(_host);
}

int uri::get_port() const{
    using namespace details;
    check_validity(_str_uri, _state);
    if(std::get<0>(_port) == std::get<1>(_port)){
        return 0;
    }
    return std::stoi(string_from_view(_port));
}

std::string uri::get_path() const{
    using namespace details;
    check_validity(_str_uri, _state);
    return string_from_view(_path);
}

std::string uri::get_query() const{
    using namespace details;
    check_validity(_str_uri, _state);
    return string_from_view(_query);
}

std::string uri::get_fragment() const{
    using namespace details;
    check_validity(_str_uri, _state);
    return string_from_view(_fragment);
}


int uri::parse_uri(iterator_type begin, iterator_type last, iterator_type end, state pstate){
    using namespace details;
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


inline std::string percent_encode(std::string decoded_origin){
    if( std::all_of(decoded_origin.begin(), decoded_origin.end(), details::is_percent_unreserved)){
        return std::move(decoded_origin);
    }
    std::string encoded;
    for(char c : decoded_origin){
        if(details::is_percent_unreserved(c)){
            encoded.push_back(c);
        }else{
            encoded.append({ '%', details::hexmap_uppercase[ ((c >> 4) & 0x0f)], details::hexmap_uppercase[c & 0x0f] });
        }
    }
    return encoded;
}




inline std::string percent_decode(std::string encoded_origin){


    if( std::all_of(encoded_origin.begin(), encoded_origin.end(), [](char c){ return c != '%'; })){
        return std::move(encoded_origin);
    }

    std::string decoded;
    details::decode_percent_rec(encoded_origin, decoded);
    return decoded;
}



} //hadoken

#endif // URI_IMPL_HPP
