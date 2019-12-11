#pragma once
#include <memory>


template <typename T>
std::shared_ptr<T> make_ptr(T obj);

