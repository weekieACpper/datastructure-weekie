/*
 * @Author: weekie
 * @Date: 2021-12-31 15:51:09
 * @LastEditTime: 2022-01-07 12:15:22
 * @LastEditors: weekie
 * @Description: 2-3-4树ADT类型和插入删除等操作的声明部分。2_3_4树相当于t=2的B树
 * @FilePath: /datastructure/2_3_4tree/2_3_4tree.cpp
 */
#ifndef __2_3_4TREE_HPP
#define __2_3_4TREE_HPP
#include<iostream>
#define MINIMUM_DEGREE 2
/**
 * @struct: B树结点结构体
 */
struct TreeNode
{
    TreeNode():nodeSize(0),isLeaf(false){}//构造函数
    int nodeSize;//当前结点关键字个数
    bool isLeaf;//判断是否为叶子结点，如果是为true，反之为false
    int valueList[2 * MINIMUM_DEGREE - 1];//关键字数组，个数为最大2t-1
    TreeNode* pointList[2 * MINIMUM_DEGREE];//指向孩子结点的指针数组，一个结点最大拥有2t个孩子结点
};
/**
 * @class: B树的ADT类型声明 
 */
class BTree
{
public:
    BTree();//构造函数
    ~BTree();//析构函数
public:
    int size();//返回树的大小
    bool empty();//判断树是否为空
    std::pair<TreeNode*,int> find(const int & value);//查找操作
    std::pair<TreeNode*,int> recursiveFind(const int & value);
    void insert(const int & value);//插入操作
    void erase(const int & value);//删除操作
    void print();//中序遍历打印这棵B树
private:
    void auxPrint(TreeNode* node);//辅助打印函数
    void auxDestructor(TreeNode* node);//辅助析构函数
    void auxErase(TreeNode* node,int index);//处理情况3
    std::pair<TreeNode*,int> auxRecursiveFind(TreeNode* node,const int & value);//递归查找辅助函数
    void splitTreeNode(TreeNode* targetNode,int index);//分裂结点
    void mergeTreeNode(TreeNode* targetNode,int index);//合并结点
private:
    TreeNode* _root;//根节点
    int _elementNum;//树中关键字的个数
};
#endif
