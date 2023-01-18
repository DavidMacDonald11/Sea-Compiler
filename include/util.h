#ifndef UTIL_H
#define UTIL_H

#include <algorithm>
#include <array>
#include <cctype>
#include <initializer_list>
#include <fmt/format.h>
#include <fstream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#define self (*this)

using nat = unsigned int;
using str = std::string;
using std::map;
using std::set;
using std::vector;

template<class T> using uptr = std::unique_ptr<T>;
template<class T> using sptr = std::shared_ptr<T>;
template<class T> using list = std::initializer_list<T>;
template<class T, size_t Nm> using array = std::array<T, Nm>;
template<class T1, class T2> using pair = std::pair<T1, T2>;

template<class T, class Container>
bool in(T i, Container cont) {
    return std::find(cont.begin(), cont.end(), i) != cont.end();
}

template<class Range>
str join(Range&& range, fmt::string_view sep) {
    return fmt::format("{}", fmt::join(range, sep));
}

bool isSubstr(str s1, str s2);
str replaceStr(const str& s, str s1, str s2);
str multiplyStr(str s, nat n);

template<class T>
vector<T> mergeAll(list<vector<T>> args) {
    size_t size = 0;
    vector<T> result;

    for(vector<T> arg : args) size += arg.size();
    result.reserve(size);

    for(vector<T> arg : args) {
        result.insert(result.end(), arg.begin(), arg.end());
    }

    return result;
}

template<class T>
str className(T* arg) {
    str name;

    for(char c : str(typeid(*arg).name())) {
        if(std::isdigit(c) and name == "") continue;
        name += c;
    }

    return name;
}

template<class T>
str vectorToString(const vector<T>& items) {
    str string;

    for(const T& item : items) {
        str iString = item.toString();
        string = (string == "")? iString : fmt::format("{}, {}", string, iString);
    }

    return string;
}

template<class T1, class T2>
std::map<T2, T1> invertMap(std::map<T1, T2> map) {
    std::map<T2, T1> result;
    for(std::pair<T1, T2> pair : map) result.insert({pair.second, pair.first});
    return result;
}

#endif
