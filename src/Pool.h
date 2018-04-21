#pragma once
#ifndef POOLALLOCATOR_HPP
# define POOLALLOCATOR_HPP

#include <cstddef>
#include <string>
#include <vector>

void pool_init(size_t n);
void pool_reset(void);
void *pool_alloc(size_t n);

template <class T>
class pool_allocator : public std::allocator<T>
{
	public:
	typedef size_t size_type;
	typedef T* pointer;
	typedef const T* const_pointer;

	template<typename _Tp1>
	struct rebind
	{
		typedef pool_allocator<_Tp1> other;
	};

	T* allocate(std::size_t n) {auto p = static_cast<T*>(pool_alloc(n*sizeof(T))); return p; }
	void deallocate(T* p, std::size_t) noexcept {}

	pool_allocator() throw(): std::allocator<T>() {}
	pool_allocator(const pool_allocator &a) throw(): std::allocator<T>(a) { }
	template <class U>
	pool_allocator(const pool_allocator<U> &a) throw(): std::allocator<T>(a) { }
	~pool_allocator() throw() { }
};

template <class T, class U>
bool operator==(const pool_allocator<T>&, const pool_allocator<U>&) { return true; }
template <class T, class U>
bool operator!=(const pool_allocator<T>&, const pool_allocator<U>&) { return false; }

typedef std::basic_string<char, ::std::char_traits<char>, pool_allocator<char>> pool_string;

template <typename T>
using pool_vector = ::std::vector<T, pool_allocator<T> >;

class pool_base {
public:
       void *operator new(size_t bytes) {
               return pool_alloc(bytes);
       }
};

#endif // POOLALLOCATOR_HPP
