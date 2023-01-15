#include <string>

#include "Formatting/NumberFormatting.hpp"

namespace formatting {

bool is_binary(const std::string &arr) {
    bool ret_value = false;

    int i = 0;
    for (i = 0; i < arr.size(); i++) {
        if (arr[i] != '0' && arr[i] != '1') {
            break;
        }
    }

    if (i == arr.size()) {
        ret_value = true;
    }

    return ret_value;
}

void binary_add(std::string& target, const std::string& arr1, const std::string& arr2) {
        const int arr1_size = arr1.size();
        const int arr2_size = arr2.size();

        char sum = '0';
        char carry = '0';
        char extra_carry = '0';    

        if (!is_binary(arr1) || !is_binary(arr2)) {
            /* TODO: Arrays are not valid. Throw exception? */
            return;
        }

        if (arr1_size != arr2_size) {
            /* TODO: Not the same size. Throw exception? */
            return;
        }
        
        /* Prepare target string */
        target.assign(arr1_size + 1, '0');

        for (int i = arr1_size - 1; 0 <= i; i--) {
            sum = '0';
            extra_carry = '0';

            /* 1. Add two bits normally */
            if (arr1[i] != arr2[i]) {
                sum = '1';
            }
            else if (arr1[i] == '1') {
                sum = '0';
                extra_carry = '1';
            }

            /* 2. Add carry */
            if (carry != sum) {
                sum = '1';
            }
            else if (carry == '1') {
                sum = '0';
                extra_carry = '1';
            }

            /* Target size is (n+1) */
            target[i + 1] = sum;
            carry = extra_carry;
        }
        
        /* Don't forget the leftmost bit */
        target[0] = carry; 
    }

}