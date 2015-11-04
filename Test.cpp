#include <iostream>
#include "NumberlikeArray.h"
#include "BigUnsigned.h"

int main() {
    BigUnsigned v(100);
    std::cout << v.toInt() << std::endl;

    return 0;
}
