#ifndef __STL_MALLOC_ALLOC_TEMPLATE_H__
#define __STL_MALLOC_ALLOC_TEMPLATE_H__

#include <new>

template<class T1, class T2>
inline void construct(T1* p, const T2& value)
{
	new(p) T1(value);
}

template<class T>
inline void destory(T* p)
{
	p->~T();
}

template<class ForwardIter>
inline void destroy(ForwardIter first, ForwardIter last)
{
	__destory(first, last, value_type(first));
}

template<class ForwardIter, class T>
inline void __destroy(ForwardIter first, ForwardIter last, T*)
{
	typedef typename __type_traits<T>::has_trivail_destructor trivial_destructor;
	__destory_aux(first, last, trivial_destructor());
}

template<class ForwardIter>
inline void __destory_aux(ForwardIter first, ForwardIter last, __false_type)
{
	for (; first < last; first++)
		destroy(&*first);
}

template<class ForwardIter>
inline void __destory_aux(ForwardIter first, ForwardIter last, __true_type)
{

}

inline void destory(char*, char*){}
inline void destory(wchar_t*, wchar_t*){}



#endif