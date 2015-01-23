/*
 * Copyright (c) 2012-2015 Daniele Bartolini and individual contributors.
 * License: https://github.com/taylor001/crown/blob/master/LICENSE
 */

#pragma once

#include "container_types.h"
#include "allocator.h"
#include "assert.h"
#include "macros.h"

namespace crown
{

/// Functions to manipulate Vector.
///
/// @ingroup Containers
namespace vector
{
	/// Returns whether the vector @a v is empty.
	template <typename T> bool empty(const Vector<T>& v);

	/// Returns the number of items in the vector @a v.
	template <typename T> uint32_t size(const Vector<T>& v);

	/// Returns the maximum number of items the vector @a v can hold.
	template <typename T> uint32_t capacity(const Vector<T>& v);

	/// Resizes the vector @a v to the given @a size.
	/// @note
	/// Old items will be copied to the newly created vector.
	/// If the new capacity is smaller than the previous one, the
	/// vector will be truncated.
	template <typename T> void resize(Vector<T>& v, uint32_t size);

	/// Reserves space in the vector @a v for at least @a capacity items.
	template <typename T> void reserve(Vector<T>& v, uint32_t capacity);

	/// Sets the capacity of vector @a v.
	template <typename T> void set_capacity(Vector<T>& v, uint32_t capacity);

	/// Grows the vector @a v to contain at least @a min_capacity items.
	template <typename T> void grow(Vector<T>& v, uint32_t min_capacity);

	/// Condenses the vector @a v so that its capacity matches the actual number
	/// of items in the vector.
	template <typename T> void condense(Vector<T>& v);

	/// Appends an item to the vector @a v and returns its index.
	template <typename T> uint32_t push_back(Vector<T>& v, const T& item);

	/// Removes the last item from the vector @a v.
	template <typename T> void pop_back(Vector<T>& v);

	/// Appends @a count @a items to the vector @a v and returns the number
	/// of items in the vector after the append operation.
	template <typename T> uint32_t push(Vector<T>& v, const T* items, uint32_t count);

	/// Clears the content of the vector @a v.
	/// @note
	/// Calls destructor on the items.
	template <typename T> void clear(Vector<T>& v);

	template <typename T> T* begin(Vector<T>& v);
	template <typename T> const T* begin(const Vector<T>& v);
	template <typename T> T* end(Vector<T>& v);
	template <typename T> const T* end(const Vector<T>& v);

	template <typename T> T& front(Vector<T>& v);
	template <typename T> const T& front(const Vector<T>& v);
	template <typename T> T& back(Vector<T>& v);
	template <typename T> const T& back(const Vector<T>& v);
} // namespace vector

namespace vector
{
	template <typename T>
	inline bool empty(const Vector<T>& v)
	{
		return v._size == 0;
	}

	template <typename T>
	inline uint32_t size(const Vector<T>& v)
	{
		return v._size;
	}

	template <typename T>
	inline uint32_t capacity(const Vector<T>& v)
	{
		return v._capacity;
	}

	template <typename T>
	inline void resize(Vector<T>& v, uint32_t size)
	{
		if (size > v._capacity)
			set_capacity(v, size);

		v._size = size;
	}

	template <typename T>
	inline void reserve(Vector<T>& v, uint32_t capacity)
	{
		if (capacity > v._capacity)
			grow(v, capacity);
	}

	template <typename T>
	inline void set_capacity(Vector<T>& v, uint32_t capacity)
	{
		if (capacity == v._capacity)
			return;

		if (capacity < v._size)
			resize(v, capacity);

		if (capacity > 0)
		{
			T* tmp = v._array;
			v._capacity = capacity;
			v._array = (T*)v._allocator->allocate(capacity * sizeof(T), CE_ALIGNOF(T));

			for (uint32_t i = 0; i < v._size; i++)
			{
				new (v._array + i) T(tmp[i]);
			}

			if (tmp)
			{
				for (uint32_t i = 0; i < v._size; i++)
				{
					tmp[i].~T();
				}
				v._allocator->deallocate(tmp);
			}
		}
	}

