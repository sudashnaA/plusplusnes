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

// Struct must contain POD members otherwise undefined behaviour can occur
template<typename T, std::size_t size>
constexpr void inline zeroPODStructArray(std::array<T, size>& arr) noexcept
{
	for (auto& entry : arr)
	{
		std::memset(&entry, 0, sizeof(entry));
	}
}