#include "util.h"

#include <iostream>

void println() { 
    std::lock_guard<std::recursive_mutex> lg{out_mtx};
    std::cout << std::endl;
}