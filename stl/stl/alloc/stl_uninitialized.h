#ifndef __STL_UNINITIALIZED_H__
#define __STL_UNINITIALIZED_H__

#include <new>

template<class ForwardIter, class Size, class T>
ForwardIter __uninitialized_fill_n_aux(ForwardIter first, Size n, const T& value, __false_type)
{	
	ForwardIter current = first;
	for (; n > 0; n--, current++)
	{
		__constuct(&*current, value);
	}
	return current;
}

template<class ForwardIter, class Size, class T>
ForwardIter __uninitialized_fill_n_aux(ForwardIter first, Size n, const T& value, __true_type)
{
	return fill_n(first, n, value);
}

template<class ForwardIter, class Size, class T, class T1>
ForwardIter __uninitialized_fill_n(ForwardIter first, Size n, const T& value, T1*)
{
	typedef typename __type_traits<T1>::is_POD_type is_POD;
	return __uninitialized_fill_n_aux(first, n, value, is_POD());
}


template<class ForwardIter, class Size, class T>
ForwardIter uninitialized_fill_n(ForwardIter first, Size n, const T& value)
{
	return __uninitialized_fill_n(first, n, value, value_type(first));
}


template<class InputIter, class ForwardIter>
inline Forward __uninitialized_copy_aux(InputIter first, InputIter last, ForwardIter result, __false_type)
{
	ForwardIter cur = result;
	for (; last != first; first++, cur++)
	{
		construct(&*cur, *first);
	}

	return cur;
}

template<class InputIter, class ForwardIter>
inline Forward __uninitialized_copy_aux(InputIter first, InputIter last, ForwardIter result, __true_type)
{
	return copy(first, last, result);
}


template<class InputIter, class ForwardIter, class T>
inline Forward __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, T*)
{
	typedef typename __type_traits<T>::is_POD_type is_POD;
	return __uninitialized_copy_aux(first, last, result, is_POD());
}



template<class InputIter, class ForwardIter>
inline Forward uninitialized_copy(InputIter first, InputIter last, ForwardIter result)
{
	return __uninitialized_copy(first, last, result, value_type(result));
}



template<class ForwardIter, class T, class T1>
inline void __uninitialized_fill(ForwardIter first, ForwardIter last, const T& value, T1*)
{
	typedef typename __type_traits<T1>::is_POD_type is_POD;
	return __uninitialized_fill_aux(first, last, value, is_POD());
}

template<class ForwardIter, class T>
inline void __uninitialized_fill_aux(ForwardIter first, ForwardIter last, const T& value, __false_type)
{
	fill(first, last, value);
}

template<class ForwardIter, class T>
inline void __uninitialized_fill_aux(ForwardIter first, ForwardIter last, const T& value, __true_type)
{
	fill(first, last, value);
}



template<class ForwardIter, class T>
inline void uninitialized_fill(ForwardIter first, ForwardIter last, const T& value)
{
	ForwardIter cur = first;
	for (; cur != last; cur++)
	{
		construct(&*cur, value);
	}
}


#endif