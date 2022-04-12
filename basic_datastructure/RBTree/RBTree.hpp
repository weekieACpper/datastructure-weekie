/*
 * @Author: weekie
 * @Date: 2021-12-26 17:07:05
 * @LastEditTime: 2022-01-08 10:36:58
 * @LastEditors: Please set LastEditors
 * @Description: 红黑树结点以及其ADT类型的声明部分
 * @FilePath: /datastructure/RBtree/RBTree.hpp
 */
#ifndef __RBTREE_HPP
#define __RBTREE_HPP
#include<vector>
/**
 * @struct: 红黑树结点的定义
 */
struct TreeNode
{
    TreeNode():color(true),leftChild(nullptr),rightChild(nullptr){}//默认构造函数
    TreeNode(const int x):color(true),value(x),leftChild(nullptr),rightChild(nullptr){}//接收关键字值的构造函数
    bool color;//结点的颜色红色为true，黑色为false
    int value;//关键字
    TreeNode* leftChild;//该结点的左孩子
    TreeNode* rightChild;//该结点有孩子·
};
/**
 * @class: 红黑树的ADT描述
 */
class RBTree
{
public:
    RBTree();//构造函数
    ~RBTree();//析构函数
public:
    int size();//返回树的大小
    bool empty();//判断树是否为空
    bool isRBTree();//判断是否为红黑树
    TreeNode* find(const int & value);//寻找关键字为value的关键字
    std::vector<TreeNode * > rangeQuery(const int & value1, const int & value2);//范围查找，返回待查找关键字范围内的所有结点
    void insert(const int & value);//插入操作
    void recursiveInsert(const int & value);//插入递归版本
    void erase(const int & value);//删除操作
    void recursiveErase(const int & value);//递归版本的删除操作
    void print();//中序遍历打印这棵红黑树树
private:
    bool isRed(TreeNode* node);//判断该结点是否死红色结点
    bool is2_Node(TreeNode* node);//判断该结点是否是2-结点
    bool isOrdered(TreeNode* node);//判断是否有序
    bool hasConsequentRedNode(TreeNode* node);//检查是否含有连续的红色结点
    bool isBST(TreeNode* node);//判断是否为二叉搜索树
    int countNode(TreeNode* node);//统计结点数量
    int countBlackNode(TreeNode* node);//判断每条路径上的黑色结点是否相等
    void flipColor(TreeNode* node);//与分裂等效颜色转换
    void flipColor2(TreeNode* node);//与合并等效的颜色转换
    TreeNode* rightRotation(TreeNode* node);//右旋转函数
    TreeNode* leftRotation(TreeNode* node);//左旋转函数
    TreeNode* auxRecursiveInsert(TreeNode* node,const int & value);//递归版本插入函数的辅助函数
    TreeNode* auxRecursiveErase(TreeNode* node,const int & value);//递归版本删除函数的辅助函数
    void auxPrint(TreeNode* node);//辅助打印函数
    void auxDestructor(TreeNode* node);//辅助析构函数
private:
    TreeNode* _sentinelNode;//哨兵结点
    TreeNode* _root;//红黑树的根结点
    int _treeSize;//红黑树中元素的数量
};
#endif