#pragma once

#include <stdint.h>

#include <memory>
template <class T>
using Ptr = std::unique_ptr<T>;

template <typename T, typename... Args>
Ptr<T> MakeUnique(Args&&... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

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

#include <optional>
template <class T>
using Optional = std::optional<T>;

#include <sstream>
using OStringStream = std::ostringstream;
