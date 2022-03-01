/*
 * @Author: weekie
 * @Date: 2022-02-17 22:05:31
 * @LastEditTime: 2022-03-01 16:04:27
 * @LastEditors: weekie
 * @Description: 二叉搜索树的ADT描述以及树结点的定义文件
 * @FilePath: /datastructure/BStree/BSTree.hpp
 */
#ifndef __BSTREE_HPP__
#define __BSTREE_HPP__
/**
 * @description: 二叉搜索树结点定义
 */
struct TreeNode
{
    TreeNode():leftChild(nullptr),rightChild(nullptr),parent(nullptr){}
    TreeNode(const int & k):key(k),leftChild(nullptr),rightChild(nullptr),parent(nullptr){}
    TreeNode* leftChild;
    TreeNode* rightChild;
    TreeNode* parent;//非必需，指向父结点
    int key;
};
/**
 * @description: 二叉搜索树的ADT描述
 */
class BSTree
{
public:
    BSTree();//构造函数
    ~BSTree();//析构函数
public:
    bool empty();//判断树是否为空
    int size();//返回树中结点的数量
    TreeNode* find(const int & key);//查找操作
    TreeNode* getMin();//获取树中的最小关键字所在结点
    TreeNode* getMax();//获取树中最大关键字所在结点
    void insert(const int & key);//插入操作
    void erase(const int & key);//删除操作
    void preOrder(void (*f)(TreeNode* node));
    void inOrder(void (*f)(TreeNode* node));
    void postOrder(void (*f)(TreeNode* node));
    void levelOrder(void (*f)(TreeNode* node));//层序遍历
    void preOrderWithStack(void (*f)(TreeNode* node));//使用栈的前序遍历
    void morrisPreOrderTraversal(void (*f)(TreeNode* node));//莫里斯遍历(前序)
    void inOrderWithStack(void (*f)(TreeNode* node));//使用栈的中序遍历
    void morrisInOrderTraversal(void (*f)(TreeNode* node));//莫里斯遍历(中序)
    void postOrderWithStack(void (*f)(TreeNode* node));//使用栈的后序遍历
    void morrisPostOrderTraversal(void (*f)(TreeNode* node));//莫里斯遍历(后序);
private:
    static void (*visit)(TreeNode* node);//访问结点的函数
    static void visitNodeString(TreeNode* node);//访问一串结点(专门为后序遍历的莫里斯遍历设计)
    static void deleteNode(TreeNode* node);//释放耽搁结点的空间
    static TreeNode* auxReverseNode(TreeNode* node);//反转结点指针转向(专门为后序遍历设计)
private:
    TreeNode* getTreeMinimum(TreeNode* node);//获取子树或中的最小关键字所在结点
    TreeNode* getTreeMaximum(TreeNode* node);//获取子树或树中最大关键字所在结点
    TreeNode* getNodeSuccessor(TreeNode* node);//获得该结点的后继
    TreeNode* getNodePredecessor(TreeNode* node);//获得该接待你的前驱
    void preOrder(TreeNode* node);//前序遍历
    void inOrder(TreeNode* node);//中序遍历
    void postOrder(TreeNode* node);//后序遍历
private:
    TreeNode* _root;//根结点
    int _treeSize;//树中结点的数量
};
#endif