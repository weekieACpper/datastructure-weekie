/*
 * @Author: weekie
 * @Date: 2022-02-11 23:24:25
 * @LastEditTime: 2022-02-28 18:18:06
 * @LastEditors: weekie
 * @Description: AVL树的ADT描述
 * @FilePath: /datastructure/AVLtree/AVLTree.hpp
 */
#ifndef __AVLTREE_HPP__
#define __AVLTREE_HPP__
struct TreeNode
{
    TreeNode():leftChild(nullptr),rightChild(nullptr),height(1){}
    TreeNode(const int & x):leftChild(nullptr),rightChild(nullptr),value(x),height(1){}
    TreeNode* leftChild;
    TreeNode* rightChild;
    int value;
    int height;
};
class AVLTree
{
public:
    AVLTree();
    ~AVLTree();
public:
    int size();
    bool empty();
    TreeNode* find(const int & value);//查找操作
    void insert(const int & value);//插入操作
    void erase(const int & value);//删除操作
    void print();//打印操作
private:
    void auxPrint(TreeNode* node);//辅助打印函数
    TreeNode* auxErase(TreeNode* node,const int & value, TreeNode* storedNode);//删除函数的辅助函数
    TreeNode* leftRotation(TreeNode* node);//左旋转
    TreeNode* rightRotation(TreeNode* node);//右旋转
    int getNodeHeight(TreeNode* node);//获得结点的高度
    void auxDestructor(TreeNode * node);//辅助析构函数
private:
    TreeNode* _sentinelNode;//哨兵结点
    TreeNode* _root;//根结点
    int _treeSize;//树中结点的数量
};
#endif