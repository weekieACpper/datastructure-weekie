/*
 * @Author: weekie
 * @Date: 2022-03-22 10:45:25
 * @LastEditTime: 2022-04-11 10:07:35
 * @LastEditors: weekie
 * @Description: 链接法散列表的声明文件
 * @FilePath: /basic_datastructre/static-Hash-Table/Hashtable.hpp
 */
#ifndef __HASHTABLE_HPP__
#define __HASHTABLE_HPP__
#include <vector>
/**
 * @struct:链接法散列表的链表结点 
 */
struct Node//链表结点
{
    Node():next(nullptr),pre(nullptr){}
    Node(const int & k, const int & v):key(k),value(v),next(nullptr),pre(nullptr){}
    Node(const int & k, const int & v, Node* preNode, Node* nextNode):key(k),value(v),pre(preNode),next(nextNode){}
    int key;//关键字
    int value;//值
    Node* next;//之后的结点
    Node* pre;//之前的结点
};
/**
 * @class: 链接法散列表的ADT描述 
 */
class HashTable
{
public:
    HashTable();//构造函数
    ~HashTable();//析构函数
public:
    Node* find(int key);//查找函数
    void insert(int key, int value);//插入函数
    void erase(int key);//删除函数
private:
    int hash(int key);//散列函数
private:
    std::vector<Node* > _table;//散列表
    int _tableSize;//散列表中槽的数量
};
#endif