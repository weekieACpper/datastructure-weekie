/*
 * @Author: weekie
 * @Date: 2022-02-25 21:18:18
 * @LastEditTime: 2022-02-28 20:33:52
 * @LastEditors: weekie
 * @Description: 队列的接口声明文件
 * @FilePath: /datastructure/queue/Queue.hpp
 */
#ifndef __QUEUE_HPP__
#define __QUEUE_HPP__
/**
 * @description: 队列的ADT表述
 */
class Queue
{
public:
    Queue();//构造函数
    Queue(const int & capacity);//构造函数
    ~Queue();//析构函数
public:
    bool empty();//半段队列是否为空
    int size();//返回队列中元素的个数
    int capacity();//返回队列总空间的大小
    int back();//返回队尾元素
    int front();//返回队首元素
    void push(const int & value);//在队尾插入元素
    void pop();//在队首弹出元素
private:
    void allocate(const int & newCapacity);//分配空间
private:
    int _head;//队首
    int _tail;//队尾
    int _capacity;//素组的实际空间
    int _size;//元素个数,这里其实不需要，也可以用_head-_tail酸醋元素个数
    int* _array;//数组指针
};
#endif