	template <typename T>
	inline void grow(Vector<T>& v, uint32_t min_capacity)
	{
		uint32_t new_capacity = v._capacity * 2 + 1;

		if (new_capacity < min_capacity)
			new_capacity = min_capacity;

		set_capacity(v, new_capacity);
	}

	template <typename T>
	inline void condense(Vector<T>& v)
	{
		resize(v, v._size);
	}

	template <typename T>
	inline uint32_t push_back(Vector<T>& v, const T& item)
	{
		if (v._capacity == v._size)
			grow(v, 0);

		new (v._array + v._size) T(item);

		return v._size++;
	}

	template <typename T>
	inline void pop_back(Vector<T>& v)
	{
		CE_ASSERT(v._size > 0, "The vector is empty");

		v._array[v._size - 1].~T();
		--v._size;
	}

	template <typename T>
	inline uint32_t push(Vector<T>& v, const T* items, uint32_t count)
	{
		if (v._capacity <= v._size + count)
			grow(v, v._size + count);

		T* arr = &v._array[v._size];
		for (uint32_t i = 0; i < count; i++)
		{
			arr[i] = items[i];
		}

		v._size += count;
		return v._size;
	}

	template <typename T>
	inline void clear(Vector<T>& v)
	{
		for (uint32_t i = 0; i < v._size; i++)
		{
			v._array[i].~T();
		}

		v._size = 0;
	}

	template <typename T>
	inline T* begin(Vector<T>& v)
	{
		return v._array;
	}

	template <typename T>
	inline const T* begin(const Vector<T>& v)
	{
		return v._array;
	}

	template <typename T>
	inline T* end(Vector<T>& v)
	{
		return v._array + v._size;
	}

	template <typename T>
	inline const T* end(const Vector<T>& v)
	{
		return v._array + v._size;
	}

	template <typename T>
	inline T& front(Vector<T>& v)
	{
		CE_ASSERT(v._size > 0, "The vector is empty");

		return v._array[0];
	}

	template <typename T>
	inline const T& front(const Vector<T>& v)
	{
		CE_ASSERT(v._size > 0, "The vector is empty");

		return v._array[0];
	}

	template <typename T>
	inline T& back(Vector<T>& v)
	{
		CE_ASSERT(v._size > 0, "The vector is empty");

		return v._array[v._size - 1];
	}

	template <typename T>
	inline const T& back(const Vector<T>& v)
	{
		CE_ASSERT(v._size > 0, "The vector is empty");

		return v._array[v._size - 1];
	}
} // namespace vector

template <typename T>
inline Vector<T>::Vector(Allocator& a)
	: _allocator(&a)
	, _capacity(0)
	, _size(0)
	, _array(NULL)
{
}

template <typename T>
inline Vector<T>::Vector(Allocator& a, uint32_t capacity)
	: _allocator(&a)
	, _capacity(0)
	, _size(0)
	, _array(NULL)
{
	vector::resize(*this, capacity);
}

template <typename T>
inline Vector<T>::Vector(const Vector<T>& other)
	: _allocator(other._allocator)
	, _capacity(0)
	, _size(0)
	, _array(NULL)
{
	*this = other;
}

template <typename T>
inline Vector<T>::~Vector()
{
	for (uint32_t i = 0; i < _size; i++)
	{
		_array[i].~T();
	}

	if (_array)
		_allocator->deallocate(_array);
}

template <typename T>
inline T& Vector<T>::operator[](uint32_t index)
{
	CE_ASSERT(index < _size, "Index out of bounds");

	return _array[index];
}

template <typename T>
inline const T& Vector<T>::operator[](uint32_t index) const
{
	CE_ASSERT(index < _size, "Index out of bounds");

	return _array[index];
}

template <typename T>
inline const Vector<T>& Vector<T>::operator=(const Vector<T>& other)
{
	const uint32_t size = vector::size(other);
	vector::resize(*this, size);

	for (uint32_t i = 0; i < size; i++)
	{
		_array[i] = other._array[i];
	}

	return *this;
}

} // namespace crown
