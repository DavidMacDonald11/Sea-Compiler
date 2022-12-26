#include "util.h"

bool isSubstr(str s1, str s2) {
    return s2.find(s1) != str::npos;
}

str replaceStr(const str& s, str s1, str s2) {
    str result = s;
    size_t i = s.find(s1);
    
    for(; i != str::npos; i = result.find(s1)) {
        result.replace(i, s1.size(), s2);
    }

    return result;
}
