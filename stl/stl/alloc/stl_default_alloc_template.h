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

template<bool thread, int inst>
char* __default_alloc_template<thread, inst>::chunk_alloc(size_t n, int& nobjs)
{
	char* result = nullptr;
	size_t total_bytes = n * nobjs;

	size_t bytes_left = end_free - start_free;

	if (bytes_left >= total_bytes)
	{
		result = start_free;
		start_free += total_bytes;
		return result;		
	}
	else if (bytes_left > n)
	{
		nobjs = bytes_left / n;
		total_bytes = n * nobjs;
		result = start_free;
		start_free += total_bytes;
		return result;
	}
	else
	{
		if (bytes_left > 0)
		{
			obj* volatile *my_free_list = free_list + FREELIST_INDEX(bytes_left);
			((obj*)start_free)->free_list_link = *my_free_list;
			*my_free_list = start_free;
		}

		size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
		start_free = (char*)malloc(bytes_to_get);
		if (0 == start_free)
		{
			obj* volatile* my_free_list = nullptr;
			obj* p = nullptr;

			for (int i = size; i < __MAX_BYTES; i += __ALIGN)
			{
				my_free_list = free_list + FREELIST_INDEX(i);
				p = *my_free_list;
				if (0 != p)
				{
					*my_free_list = p->free_list_link;
					start_free = p;
					end_free = p + i;
					return chunk_alloc(n, nobjs);
				}
			}

			end_free = 0;
			start_free = malloc_alloc::allocate(bytes_to_get);
		}

		heap_size += bytes_to_get;
		end_free = start_free + bytes_left;
		return malloc_alloc::allocate(bytes_to_get);
	}
}

#endif
