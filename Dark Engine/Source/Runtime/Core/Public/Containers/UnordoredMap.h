#pragma once



#include <unordered_map>
#include <unordered_set>
#include "map"

template <typename T>
using TUnordoredSet = std::unordered_set<T>;

template <typename Left, typename Right>
using TUnordoredMap = std::unordered_map<Left, Right>;

template <typename ...Args>
using TMap = std::map<Args...>;