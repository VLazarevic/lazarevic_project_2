#pragma once

#include <vector>
#include <string>
#include <mutex>
#include <iostream>
#include <ctime>
#include <unistd.h>

static std::recursive_mutex out_mtx;

void println();

template<typename T, typename... Rest>
void println(const T& word, const Rest&... rest) {
  std::lock_guard<std::recursive_mutex> lg{out_mtx};
  std::cout << word;
  println(rest...);
}
