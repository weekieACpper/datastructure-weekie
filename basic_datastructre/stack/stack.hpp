/*
 * @Author: weekie
 * @Date: 2022-02-24 09:38:37
 * @LastEditTime: 2022-02-28 20:23:54
 * @LastEditors: weekie
 * @Description: 栈的接口声明部分
 * @FilePath: /datastructure/stack/stack.hpp
 */
#ifndef __STACK_HPP__
#define __STACK_HPP__
/**
 * @description: 栈的ADT描述
 */
class stack
{
public:
    stack();//构造函数
    stack(const int & size);//构造函数
    ~stack();//析构函数
public:
    bool empty();//判断栈是否为空
    int size();//返回栈的大小
    int capacity();//返回栈空间大小
    int top();//返回栈顶元素
    void pop();//弹栈
    void push(const int & value);//压栈
private:
    void allocate(const int & newCapacity);//用于分配空间，防止栈上溢
private:
    int _capacity;//栈的空间
    int _top;//栈顶在数组中的位置，初始为-1.
    int* _array;//数组
};
#endif