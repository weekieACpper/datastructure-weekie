/*
 * @Author: weekie
 * @Date: 2022-03-02 20:54:57
 * @LastEditTime: 2022-03-03 09:43:12
 * @LastEditors: weekie
 * @Description: 链表接口的实现
 * @FilePath: /datastructure/basic_datastructre/linkedList/SinglyLinkedList/SinglyLinkedList.cpp
 */
#include<SinglyLinkedList.hpp>
#include<stdexcept>
#include<iostream>
/**
 * @description: 链表的构造函数，初始化headNode
 * @param {*}
 * @return {*}
 */
linkedList::linkedList():listSize(0)
{   
    headNode = new Node();//构造HeadNode结点
}
/**
 * @description: 链表的析构函数
 * @param {*}
 * @return {*}
 */
linkedList::~linkedList()
{ 
    //析构函数只需要在clear的基础上释放头结点空间即可。
    clear();
    delete headNode;
}
/**
 * @description: 判断链表是否为空
 * @param {*}
 * @return {bool}: 如果量表为空，返回true
 */
bool linkedList::empty()
{
    return (listSize == 0);
}
/**
 * @description: 返回链表中结点的个数
 * @param {*}
 * @return {int}: 链表中结点的个数
 */
int linkedList::size()
{
    return listSize;
}
/**
 * @description: 清楚链表中含有关键字的所有结点
 * @param {*}
 * @return {*}
 */
void linkedList::clear()
{
    Node* targetNode = headNode->nextNode;//先指向第一个结点
    while(targetNode != nullptr)
    {
        Node* tempNode = targetNode->nextNode;//先用临时结点保存下一个集诶单
        delete targetNode;//删除当前结点
        targetNode = tempNode;//赋值变为下个结点
    }
    listSize = 0;//结点数量清为0.
}
/**
 * @description: 找到第一个关键字为Key的结点
 * @param {int &} key:待查找的关键字
 * @return {Node}: 第一个含有该关键字的结点
 */
Node* linkedList::findFirst(const int & key)
{
    Node* targetNode = headNode->nextNode;//先获取还有第一个关键字的结点
    while(targetNode != nullptr && targetNode->key != key)
    {
        targetNode = targetNode->nextNode;//如果没找到或不是空结点，就不停的指向下个结点
    }
    return targetNode;//如果没找到，那么targetNode为空指针。
}
/**
 * @description: 在索引处，插入关键字
 * @param {int} index:对应于数组中的索引
 * @param {int &} key:待插入的关键字
 * @return {*}
 */
void linkedList::insert(int index, const int & key)
{
    if (index < 0 || index > listSize)//越界检查
    {
        throw std::runtime_error("index error !");
    }
    Node* preNode = headNode;
    //以下遍历到索引处位置的前一个结点
    for (size_t i = 0; i < index ; i++)
    {
        preNode = preNode->nextNode;
    }
    preNode->nextNode = new Node(key, preNode->nextNode);//构造新的结点
    listSize++;
}
/**
 * @description: 在链表的头部插入关键字
 * @param {int &} key:待插入关键字
 * @return {*}
 */
void linkedList::pushFront(const int & key)
{
    insert(0, key);//我们可以直接调用insert完成这个操作。
}
/**
 * @description: 在链表尾部插入关键字
 * @param {int &} key:待插入的关键字
 * @return {*}
 */
void linkedList::pushBack(const int & key)
{
    insert(listSize , key);//同样，我们可以调用insert完成这个操作。
}
/**
 * @description: 删除在链表中索引为index所在的结点
 * @param {int} index:索引
 * @return {*}
 */
void linkedList::erase(int index)
{
    if (index < 0 || index >= listSize)//越界检查
    {
        throw std::runtime_error("index error!");
    }
    Node* preNode = headNode;
    //以下遍历到索引处位置的前一个结点
    for (size_t i = 0; i < index; i++)
    {
        preNode = preNode->nextNode;
    }
    Node* tempNode = preNode->nextNode;//保存待删除的结点
    preNode->nextNode = preNode->nextNode->nextNode;//将待删除结点的前一个结点的下一个结点设为待删除结点的后一个结点
    delete tempNode;//释放待删除结点的空间
    listSize--;
}
/**
 * @description: 删除链表中所有关键字为key的结点
 * @param {int &} key:待删除的关键字
 * @return {*}
 */
void linkedList::eraseKey(const int & key)
{
    //首先要寻找到含有该key的结点
    Node *preNode = headNode, *targetNode = headNode->nextNode;
    while(targetNode != nullptr)
    {
        if(targetNode->key == key)
        {
            preNode->nextNode = preNode->nextNode->nextNode;
            delete targetNode;
            targetNode = preNode->nextNode;
            listSize--;
            if (targetNode == nullptr)//如果刚好删除的是最后一恶搞结点，我们需要跳出循环
            {
                break;
            }
        }
        preNode = targetNode;
        targetNode = targetNode->nextNode;
    }   
}
/**
 * @description: 删除链表的第一个结点
 * @param {*}
 * @return {*}
 */
void linkedList::popFront()
{
    erase(0);//借用erase实现这个操作
}
/**
 * @description: 删除链表最后一个结点
 * @param {*}
 * @return {*}
 */
void linkedList::popBack()
{
    erase(listSize - 1);//借用erase实现这个操作
}
/**
 * @description: 反转链表
 * @param {*}
 * @return {*}
 */
void linkedList::reverse()//反转链表
{
    if (empty())
    {
        return; //空链表无需反转
    }
    Node *preNode = headNode->nextNode, *targetNode = headNode->nextNode->nextNode;
    preNode->nextNode = nullptr;//先把第一个结点的下个结点职位空
    while (targetNode != nullptr)
    {
        Node* tempNode = targetNode->nextNode;//保存targetNode的下一个结点
        targetNode->nextNode = preNode;
        preNode = targetNode;//preNode移动到targetNode位置
        targetNode = tempNode;//targetNode移动到其后位置
    }
    headNode->nextNode = preNode;//重置headNode的下一恶搞结点指针
}
/**
 * @description: 打印链表中的元素
 * @param {*}
 * @return {*}
 */
void linkedList::print()
{
    Node* targetNode = headNode->nextNode;
    while(targetNode != nullptr)
    {
        std::cout << targetNode->key << " " ;
        targetNode = targetNode->nextNode;
    }
}