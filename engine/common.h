#pragma once

#include "core/log/log.h"

#include <stdint.h>

#include <memory>
template <class T>
using Ref = std::shared_ptr<T>;

template <typename T, typename... Args>
Ref<T> MakeRef(Args&&... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

#include <stdexcept>
using Exception = std::exception;

#include <vector>
template <class T>
using Vector = std::vector<T>;

#include <map>
template <class P, class Q>
using Map = std::map<P, Q>;

#include <unordered_map>
template <class P, class Q>
using HashMap = std::unordered_map<P, Q>;

#include <string>
using String = std::string;
