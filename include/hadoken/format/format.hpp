#ifndef _HADOKEN_FORMAT_HPP_
#define _HADOKEN_FORMAT_HPP_

#include <ostream>
#include <sstream>
#include <string>
#include <type_traits>

namespace hadoken {

namespace format{

inline void scat(std::ostream & ostream){
    (void) ostream;
}



///
/// @brief scat
///
/// concatenate an arbitrary list of argument to an std::ostream
/// Proviate an alternative syntax to iostream for convenience
///
///  e.g
///  format::scat(std::cout, "hello world", "I am ", 2500, "years old today");
///
///  support any type handled by ostream
///
///
template<typename T, typename ... Args>
inline void scat(std::ostream & ostream, const T & arg1, const Args&...  args){
    ostream << arg1;
    scat(ostream, args...);
}



// enable only for 1st argument != ostream

///
/// @brief scat
///
/// concatenate an arbitrary list of argument to a string representation.
/// Proviate an alternative syntax to iostream for convenience
///
///  e.g
///  auto stre = format::scat("hello world", "I am ", 2500, "years old today");
///
///  support any type handled by ostream
///
///
template<typename T, typename ... Args>
inline  typename std::enable_if<(not std::is_base_of<std::ostream, T>::value), std::string>::type
scat(const T & arg1, const Args&...  args){
    std::ostringstream oss;
    scat(oss, arg1, args...);
    return oss.str();
}




} //format


} //hadoken

#endif // FORMAT_HPP

