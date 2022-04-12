/*
 * @Author: weekie
 * @Date: 2021-12-18 18:48:08
 * @LastEditTime: 2022-01-12 15:53:43
 * @LastEditors: weekie
 * @Description: 2-3树以及树结点的声明部分
 * @FilePath: /datastructure/2-3tree/2_3tree.hpp
 */
#ifndef __2_3TREE_HPP
#define __2_3TREE_HPP
//以下为宏声明
#define MAX_ELEMENT_NUM_PER_NODE 2//定义每个结点最大关键字个数
#define MAX_POINT_NUM_PER_NODE 3//定义每个结点最大孩子指针的个数
#include<iostream>
/**
 * @brief:方便起见，这里结点的元素都使用了int类型作为测试，不使用template
 */
/**
 * @struct:树结点结构体，用以表示2-3树中的一个结点
 * @description: 
 */
struct TreeNode
{
    TreeNode():nodeSize(0),isLeaf(false){}//结点的构造函数
    int nodeSize;//记录结点上值的个数
    bool isLeaf;//判断是否为叶子结点，true为叶子结点
    int valueList[MAX_ELEMENT_NUM_PER_NODE+1];//该结点的元素列表，按照由高到低排序
    TreeNode* pointList[MAX_POINT_NUM_PER_NODE+1];//指针列表，指向孩子结点
};
/**
 * @class:twoThreeTree
 * @description:2-3tree的ADT表示
 */
class twoThreeTree
{
public:
    twoThreeTree();//构造函数
    ~twoThreeTree();//析构函数
public:
    bool empty();
    int size();
    std::pair<TreeNode*, int> find(const int & value);//查找操作
    void insert(const int & value);//插入操作
    void erase(const int & value);//删除操作
    void print();//中序遍历打印这棵2-3树
private:
    void auxDestructor(TreeNode* Node);//析构函数的辅助函数，后序遍历析构每个节点
    void auxPrint(TreeNode* Node);//中序遍历打印每个节点 
    TreeNode* auxInsert(TreeNode* node, const int & value);
    TreeNode* auxErase(TreeNode* targetNode, const int & value);//erase的辅助函数
    void splitNode(TreeNode* parentNode, int index);//分裂结点
    void mergeNode(TreeNode* parentNode,int index);//合并结点
private:
    TreeNode* _root;//根节点
    int _treeSize;//树中元素的总数量
};
#endif