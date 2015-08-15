#ifndef __STL_MALLOC_ALLOC_TEMPLATE_H__
#define __STL_MALLOC_ALLOC_TEMPLATE_H__

#include <new>
#include "stl_malloc_alloc_template.h"

enum { __ALIGN = 8 };
enum { __MAX_BYTES = 128 };
enum { __NFREELISTS = __MAX_BYTES/__ALIGN };

template<bool thread, int inst>
class __default_alloc_template
{
private:
	static size_t ROUND_UP(size_t bytes)
	{
		return ((bytes + __ALIGN - 1) & ~(__ALIGH - 1));
	}

private:
	union obj
	{
		union obj* free_list_link;
		char client_data[1];
	};

private:
	static obj* volatile free_list[__NFREELISTS];
	static size_t FREELIST_INDEX(size_t bytes)
	{
		return ((bytes + __ALIGN - 1) / __ALIGN - 1);
	}

	static void* refill(size_t n);
	static char* chunk_alloc(size_t n, int& nobjs);

	static char* start_free;
	static char* end_free;
	static size_t heap_size;

public:
	static void* allocate(size_t n);
	static void deallocate(void* p, size_t n);
	static void* reallocate(void* p, size_t oldsz, size_t newsz);
};

template<bool thread, int inst>
char* __default_alloc_template<thread, inst>::start_free = nullptr;

template<bool thread, int inst>
char* __default_alloc_template<thread, inst>::end_free = nullptr;

template<bool thread, int inst>
size_t __default_alloc_template<thread, inst>::heap_size = 0;

template<bool thread, int inst>
__default_alloc_template<thread, inst>::obj* 
__default_alloc_template<thread, inst>::free_list[__NFREELISTS] = { 0 };

template<bool thread, int inst>
void* __default_alloc_template<thread, inst>::allocate(size_t n)
{
	obj* volatile *my_free_list = nullptr;
	obj* result = nullptr;

	if (n > __MAX_BYTES)
	{
		return malloc_alloc::allocate(n);
	}

	my_free_list = free_list + FREELIST_INDEX(n);

	result = *my_free_list;
	if (0 == result)
	{
		return refill(ROUND_UP(n));
	}

	*my_free_list = result->free_list_link;

	return result;
}

template<bool thread, int inst>
void __default_alloc_template<thread, inst>::deallocate(void* p, size_t n)
{
	obj* q = (obj*)p;
	obj* volatile *my_free_list = nullptr;


	if (n > __max)
	{
		malloc_alloc:deallocate(p, n);
		return;
	}

	my_free_list = free_list + FREELIST_INDEX(n);
	q->free_list_link = *my_free_list;
	*my_free_list = q;

	return;

}

template<bool thread, int inst>
void* __default_alloc_template<thread, inst>::refill(size_t n)
{
	int nobjs = 20;

	char* chunk = chunk_alloc(n, nobjs);
	if (1 == nobjs)
	{
		return chunk;
	}

	obj* volatile *my_free_list = nullptr;
	obj* result = nullptr;
	obj* current = nullptr;
	obj* next = nullptr;

	my_free_list = free_list + FREELIST_INDEX(n);

	result = (obj*)chunk;
	*my_free_list = next = (obj*)(chunk + n);
	for (int i = 1; ; i++)
	{
		current = next;
		next = (obj*)((char*)next + n);
		if (nobjs - i == 1)
		{
			current->free_list_link = nullptr;
			break;
		}
		else
		{
			current->free_list_link = next;
		}
	}
	
	return result;

}

#endif
