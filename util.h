#ifndef UTIL_H_
#define UTIL_H_

#include <iostream>
#include <string>

inline void print_debug(std::string s) {
    std::cout << "\e[1;38;5;220m[DEBUG]: " << s << "\e[0m" << std::endl;
}
#endif /* UTIL_H_ */
