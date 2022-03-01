/*
 * @Author: weekie
 * @Date: 2022-02-25 21:18:13
 * @LastEditTime: 2022-02-28 21:07:20
 * @LastEditors: weekie
 * @Description: 队列接口的实现文件
 * @FilePath: /datastructure/queue/Queue.cpp
 */
#include<Queue.hpp>
#include<stdexcept>
/**
 * @description:构造函数
 * @param {*}
 * @return {*}
 */
Queue::Queue():_head(0),_tail(0),_capacity(10),_size(0)
{
    _array = new int[10];//默认构造函数分配10大小的空间
}
/**
 * @description: 构造函数，带有一个参数
 * @param {int &} capacity:用于指定初始队列空间大小
 * @return {*}
 */
Queue::Queue(const int & capacity):_head(0),_tail(0),_capacity(capacity),_size(0)
{
    _array = new int[capacity];
}
/**
 * @description: 析构函数
 * @param {*}
 * @return {*}
 */
Queue::~Queue()
{
    delete [] _array;//释放数组占用的内存空间
}
/**
 * @description: 用于判断队列是否为空
 * @param {*}
 * @return {bool}:为空饭后true，否则返回false
 */
bool Queue::empty()
{
    return (_tail == _head);//如果队尾位置和对手重合，说明队列为空。
}
/**
 * @description: 返回队列中元素的大小
 * @param {*}
 * @return {int}:队列中的元素个数
 */
int Queue::size()
{
    return _size;
}
/**
 * @description: 获取队列的空间
 * @param {*}
 * @return {int}:返回队列空间大小
 */
int Queue::capacity()
{
    return _capacity;
}
/**
 * @description: 返回队列中队尾元素
 * @param {*}
 * @return {int}:队尾元素
 */
int Queue::back()
{
    if (Queue::empty())
    {
        throw std::runtime_error("Queue: you can't call back() when the queue is empty");
    }
    return _array[(_tail - 1) % _capacity];
}
/**
 * @description://返回队列中队首的元素 
 * @param {*}
 * @return {int}:队首元素
 */
int Queue::front()
{
    if (Queue::empty())
    {
        throw std::runtime_error("Queue: you can't call front() when the queue is empty");
    }
    return _array[_head];
}
/**
 * @description:向队列中队尾添加元素
 * @param {int &} value:待添加的值
 * @return {*}
 */
void Queue::push(const int & value)//向队列为短添加元素
{
    if((_tail + 1) % _capacity == _head)//如果数组在增加一个就要被填满了。我们需要对数组扩容
    {
        allocate(2 * _capacity);
    }
    _array[_tail] = value;
    _tail = (_tail + 1) % _capacity;
    _size++;
}
/**
 * @description:弹出队首元素 
 * @param {*}
 * @return {*}
 */
void Queue::pop()//将队列对手元素弹出
{
    if(_tail == _head)//当_tail==_head时，队列为空，无法弹出
    {
        throw std::runtime_error("you can't use the pop() function when the queue is empty!");
    }
    _head = (_head + 1) % _capacity;
    _size--;
    if(4 * _size <  _capacity)//如果数组中元素过小，我们应当释放未被使用的空间
    {
        allocate(_capacity / 2);
    }
}
/**
 * @description:为队列分配空间的函数
 * @param {int &} newCapacity:分配空间的大小
 * @return {*}
 */
void Queue::allocate(const int & newCapacity)
{
    int* temp = new int [newCapacity];//申请新空间
    if(_head > _tail)//如果_head位置在_tail后面
    {
        //这种情况我们应该将_head的位置开始到数组最后一个位置的元素拷贝到新数组，然后再从原数组索引0开始拷贝到_tail
        int i = 0;
        for (; i < _capacity - _head; i++)
        {
            temp[i] = _array[i + _head];
        }
        for (int j = 0; j <= _tail; j++, i++)
        {
            temp[i] = _array[j];
        }
    }
    else
    {
        //如果_head<= _tail,那么可以直接将_head到_tail的元素拷贝到行数组
        for (size_t i = _head; i < _tail; i++)
        {
            temp[i - _head] = _array[i];
        }
    }
    delete [] array;
    _array = temp;//重新设置指向数组饿指针
    _head = 0;//重新设置_tail和_head
    _tail = _size;
    _capacity = newCapacity;
}