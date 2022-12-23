#include "util.h"

bool isSubstr(str s1, str s2) {
    return s2.find(s1) != str::npos;
}
