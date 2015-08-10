#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <cstring>
#include "../Traits/Traits.h"
namespace LightSTL{

/**********fill函数**********/

//分支,迭代器为char*
inline void fill_branch(char* start,char* finish,const char val,random_access_iterator)
{
	memset(start,val,finish - start);
}

//分支,迭代器为random_access
template<class InputIterator,class T>
void fill_branch(InputIterator start,InputIterator finish,const T& val,random_access_iterator)
{
	size_t n = finish - start;
	while(n--){
		*start++ = val;
	}
}

//分支,迭代器为forward
template<class InputIterator,class T>
void fill_branch(InputIterator start,InputIterator finish,const T& val,forward_iterator)
{
	while(start != finish){
		*start++ = val;
	}
}

//唯一对外接口
template<class InputIterator,class T>
void fill(InputIterator start,InputIterator finish,const T& val)
{
	typedef typename iterator_traits<InputIterator>::iterator_type iterator_type;
	fill_branch(start,finish,val,iterator_type());

}

/**********fill_n函数**********/

inline char* fill_n(char* start,size_t n,const char val)
{
	memset(start,val,n);
	return start + n;
}

template<class InputIterator,class T>
InputIterator fill_n(InputIterator start,size_t n,const T& val)
{
	while(n--){
		*start++ = val;
	}
	return start;
}

/**********copy函数**********/

//分支2,value_type有trivial_assignment
template<class InputIterator,class OutputIterator>
static OutputIterator copy_branch2(InputIterator start,InputIterator finish,OutputIterator des,true_type)
{
    typedef typename iterator_traits<InputIterator>::value_type T;
	size_t n = finish - start;
	memmove(des,start,n * sizeof(T));
	return des + n;
}

//分支2,value_type有nontrivial_assignment
template<class InputIterator,class OutputIterator>
static OutputIterator copy_branch2(InputIterator start,InputIterator finish,OutputIterator des,false_type)
{
	size_t n = finish - start;
	while(n--){
		*des++ = *start++;
	}
	return des;
}


//分支1，迭代器random_access_iterator
template<class InputIterator,class OutputIterator>
static OutputIterator copy_branch1(InputIterator start,InputIterator finish,OutputIterator des,random_access_iterator)
{
    size_t n = finish - start;
	while(n--){
		*des++ = *start++;
	}
	return des;
}

//分支1，迭代器为forward_iterator
template<class InputIterator,class OutputIterator>
static OutputIterator copy_branch1(InputIterator start,InputIterator finish,OutputIterator des,forward_iterator)
{
	while(start != finish){
		*des++ = *start++;
	}
	return des;
}

//三个对外接口
template<class InputIterator,class OutputIterator>
OutputIterator copy(InputIterator start,InputIterator finish,OutputIterator des)
{
	typedef typename iterator_traits<InputIterator>::iterator_type iterator_type;
	return copy_branch1(start,finish,des,iterator_type());
}

template<class T>
T* copy(T* start,T* finish,T* des)
{
	typedef typename type_traits<T>::has_trivial_assignment_type trivial_assignment;
	return copy_branch2(start,finish,des,trivial_assignment());
}

template<class T>
T* copy(const T* start,const T* finish,T* des)
{
	typedef typename type_traits<T>::has_trivial_assignment_type trivial_assignment;
	return copy_branch2(start,finish,des,trivial_assignment());
}

/**********copy_backward函数**********/

//分支2,value_type有trivial_assignment
template<class InputIterator,class OutputIterator>
static OutputIterator copy_backward_branch2(InputIterator start,InputIterator finish,OutputIterator des,true_type)
{
    typedef typename iterator_traits<InputIterator>::value_type T;
	size_t n = finish - start;
	memmove(des - n,start,n * sizeof(T));
	return des - n;
}

//分支2,value_type有nontrivial_assignment
template<class InputIterator,class OutputIterator>
static OutputIterator copy_backward_branch2(InputIterator start,InputIterator finish,OutputIterator des,false_type)
{
	size_t n = finish - start;
	while(n--){
		*--des = *--finish;
	}
	return des;
}


//分支1，迭代器random_access_iterator
template<class InputIterator,class OutputIterator>
static OutputIterator copy_backward_branch1(InputIterator start,InputIterator finish,OutputIterator des,random_access_iterator)
{
    size_t n = finish - start;
	while(n--){
		*--des = *--finish;
	}
	return des;
}

//分支1，迭代器为bidirection_iterator
template<class InputIterator,class OutputIterator>
static OutputIterator copy_backward_branch1(InputIterator start,InputIterator finish,OutputIterator des,bidirection_iterator)
{
	while(start != finish){
		*--des = *--finish;
	}
	return des;
}

//三个对外接口
template<class InputIterator,class OutputIterator>
OutputIterator copy_backward(InputIterator start,InputIterator finish,OutputIterator des)
{
	typedef typename iterator_traits<InputIterator>::iterator_type iterator_type;
	return copy_backward_branch1(start,finish,des,iterator_type());
}

template<class T>
T* copy_backward(T* start,T* finish,T* des)
{
	typedef typename type_traits<T>::has_trivial_assignment_type trivial_assignment;
	return copy_backward_branch2(start,finish,des,trivial_assignment());
}

template<class T>
T* copy_backward(const T* start,const T* finish,T* des)
{
	typedef typename type_traits<T>::has_trivial_assignment_type trivial_assignment;
	return copy_backward_branch2(start,finish,des,trivial_assignment());
}


/**********equal函数**********/
template<class InputIterator1,class InputIterator2>
bool equal(InputIterator1 start1,InputIterator1 finish1,InputIterator2 start2)
{
    while(start1 != finish1){
        if(*start1++ != *start2++)  return false;
    }
    return true;
}

template<class InputIterator1,class InputIterator2,class Compare>
bool equal(InputIterator1 start1,InputIterator1 finish1,InputIterator2 start2,Compare cmp)
{
    while(start1 != finish1){
        if(!cmp(*start1++,*start2++))  return false;
    }
    return true;
}

/**********lower_bound函数**********/

//operator<版本
template<class InputIterator,class T>
InputIterator lower_bound_aux(InputIterator start,InputIterator finish,const T& val,random_access_iterator)
{
    while(finish - start > 0){
        size_t len = finish - start;
        InputIterator mid = start + len / 2;
        if(*mid >= val)     finish = mid;
        else    start = mid + 1;
    }
    return finish;
}


template<class InputIterator,class T>
InputIterator lower_bound(InputIterator start,InputIterator finish,const T& val)
{
    typedef typename iterator_traits<InputIterator>::iterator_type iterator_type;
    return lower_bound_aux(start,finish,val,iterator_type());
}

//仿函数版本
template<class InputIterator,class T,class Compare>
InputIterator lower_bound_aux(InputIterator start,InputIterator finish,const T& val,random_access_iterator,Compare smaller)
{
    while(finish - start > 0){
        size_t len = finish - start;
        InputIterator mid = start + len / 2;
        if(!smaller(*mid,val))     finish = mid;
        else    start = mid + 1;
    }
    return finish;
}


template<class InputIterator,class T,class Compare>
InputIterator lower_bound(InputIterator start,InputIterator finish,const T& val,Compare smaller)
{
    typedef typename iterator_traits<InputIterator>::iterator_type iterator_type;
    return lower_bound_aux(start,finish,val,iterator_type(),smaller);
}


/**********swap函数**********/

template<class T>
void swap(T& lhs,T& rhs)
{
    T tmp = lhs;
    lhs = rhs;
    rhs = tmp;
}

/**********sort函数**********/

//插入排序
template<class RandomAccessIterator>
static void insert_sort(RandomAccessIterator lhs,RandomAccessIterator rhs)
{
    if(lhs == rhs)  return ;
    typedef typename iterator_traits<RandomAccessIterator>::value_type T;
    for(RandomAccessIterator i = lhs + 1;i < rhs;++i){
        T val = *i;
        if(val < *lhs){
            copy_backward(lhs,i,i + 1);
            *lhs = val;
        }
        else{
            RandomAccessIterator j = i;
            while(val < *(j - 1)){
                *j = *(j - 1);
                --j;
            }
            *j = val;
        }
    }
}


}
#endif








