/*
 * @Author: weekie
 * @Date: 2022-03-17 15:01:47
 * @LastEditTime: 2022-04-11 10:56:57
 * @LastEditors: weekie
 * @Description: 指针实现的并查集的接口声明部分
 * @FilePath: /datastructure/basic_datastructre/unionFind/UF_in_pointer.hpp
 */
#ifndef __UF_IN_POINTER_HPP__
#define __UF_IN_POINTER_HPP__
#include <unordered_map>
#include <vector>
/**
 * @struct: 集合中的结点 
 */
struct Node
{
    Node(int k):key(k),rank(0),parent(nullptr){}//构造函数
    int key;//元素值
    int rank;//记录秩
    Node* parent;//指向父结点的指针
};
/**
 * @class:并查集的ADT描述 
 */
class DisjointSet
{
public:
    DisjointSet();//构造函数
    ~DisjointSet();//析构函数
public:
    void makeSet(int a);//建立一个新集合，参数为这个集合的初始元素值
    void makeSet(const std::vector<int> & a);//为n个元素建立n个集合
    Node* find(int i);//输入待查询元素在数组中的编号来获取其所在的集合名字
    void Union(Node* a, Node* b);//合并操作
private:
    std::unordered_map<int, Node*> HashTable;//存放元素的hash表
};
#endif
 