#ifndef __STL_ALLOC_H__
#define __STL_ALLOC_H__

#include <new>
#include <cstddef>
#include <iostream>

template<class T>
inline T* _allocate(ptrdiff_t n, T*)
{
	std::set_new_handler(0);
	T* tmp = (T*)::operator new((size_t)n*sizeof(T));
	return tmp;
}

template<class T>
inline void _deallocate(T* buffer)
{
	::operator delete((void*)buffer);
}

template<class T1, class T2>
inline void _construct(T1* p, const T2& value)
{
	new(p) T1(value);
}

template<class T>
inline void _destory(T* p)
{
	p->~T();
}

template<class T>
class Alloc
{
public:
	typedef T value_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

public:
	pointer allocate(size_type n, void* hint = 0)
	{
		std::cout << "start allocate" << std::endl;
		return _allocate(difference_type(n), pointer(0));
	}

	void deallocate(pointer p, size_type n)
	{
		_deallocate(p);
	}

	void construct(pointer p, const_reference value)
	{
		_construct(p, value);
	}

	void destory(pointer p)
	{
		_destory(p);
	}

	pointer address(reference value)
	{
		return (pointer)&value;
	}

	const_pointer const_address(const_reference value)
	{
		return (const_pointer)&value;
	}
};



#endif