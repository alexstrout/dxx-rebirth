#pragma once

#include <iterator>

/* `null_sentinel_sentinel` is an empty type returned by the `end()` method of
 * containers that use `null_sentinel_iterator` to traverse their storage.
 * `null_sentinel_iterator` defines `operator==` to recognize
 * `null_sentinel_sentinel` and respond appropriately.
 *
 * The template parameter exists solely to prevent accidental mixing of
 * sentinels from different container types.
 */
template <typename I>
struct null_sentinel_sentinel
{
};

/* `null_sentinel_iterator` wraps a pointer of type `I *` and allows the caller
 * to traverse a contiguous container of indeterminate length.  The container
 * is required to have an element with value `nullptr` as a sentinel, placed
 * after all valid elements.  Any elements beyond the sentinel are assumed to
 * be outside the container's storage, and are not accessed.
 * `null_sentinel_iterator` compares equal to `null_sentinel_sentinel` when the
 * `null_sentinel_iterator` reaches the `nullptr` value in the container.
 */
template <typename I>
class null_sentinel_iterator
{
public:
	using iterator_category = std::forward_iterator_tag;
	using value_type = I;
	using difference_type = std::ptrdiff_t;
	using pointer = I *;
	using reference = I &;
	constexpr null_sentinel_iterator() = default;
	constexpr null_sentinel_iterator(const pointer i) :
		p{i}
	{
	}
	pointer get() const
	{
		return p;
	}
	reference operator*() const
	{
		return *p;
	}
	null_sentinel_iterator &operator++()
	{
		++p;
		return *this;
	}
	null_sentinel_iterator operator++(int)
	{
		auto r{*this};
		++ *this;
		return r;
	}
	constexpr bool operator==(const null_sentinel_iterator &) const = default;
	constexpr bool operator==(null_sentinel_sentinel<I>) const
	{
		return !**this;
	}
private:
	pointer p{nullptr};
};

/* `null_sentinel_array` wraps an array of pointers and treats the array as a
 * container suitable for use with `null_sentinel_iterator`.
 *
 * `null_sentinel_array` does not take ownership of the array.
 */
template <typename I>
class null_sentinel_array
{
	I *const b;
public:
	constexpr null_sentinel_array(I *const i) :
		b{i}
	{
	}
	null_sentinel_iterator<I> begin() const
	{
		return b;
	}
	null_sentinel_sentinel<I> end() const
	{
		return {};
	}
};
