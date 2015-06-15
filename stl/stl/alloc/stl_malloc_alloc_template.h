#ifndef __STL_MALLOC_ALLOC_TEMPLATE_H__
#define __STL_MALLOC_ALLOC_TEMPLATE_H__

#include <new>

template<int inst>
class __malloc_alloc_template
{
private:
	//oom:out of memory
	static void* oom_malloc(size_t);
	static void* oom_remalloc(void*, size_t);
	static void(*__malloc_alloc_oom_handler)();

public:
	// all static
	static void* allocate(size_t n)
	{
		void* result = malloc(n);
		if (result == 0)
			result = oom_malloc(n);

		return result;
	}

	static void deallocate(void* p, size_t)
	{
		free(p);
	}

	static void* reallocate(void* p, size_t oldsz, size_t newsz)
	{
		void* result = realloc(p, newsz);
		if (result == 0)
			result = oom_remalloc(p, newsz);

		return result;
	}

	static void(*set_malloc_handler(void(*f)()))()
	{
		void(*old)() = __malloc_alloc_oom_handler;
		__malloc_alloc_oom_handler = f;
		return old;
	}
};

template<int inst>
void(*__malloc_alloc_template<inst>::__malloc_alloc_oom_hanlder)() = nullptr;

template<int inst>
void* __malloc_alloc_template<inst>::oom_malloc(size_t n)
{
	void(*my_mallloc_handler)() = nullptr;
	void* result = nullptr;
	for (;;)
	{
		my_mallloc_handler = __malloc_alloc_oom_handler;
		if (my_mallloc_handler == 0)
			throw bad_alloc;

		(*my_mallloc_handler)();
		result = mallloc(n);
		if (result)
			return result;
	}
}

template<int inst>
void* __malloc_alloc_template<inst>::oom_remalloc(void* p, size_t n)
{
	void(*my_mallloc_handler)() = nullptr;
	void* result = nullptr;
	for (;;)
	{
		my_mallloc_handler = __malloc_alloc_oom_handler;
		if (my_mallloc_handler == 0)
			throw bad_alloc;

		(*my_mallloc_handler)();
		result = realloc(p, n);
		if (result)
			return result;
	}
}

typedef __malloc_alloc_template<0> malloc_alloc;

#endif