/*
 * @Author: weekie
 * @Date: 2022-03-17 15:01:32
 * @LastEditTime: 2022-04-11 11:06:18
 * @LastEditors: weekie
 * @Description: 指针实现的并查集的接口的实现部分
 * @FilePath: /datastructure/basic_datastructre/unionFind/UF_in_pointer.cpp
 */
#include <UF_in_pointer.hpp>
#include <stdexcept>
/**
 * @description: 构造函数
 * @param {*}
 * @return {*}
 */
DisjointSet::DisjointSet(){}
/**
 * @description: 析构函数，释放并查集中每个结点的空间
 * @param {*}
 * @return {*}
 */
DisjointSet::~DisjointSet()
{
    //这里需要注意的是标准库的容器是不会释放原生指针动态申请的内存的
    //我们需要借助unordered_map的迭代器主动释放内存
    for (auto i = HashTable.begin(); i != HashTable.end(); i++)
    {
        delete i->second;//释放内存
    }
};
/**
 * @description: 在不相交集合中，建立一个新集合
 * @param {int} a:新集合中的一个1结点
 * @return {*}
 */
void DisjointSet::makeSet(int a)
{
    Node* newNode = new Node(a);
    HashTable.insert({a, newNode});
}
/**
 * @description: 对数组中的每一个结点建立一个新集合
 * @param {const std::vector<int> &} a: 结点数组
 * @return {*}
 */
void DisjointSet::makeSet(const std::vector<int> & a)
{
    for (auto i = a.begin(); i != a.end(); i++)
    {
        Node* newNode = new Node(*i);
        HashTable.insert({*i, newNode});
    }
}
/**
 * @description: 根据给定的值，查到这个值所在的集合，
 * @param {int} i:给定的值
 * @return {Node*}:代表集合的根结点
 */
Node* DisjointSet::find(int i)
{
    auto result = HashTable.find(i);//获取查找结果
    if (result == HashTable.end())//如果集合中未含此元素，返回空指针。
    {
        return nullptr;
    }
    Node* targetNode = result->second;//获得元素对应的Node
    while(targetNode->parent != nullptr)//寻找到期代表元素。
    {
        targetNode = targetNode->parent;
    }
    return targetNode;//返回代表元素
}
/**
 * @description: 将两个集合合并
 * @param {Node*} a:第一个集合的根结点
 * @param {Node*} b:第二个集合的根结点
 * @return {*}
 */
void Union(Node* a, Node* b)
{
    if (a == nullptr || b == nullptr)
    {
        throw std::invalid_argument("argument can't be nullptr");
    }
    if (a->rank > b->rank)//取集合中含有元素较多的集合的代表元素作为新集合的代表元素
    {
        b->parent = a; 
        a->rank = std::max(b->rank + 1 , a->rank);
    }
    else
    {
        a->parent = b;
        b->rank = std::max(a->rank + 1, b->rank);
    }
}

