/*
 * @Author: weekie
 * @Date: 2022-03-04 09:06:12
 * @LastEditTime: 2022-03-04 19:48:53
 * @LastEditors: weekie
 * @Description: 二叉堆接口的实现部分
 * @FilePath: /datastructure/basic_datastructre/binary_heap/minHeap.cpp
 */
#include<maxHeap.hpp>
#include<iostream>
#include<stdexcept>
/**
 * @description: 构造函数
 * @param {*}
 * @return {*}
 */
maxHeap::maxHeap():_heapSize(0),_capacity(10)
{
    _array = new int[10];//默认分配10个元素
}
/**
 * @description: 对大根堆指定使用空间的构造函数
 * @param {int} capacity:指定大根堆所使用的空间
 * @return {*}
 */
maxHeap::maxHeap(int capacity):_heapSize(0), _capacity(capacity)
{
    _array = new int[capacity];
}
/**
 * @description: 给定一个数组，在这个数组的基础上构造大根堆
 * @param {int *} array:待堆化的数组
 * @param {int} size:数组大小
 * @return {*}
 */
maxHeap::maxHeap(int * array, int size):_array(nullptr)
{
    createHeap(array, size);
}
/**
 * @description: 析构函数
 * @param {*}
 * @return {*}
 */
maxHeap::~maxHeap()
{
    delete [] _array;//释放数组空间
}
/**
 * @description: 判断当前堆是否为空
 * @param {*}
 * @return {bool}:如果为空返回true
 */
bool maxHeap::empty()
{
    return _heapSize == 0;
} 
/**
 * @description: 判断是否为为二叉堆
 * @param {*} 
 * @return {bool}: 如果是返回true
 */
bool maxHeap::isHeap()
{
    for (size_t  i = _heapSize / 2; i >= 1; i--)
    {
        int index = i;
        int childIndex = index * 2;
        while(childIndex <= _heapSize)
        {
            if (_array[childIndex] > _array[index] || (childIndex + 1 <= _heapSize && _array[childIndex + 1] > _array[index]))
            {
                return false;
            }
            index = childIndex;
            childIndex = index * 2;
        }
    }
    return true;
}
/**
 * @description:返回堆中姐关键字数量
 * @param {*}
 * @return {int}:堆中关键字数量
 */
int maxHeap::size()
{
    return _heapSize;
}
/**
 * @description: 获取堆使用的空间大小
 * @param {*}
 * @return {int}:堆所使用的空间大小
 */
int maxHeap::capacity()
{
    return _capacity;
}
/**
 * @description: 获取堆中的最大的关键字
 * @param {*}
 * @return {int}:关键字
 */
int maxHeap::findMax()
{
    return _array[1];//返回索引为1的元素即可
}
/**
 * @description: 使用floyd算法建堆
 * @param {int*} array:待堆化的数组
 * @param {int} size:数组的大小
 * @return {*}
 */
void maxHeap::createHeap(int* array, int size)
{
    if(_array != nullptr)
    {
        delete [] _array;
    }
    _heapSize = size;
    _capacity = size << 1;//左移1位相当于乘2
    _array = new int[2 * size];
    for(int i = 0; i < size; i++)
    {
        _array[i + 1] = array[i];
    }//拷贝；
    for (size_t i = _heapSize / 2; i >= 1; i--)
    {
        siftDown(i);
    }
}
/**
 * @description: 插入函数
 * @param {int &} key:待插入的关键字
 * @return {*}
 */
void maxHeap::insert(const int & key)
{
    //如果空间满了，我们需要分配空间，需要注意的是
    //由于我们索引0的空间是空着的，所以当_heapSize == _capacity - 1
    //就应该分配空间
    if (_heapSize + 1 == _capacity)
    {
        allocate(2 * _capacity);
    }
    _array[++_heapSize] = key;
    siftUp(_heapSize);//向上调整堆
}
/**
 * @description: 提取堆中的最大关键字并删除
 * @param {*}
 * @return {int}:堆中最大的关键字
 */
int maxHeap::extractMax()
{
    if(empty())
    {
        throw std::runtime_error("you can't use the extractMax() in the empty binary heap!");
    }
    int result = _array[1];//提取首元素
    //我们将数组最后一个元素覆盖第一个元素
    _array[1] = _array[_heapSize--];
    siftDown(1);//下降调整
    //如果数组中元素很少，但数组空间又非常大时，需要释放部分空间
    if (_heapSize * 4 <  _capacity)
    {
        allocate(_capacity / 2);
    }
    return result;
}
/**
 * @description: 删除堆中最大的关键字
 * @param {*}
 * @return {*}
 */
void maxHeap::deleteMax()
{
    if(empty())
    {
        throw std::runtime_error("you can't use the deleteMax() in empty binary heap!");
    }
    //我们将数组最后一个元素覆盖第一个元素
    _array[1] = _array[_heapSize--];
    siftDown(1);//下降调整
    //如果数组中元素很少，但数组空间又非常大时，需要释放部分空间
    if (_heapSize * 4 <  _capacity)
    {
        allocate(_capacity / 2);
    }
}
/**
 * @description: 删除堆中的最大关键字，并用另一个关键字代替
 * @param {int &} key:代替的关键字
 * @return {*}
 */
void maxHeap::replace(const int & key)
{
    if(empty())
    {
        throw std::runtime_error("you can't use the replace() in empty binary heap!");
    }
    _array[1] = key;//直接将首元素替换
    siftDown(1);
}
/**
 * @description: 打印堆中的宿友关键字
 * @param {*}
 * @return {*}
 */
void maxHeap::print()
{
    for (size_t i = 1; i <= _heapSize; i++)
    {
        std::cout << _array[i] << " ";
    }
}
/**
 * @description: 向上堆化
 * @param {int} index:待堆化的起始点的索引
 * @return {*}
 */
void maxHeap::siftUp(int index)
{
    int parentIndex = index / 2;//获取父结点的索引
    while(parentIndex >= 1)
    {
        //如果父结点关键字比子结点关键字小，则其破话了大根堆的性质，需要将父子结点交换
        if (_array[parentIndex] < _array[index])
        {
            std::swap(_array[parentIndex], _array[index]);
            //之后向上继续调整
            index = parentIndex;
            parentIndex = index / 2;
        }
        else//如果满足了，就无须再向上
        {
            break;
        }
    }
}
/**
 * @description: 向下堆化
 * @param {int} index:待堆化的起始点所在的索引
 * @return {*}
 */
void maxHeap::siftDown(int index)
{
    int childIndex = index * 2;
    while(childIndex <= _heapSize)
    {
        //我们需要将左右孩子结点中较大的结点提升到父结点
        if(childIndex + 1 <= _heapSize && _array[childIndex + 1] > _array[childIndex])
        {
            childIndex += 1;
        }
        //如果父结点的关键字的大小比子结点小，交换两者的值
        if(_array[index] < _array[childIndex])
        {
            std::swap(_array[index], _array[childIndex]);
        }
        else//否则已经满足条件了，无需下降
        {
            break;
        }
        index = childIndex;
        childIndex = index * 2;
    }
}
/**
 * @description: 为堆分配空间的函数
 * @param {int} newCapacity:待分配空间
 * @return {*}
 */
void maxHeap::allocate(int newCapacity)
{
    int* temp = new int[newCapacity];//创建新数组
    for (size_t i = 1; i <= _heapSize; i++)
    {
        temp[i] = _array[i];//拷贝原数组元素
    }
    delete [] _array;//释放原数组空间
    _array = temp;//调整原数组指针
    _capacity = newCapacity;
}