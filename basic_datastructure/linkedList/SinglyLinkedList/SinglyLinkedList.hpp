/*
 * @Author: weekie
 * @Date: 2022-03-02 20:55:10
 * @LastEditTime: 2022-03-04 09:03:51
 * @LastEditors: weekie
 * @Description: 链表和链表结点接口声明
 * @FilePath: /datastructure/basic_datastructre/linkedList/SinglyLinkedList/SinglyLinkedList.hpp
 */
#ifndef __SINGLYLINKEDLIST_HPP__
#define __SINGLYLINKEDLIST_HPP__
/**
 * @description: 链表结点的定义
 */
struct Node
{
    Node():nextNode(nullptr){}
    Node(const int & k):key(k), nextNode(nullptr){}
    Node(const int & k, Node* n):key(k), nextNode(n){}
    int key;//关键字元素，我们假设关键字类型为int，之后也是。
    Node* nextNode;//指向下一个结点的指针
};
/**
 * @description: 链表的ADT描述
 */
class linkedList
{
public:
    linkedList();//构造函数
    ~linkedList();//析构函数
public:
    bool empty();//判断链表是否为空
    int size();//返回链表中结点的个数
    Node* findFirst(const int & key);//查找函数
    void insert(int index, const int &  key);//在索引为index中插入元素
    void erase(int index);//删除索引为index的元素；
    void eraseKey(const int & key);//删除关键字为key的元素
    void pushFront(const int & key);//在链表的头部插入关键字为key的元素
    void pushBack(const int & key);//在链表的尾部插入关键字为key的元素
    void popFront();//删除链表最末端结点
    void popBack();//删除链表最后结点
    void reverse();//反转链表
    void clear();//清除链表中的所有含有关键字的元素
    void print();//打印链表
private:
    Node* headNode;//头结点
    int listSize;//链表中的结点个数
};
#endif