/*
 * @Author: weekie
 * @Date: 2022-02-24 09:38:30
 * @LastEditTime: 2022-02-28 21:05:21
 * @LastEditors: weekie
 * @Description: 栈的接口实现部分
 * @FilePath: /datastructure/stack/stack.cpp
 */
#include<stack.hpp>
#include<stdexcept>
/**
 * @description: 构造函数
 * @param {*}
 * @return {*}
 */
stack::stack():_capacity(10), _top(-1)
{
    _array = new int[10];//初始分配10个空间大小
}
/**
 * @description: 接受一个参数的构造函数
 * @param {int &} size:待设置的栈初始空间大小
 * @return {*}
 */
stack::stack(const int & size):_capacity(size), _top(-1)
{
    _array = new int[size];
}
/**
 * @description: 析构函数
 * @param {*}
 * @return {*}
 */
stack::~stack()
{
    delete [] _array;//释放数组空间
}
/**
 * @description: 判断栈是否为空
 * @param {*}
 * @return {bool}:如果true说明为空
 */
bool stack::empty()
{
    return _top == -1;
}
/**
 * @description: 返回栈中的元素个数
 * @param {*}
 * @return {int}:栈中的元素个数
 */
int stack::size()
{
    return _top + 1;
}
/**
 * @description: 返回栈使用的空间大小
 * @param {*}
 * @return {int}:空间大小
 */
int stack::capacity()
{
    return _capacity;
}
/**
 * @description: 返回栈顶元素
 * @param {*}
 * @return {int}: 栈顶元素的值
 */
int stack::top()
{
    if (stack::empty())
    {
        throw std::runtime_error("you can't use top() in a empty stack");
    }
    return _array[_top];
}
/**
 * @description: 弹栈操作
 * @param {*}
 * @return {*}
 */
void stack::pop()
{
    if (_top == -1)
    {
        throw std::runtime_error("you can't use pop() in a empty stack");
    }
    _top--;//只需将栈顶位置在数组中的下标-1即可
    if ((_top + 1) * 4 <= _capacity)//如果数组中实际使用的空间不到分配空间的4分之1时，我们需要释放capacity/2的空间
    {
        allocate(_capacity / 2);
    }
}
/**
 * @description: 压栈操作
 * @param {int &} value:待压入栈中的值
 * @return {*}
 */
void stack::push(const int & value)
{
    if(_top + 1 == _capacity)//当当前空间已经用完时，需要分配新空间
    {
        allocate(2 * _capacity);
    }
    _array[++_top] = value;
}
/**
 * @description: 栈的空间分配函数
 * @param {int &} capacity:待分配空间的值
 * @return {*}
 */
void stack::allocate(const int & capacity)
{
    _capacity = capacity;
    int* temp = new int[_capacity];//先分配信的一段内存
    for (size_t i = 0; i <= _top; i++)
    {
        temp[i] = _array[i];//复制
    }
    delete [] _array;//释放原来的内存
    _array = temp;//指向新的内存
}