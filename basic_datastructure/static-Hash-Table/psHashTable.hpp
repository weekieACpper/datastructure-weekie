/*
 * @Author: weekie
 * @Date: 2022-03-23 14:49:54
 * @LastEditTime: 2022-04-11 10:27:34
 * @LastEditors: weekie
 * @Description: 开放寻址法散列表的接口声明部分
 * @FilePath: /basic_datastructre/static-Hash-Table/psHashTable.hpp
 */
#ifndef __PSHASHTABLE_HPP__
#define __PSHASHTABLE_HPP__
#include <vector>
#include <iostream>
/**
 * @class: 开放寻址法散列表的ADT描述 
 */
class psHashTable
{
public:
    psHashTable();
    ~psHashTable();
public:
    std::pair<int, bool> find(int k);
    void insert(int k);
private:
    int hash(int k , int index);
private:
    std::vector<std::pair<int, bool> > _table;
    int _tableSize;
};
#endif