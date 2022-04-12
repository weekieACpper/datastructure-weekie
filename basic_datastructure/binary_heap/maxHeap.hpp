/*
 * @Author: weekie
 * @Date: 2022-03-04 09:06:01
 * @LastEditTime: 2022-03-04 19:49:23
 * @LastEditors: weekie
 * @Description: 二叉堆接口的声明部分
 * @FilePath: /datastructure/basic_datastructre/binary_heap/maxHeap.hpp
 */
#ifndef __MAXHEAP_HPP__
#define __MAXHEAP_HPP__
/**
 * @description: 大根堆的ADT描述
 */
class maxHeap
{
public:
    maxHeap();//构造函数
    maxHeap(int capacity);//带一个参数的构造函数
    maxHeap(int* array, int size);//给定数组建堆
    ~maxHeap();//析构函数
public:
    bool empty();//判断堆是否为空
    bool isHeap();//判断当前是否是堆
    int size();//返回堆中元素的个数
    int capacity();//返回数组空间大小
    int findMax();//返回堆中最大元素
    void createHeap(int* array, int size);//floyd算法自底向上建堆
    int extractMax();//获取堆中最大元素，并将该元素从堆中删除
    void deleteMax();//删除堆中最大元素
    void insert(const int & key);//插入操作
    void replace(const int & key);//弹出最大关键字元素，并插入一个新元素
    void print();
private:
    void siftUp(int index);//向上调整使其满足二叉堆的性质
    void siftDown(int index);//向下调整满足二叉堆的性质
    void allocate(int newCapacity);//分配新的数组空间
private:
    int *_array;//存放堆中元素的数组
    int _heapSize;//堆中元素的数量
    int _capacity;//数组空间大小
};
#endif