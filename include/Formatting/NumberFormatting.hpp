#ifndef NUMBER_FORMATTING_HPP
#define NUMBER_FORMATTING_HPP

#include <string>

namespace formatting {


/* Binary Operations */
bool is_binary(const std::string& arr);
void binary_add(std::string& target, const std::string& arr1, const std::string& arr2);

}

#endif