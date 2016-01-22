/**
 * Copyright (C) 2016 Adrien Devresse
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
*/
#ifndef _HADOKEN_STRING_ALGO_
#define _HADOKEN_STRING_ALGO_

#include <string>
#include <vector>
#include <memory>
#include <algorithm>


namespace hadoken{

namespace string{


/////////////////////////////

///
/// \brief tokenize string to a vector of delimited token
///
/// Parse str and split the string on each char contained in the delimiter string
/// return a vector of string
///
/// \param str
/// \param delimiter
/// \return vector of delimited string
///
inline std::vector<std::string> tokenize(const std::string & str, const std::string & delimiter){
        std::vector<std::string> res;
        std::string::const_iterator it_prev, it_cur;
        for(it_prev = it_cur = str.begin(); it_cur < str.end(); ((it_prev != str.end())?(it_prev++):(it_prev))){
            it_cur = std::find_first_of(it_prev, str.end(), delimiter.begin(), delimiter.end());
            if(it_prev != it_cur)
                res.push_back(std::string(it_prev,it_cur));
            it_prev = it_cur;
        }
        return res;

}



}

}

#endif // STRINGUTILS_H
