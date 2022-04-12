/*
 * @Author: weekie
 * @Date: 2022-03-16 13:52:59
 * @LastEditTime: 2022-04-11 10:53:47
 * @LastEditors: weekie
 * @Description: 并查集接口的实现
 * @FilePath: /datastructure/basic_datastructre/unionFind/UF_in_array.cpp
 */
#include <UF_in_array.hpp>
#include <stdexcept>
/**
 * @description: 构造函数
 * @param {*}
 * @return {*}
 */
DisjointSet::DisjointSet(){}
/**
 * @description: 析构函数
 * @param {*}
 * @return {*}
 */
DisjointSet::~DisjointSet(){}
/**
 * @description: 在不相交集合中创建一个新的集合
 * @param {*}
 * @return {*}
 */
void DisjointSet::makeSet()
{
    int k = _setList.size();//获取荡子集合的数量作为名字
    _setList.emplace_back(Set(k));//加入子集合数组
    _setList[k].array.push_back(_array.size());//添加新元素在数组中的索引
    _array.push_back(k);//添加新元素对应的集合。
}
/**
 * @description: 在不相交集合中连续创造n个新的集合。
 * @param {int} n:需要创建集合的数量。
 * @return {*}
 */
void DisjointSet::makeSet(int n)
{
    int k = _setList.size();
    //连续创建n个集合
    for (size_t i = 0; i < n; i++)
    {
        _setList.emplace_back(Set(k + i));
        _setList[k + i].array.push_back(_array.size());
        _array.push_back(k + i);
    }
}
/**
 * @description: 给定其中一个结点的编号，返回其所在集合的编号
 * @param {int} i:给定结点的编号
 * @return {int}: 集合的编号
 */
int DisjointSet::find(int i)
{
    if(i >= _array.size() || i < 0)//待查找的索引超出范围
    {
        return -1;//返回-1
    }
    return _array[i];
}
/**
 * @description: 将两个名字为a和b的集合合并
 * @param {int} a:集合a的名字
 * @param {int} b:集合b的名字
 * @return {*}
 */
void DisjointSet::Union(int a, int b)//将两个名字为a和b的集合合并
{
    if (a >= _setList.size() || a < 0 || b >= _setList.size() || b < 0)
    {
        throw std::invalid_argument(" a and b should be < total number of set and > 0");
    }
    //如果集合a中的元素较多，那我们就讲集合b中的元素所在集合的名字更新为a
    if(_setList[a].array.size() > _setList[b].array.size())
    {
        for(auto i = _setList[b].array.begin(); i != _setList[b].array.end(); i++)
        {
            _array[*i] = a;//名字更改为a。
        }
    }
    else
    {
        for(auto i = _setList[a].array.begin(); i != _setList[a].array.end(); i++)
        {
            _array[*i] = b;//名字更改为b。
        }
    }
}

