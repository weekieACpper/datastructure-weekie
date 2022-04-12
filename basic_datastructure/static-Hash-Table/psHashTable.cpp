/*
 * @Author: weekie
 * @Date: 2022-03-23 14:50:08
 * @LastEditTime: 2022-04-11 10:34:12
 * @LastEditors: weekie
 * @Description: 开放寻址法散列表的接口实现部分
 * @FilePath: /basic_datastructre/static-Hash-Table/psHashTable.cpp
 */
#include <psHashTable.hpp>
#include <stdexcept>
/**
 * @description: 构造函数
 * @param {*}
 * @return {*}
 */
psHashTable::psHashTable():_table(11,{0,true}),_tableSize(11){}
/**
 * @description: 析构函数
 * @param {*}
 * @return {*}
 */
psHashTable::~psHashTable(){}
/**
 * @description: 散列函数，用于将关键字散列到对应的槽
 * @param {int} k:关键字的值
 * @param {int} index:探查值
 * @return {int}: 返回对应的槽编号
 */
int psHashTable::hash(int k, int index)
{
    return (k % _tableSize + 2 * index + index * index) % _tableSize;//二次探查形式
}
/**
 * @description: 插入函数，将关键字插入对应的槽
 * @param {int} k: 待插入关键字的值
 * @return {*}
 */
void psHashTable::insert(int k)
{
    int i = 0 ;
    for (; i < _tableSize; i++)
    {
        if(_table[hash(k,i)].second == true)
        {
            _table[hash(k,i)] = {k, false};//找到空槽插入元素
            return;
        }
    }
    throw std::runtime_error("HashTable overflow!");
}
/**
 * @description: 查找函数，根据关键字值的大小返回对应的槽
 * @param {int} k:关键字的值
 * @return {std::pair<int ,bool>}:槽
 */
std::pair<int, bool> psHashTable::find(int k)
{
    int i = 0;
    std::pair<int, bool> temp;
    do
    {
        temp = _table[hash(k,i)];
        if (temp.first == k)
        {
            return {k, false};
        }
        i++;
    }
    while(temp.second == true && i != _tableSize);
    return {-1, true};
}