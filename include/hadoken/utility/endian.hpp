#ifndef HADOKEN_ENDIAN_HPP
#define HADOKEN_ENDIAN_HPP

#include <sys/param.h>
#include <netinet/in.h>

//#include <boost/integer.hpp>


namespace hadoken{

///
/// \brief host to network endianness conversion ( * -> bigendian )
/////
inline boost::uint16_t hton(boost::uint16_t n){
    return htons(n);
}

inline boost::uint32_t hton(boost::uint32_t n){
    return htonl(n);
}

inline boost::uint64_t hton(boost::uint64_t n){
    const int num = 1;

    if(*(char *)&num == 1){
        return (((boost::uint64_t )htonl(n)) << 32) + htonl(n >> 32);
    }

    return n;
}




///
/// \brief network to host endianness conversion ( bigendian -> native )
/////
inline boost::uint16_t ntoh(boost::uint16_t n){
    return ntohs(n);
}

inline boost::uint32_t ntoh(boost::uint32_t n){
    return ntohl(n);
}

inline boost::uint64_t ntoh(boost::uint64_t n){
    const int num = 1;

    if(*(char *)&num == 1){
        return (((boost::uint64_t )ntohl(n)) << 32) + ntohl(n >> 32);
    }

    return n;
}


}

#endif // ENDIAN_HPP
