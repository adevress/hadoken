#ifndef HADOKEN_ENDIAN_HPP
#define HADOKEN_ENDIAN_HPP


#include <sys/param.h>
#include <netinet/in.h>

#include <cstdint>


namespace hadoken{

///
/// \brief host to network endianness conversion ( * -> bigendian )
/////
inline std::uint16_t hton(std::uint16_t n){
    return htons(n);
}

inline std::uint32_t hton(std::uint32_t n){
    return htonl(n);
}

inline std::uint64_t hton(std::uint64_t n){
    const int num = 1;

    if(*(char *)&num == 1){
        return (((std::uint64_t )htonl(n)) << 32) + htonl(n >> 32);
    }

    return n;
}




///
/// \brief network to host endianness conversion ( bigendian -> native )
/////
inline std::uint16_t ntoh(std::uint16_t n){
    return ntohs(n);
}

inline std::uint32_t ntoh(std::uint32_t n){
    return ntohl(n);
}

inline std::uint64_t ntoh(std::uint64_t n){
    const int num = 1;

    if(*(char *)&num == 1){
        return (((std::uint64_t )ntohl(n)) << 32) + ntohl(n >> 32);
    }

    return n;
}


}

#endif // ENDIAN_HPP
