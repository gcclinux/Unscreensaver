#include "caching.hpp"
#include "checkkey.hpp"
#include <iostream>

int main() {
   
    if (isAccessKeyValid()) {
        downloadImageFromUnsplash();
    } else {
        std::cerr << "Invalid Unsplash access key." << std::endl;
    }

    return 0;
}