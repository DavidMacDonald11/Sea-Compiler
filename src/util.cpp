#include <stdexcept>
#include "util.h"

bool isSubstr(str s1, str s2) {
    return s2.find(s1) != str::npos;
}

str replaceStr(const str& s, str s1, str s2) {
    if(s2.find(s1) != str::npos) 
        throw std::runtime_error("replaceStr: s1 in s2 will cause infinite replacing");

    str result = s;
    size_t i = s.find(s1);
    
    for(; i != str::npos; i = result.find(s1)) {
        result.replace(i, s1.size(), s2);
    }

    return result;
}

str multiplyStr(str s, nat n) {
    str string;
    for(nat i = 0; i < n; i++) string += s;
    return string;
}
