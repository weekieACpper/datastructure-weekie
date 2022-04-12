/*
 * @Author: weekie
 * @Date: 2022-03-16 13:52:40
 * @LastEditTime: 2022-04-11 10:46:34
 * @LastEditors: weekie
 * @Description: 简单并查集的接口声明部分
 * @FilePath: /datastructure/basic_datastructre/unionFind/UF_in_array.hpp
 */
#ifndef __UF_IN_ARRAY_HPP__
#define __UF_IN_ARRAY_HPP__
#include <vector>
/**
 * @struct: 描述不相交集合中的每个集合 
 */
struct Set
{
    Set(int k):name(k){}
    std::vector<int> array;//用于存储其中元素在大数组中的索引。
    int name;//代表集合的名字。
};
/**
 * @class: 并查集的ADT描述
 */
class DisjointSet
{
public:
    DisjointSet();//构造函数
    ~DisjointSet();//析构函数
public:
    void makeSet();//建立一个新集合
    void makeSet(int n);//建立n个新新集合
    int find(int i);//输入待查询元素在数组中的编号来获取其所在的集合名字
    void Union(int a, int b);//合并操作
private:
    std::vector<int> _array;//大小为元素的数目，里面存放和每个元素所在集合的名字
    std::vector<Set> _setList;//用于存放集合，我们可以通过集合的名字快速找到这个集合。
};
#endif