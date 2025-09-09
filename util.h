#pragma once
#include <type_traits>

template <typename T>
inline T toInt(int x) {
	return static_cast<T>(x);
}

template<typename E>
constexpr auto inline toUType(E enumerator) noexcept
{
	return static_cast<std::underlying_type_t<E>>(enumerator);
}