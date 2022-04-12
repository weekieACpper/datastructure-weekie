/*
 * @Author: weekie
 * @Date: 2022-03-22 10:46:38
 * @LastEditTime: 2022-04-11 10:28:46
 * @LastEditors: Please set LastEditors
 * @Description: 链接法散列表的接口实现部分
 * @FilePath: /basic_datastructre/static-Hash-Table/Hashtable.cpp
 */
#include <HashTable.hpp>
/**
 * @description: 构造函数
 * @param {*}
 * @return {*}
 */
HashTable::HashTable():_tableSize(11),_table(11){}
/**
 * @description: 析构函数，释放槽所对应链表的空间
 * @param {*}
 * @return {*}
 */
HashTable::~HashTable()
{
    //对于每个槽中的链表释放空间。
    for (auto i = _table.begin(); i != _table.end(); i++)
    {
        Node* targetNode = *i;
        while (targetNode != nullptr)
        {
            Node* deleteNode = targetNode;
            targetNode = targetNode->next;
            delete targetNode;
        }
    }
}
/**
 * @description: 散列函数,用于将关键字散列到对应的槽
 * @param {int} key:关键字的值
 * @return {int}: 返回关键字将要散列到的槽的编号
 */
int HashTable::hash(int key)
{
    return key % _tableSize;
}
/**
 * @description: 查找函数，更具关键字的值返回其在链接法散列表中对应的结点
 * @param {int} key:关键字的值
 * @return {Node *}:关键字所在的结点
 */
Node* HashTable::find(int key)
{
    Node* targetNode = _table[hash(key)];//获取该槽内链表的头结点
    while (targetNode != nullptr && targetNode->key != key)
    {
        targetNode = targetNode->next;
    }
    return targetNode;
}
/**
 * @description: 插入函数，将关键字插入到其对应槽的链表中
 * @param {int} key:关键字的值
 * @param {int} value: 键值对中的值
 * @return {*}
 */
void HashTable::insert(int key, int value)
{
    int index = hash(key);
    //在链表的头结点插入
    Node* newNode = new Node(key, value , nullptr , _table[index]);
    _table[index] = newNode;//槽更改为存储新的头结点
}
/**
 * @description: 删除函数，删除该关键字所对应的结点
 * @param {int} key:关键字
 * @return {*}
 */
void HashTable::erase(int key)
{
    Node* targetNode = find(key);//先找到该结点
    if (targetNode == nullptr)//如果该结点为空，直接返回
    {
        return ;
    }
    if (targetNode->pre != nullptr)
    {
        targetNode->pre->next = targetNode->next;//其前结点的next指针指向targetNode后一个结点        
    }
    else
    {
        _table[hash(key)] = targetNode->next;//更改新的头结点。
    }
    delete targetNode;
}
