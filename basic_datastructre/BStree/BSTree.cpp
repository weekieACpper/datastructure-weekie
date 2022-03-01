/*
 * @Author: weekie
 * @Date: 2022-02-17 22:05:39
 * @LastEditTime: 2022-03-01 16:03:58
 * @LastEditors: weekie
 * @Description: 二叉搜索树的接口实现部分
 * @FilePath: /datastructure/BStree/BStree.cpp
 */
#include<BSTree.hpp>
#include<stack>
#include<queue>
void (*BSTree::visit)(TreeNode* node) = deleteNode; //静态成员
/**
 * @description: 构造函数
 * @param {*}
 * @return {*}
 */
BSTree::BSTree():_root(nullptr),_treeSize(0){}
/**
 * @description: 析构函数，调用后序遍历来释放空间
 * @param {*}
 * @return {*}
 */
BSTree::~BSTree()
{
    postOrder(deleteNode);
}
/**
 * @description: 判断树是否为空
 * @param {*}
 * @return {bool}:返货true代表树为空
 */
bool BSTree::empty()
{
    return _treeSize == 0;
}
/**
 * @description: 获取树中结点的个数
 * @param {*}
 * @return {int}: 返回值为树中结点
 */
int BSTree::size()
{
    return _treeSize;
}
/**
 * @description: 查找函数
 * @param {int &} key：待查找的关键字
 * @return {TreeNode*}: 返回值为待查找关键字所在的结点
 */
TreeNode* BSTree::find(const int & key)
{
    TreeNode* targetNode = _root;
    while (targetNode != nullptr)
    {
        if (targetNode->key == key)
        {
            return targetNode;
        }
        else if (targetNode->key > key)
        {
            targetNode = targetNode->leftChild;//下降到左子结点
        }
        else
        {
            targetNode = targetNode->rightChild;//下降到右子结点
        }
    }
    return targetNode;
}
/**
 * @description: 获取树中最小值所在的结点
 * @param {*}
 * @return {TreeNode*}: 树中最小值所在的结点
 */
TreeNode* BSTree::getMin()
{
    return getTreeMinimum(_root);
}
/**
 * @description: 获取树中最大的值所在的结点
 * @param {*}
 * @return {TreeNode*}: 树中最大值所在的结点
 */
TreeNode* BSTree::getMax()
{
    return getTreeMaximum(_root);
}
/**
 * @description: 插入函数
 * @param {int &} key:待插入的关键字
 * @return {*}
 */
void BSTree::insert(const int & key)
{
    TreeNode* preNode = nullptr, *targetNode = _root;//preNode为targetNode的父结点
    while(targetNode != nullptr)
    {
        preNode = targetNode;
        if (key == targetNode->key)//假定我们的树中不支持重复的键，如果周到相同的直接返回
        {
            return;
        }
        else if(key > targetNode->key)
        {
            targetNode = targetNode->rightChild;//下降到右子结点
        }
        else
        {
            targetNode = targetNode->leftChild;//下降到左子结点
        }
    }
    targetNode = new TreeNode(key);//构造新的结点
    targetNode->parent = preNode;//调整父结点指针
    if(preNode == nullptr)//只有根结点没有父结点
    {
        _root = targetNode;
    }
    else if(preNode->key > key)
    {
        preNode->leftChild = targetNode;
    }
    else
    {
        preNode->rightChild = targetNode;
    }
    _treeSize++;//树中结点的数量+1
}
/**
 * @description: 删除函数
 * @param {int &} key:待删除的关键字，如果树中不存在，则这个函数什么都不做
 * @return {*}
 */
void BSTree::erase(const int & key)
{
    TreeNode* preNode = nullptr, *targetNode = _root;//preNode为targetNode的父结点
    while(targetNode != nullptr && targetNode->key != key)//寻找含有待删除关键字的结点
    {
        preNode = targetNode;
        if (targetNode->key < key)
        {
            targetNode = targetNode->rightChild;
        }
        else
        {
            targetNode = targetNode->leftChild;
        }
    }
    if(targetNode == nullptr)//如果没找到直接返回
    {
        return;
    }
    if(targetNode->rightChild == nullptr)//这种事情况(1)和情况(2)的集合
    {
        if(targetNode->leftChild != nullptr)//右子结点为空，左子结点不为空
        {
            if(preNode == nullptr)//只有根结点的父结点为空
            {
                _root = targetNode->leftChild;//重新设置根结点
            }
            else if(targetNode == preNode->rightChild)
            {
                preNode->rightChild = targetNode->leftChild;//调整指针
            }
            else
            {
                preNode->leftChild = targetNode->leftChild;
            }
            targetNode->leftChild->parent = preNode;//调整父指针
        }
        delete targetNode;//释放该结点的资源。
    }
    else//否则我们应该寻找其后继结点
    {
        TreeNode* preNextNode = targetNode, *nextNode = targetNode->rightChild;
        while (nextNode->leftChild != nullptr)//寻找后继结点
        {
            preNextNode = nextNode;
            nextNode = nextNode->leftChild;
        }
        if (nextNode->rightChild != nullptr)//这种情况，我们需要将其右子结点替换nextNode所在的位置
        {
            if(preNextNode->rightChild == nextNode)
            {
                preNextNode->rightChild = nextNode->rightChild;
            }
            else
            {
                preNextNode->leftChild = nextNode->rightChild;
            }
            nextNode->rightChild->parent = preNextNode;
        }
        else 
        {
            preNextNode->leftChild = nullptr;
        }
        //用nextNode替换targetNode的位置
        if (preNode == nullptr)//只有根结点的父结点为空
        {
            _root = nextNode;
        }
        else if(preNode->rightChild == targetNode)
        {
            preNode->rightChild = nextNode;
        }
        else
        {
            preNode->leftChild = nextNode;
        }
        nextNode->parent = preNode;
        nextNode->leftChild = targetNode->leftChild;
        if(preNextNode != targetNode)
        {
            nextNode->rightChild = targetNode->rightChild;
        }
        delete targetNode;//删除该结点。
    }
    _treeSize--;
}
/**
 * @description: 先序遍历
 * @param {void (*) (TreeNode* node)} f：对结点的操作函数，可以为打印或者其他，由用户决定
 * @return {*}
 */
void BSTree::preOrder(void (*f)(TreeNode* node))
{
    visit = f;
    preOrder(_root);
}
/**
 * @description: 中序遍历
 * @param {void (*) (TreeNode* node)} f：对结点的操作函数，可以为打印或者其他，由用户决定
 * @return {*}
 */
void BSTree::inOrder(void (*f)(TreeNode* node))
{
    visit = f;
    inOrder(_root);
}
/**
 * @description: 后序遍历
* @param {void (*) (TreeNode* node)} f：对结点的操作函数，可以为打印或者其他，由用户决定
 * @return {*}
 */
void BSTree::postOrder(void (*f)(TreeNode* node))
{
    visit = f;
    postOrder(_root);
}
/**
 * @description: 获取以该结点为根的子树中的最小结点
 * @param {TreeNode*} node:树中的一个结点
 * @return {*}
 */
TreeNode* BSTree::getTreeMinimum(TreeNode* node)
{
    while(node->leftChild != nullptr)
    {
        node = node->leftChild;
    }
    return node;
}
/**
 * @description: 获取以该结点为根的子树中的最大结点
 * @param {TreeNode*} node:树中的一个结点
 * @return {*}
 */
TreeNode* BSTree::getTreeMaximum(TreeNode* node)
{
    while(node->rightChild != nullptr)
    {
        node = node->rightChild;
    }
    return node;
}
/**
 * @description: 获取树中一个结点的前驱结点
 * @param {TreeNode*} node:树中的一个结点
 * @return {*}
 */
TreeNode* BSTree::getNodeSuccessor(TreeNode* node)
{
    if(node->rightChild != nullptr)
    {
        return getTreeMinimum(node->rightChild);
    }
    //反向寻找，下面是寻找左子树最大结点的逆过程
    TreeNode* parentNode = node->parent;
    while(parentNode != nullptr && parentNode->rightChild == node)
    {
        node = parentNode;
        parentNode = parentNode->parent;
    } 
    return parentNode;
}
/**
 * @description: 获取树中一个结点的后继结点
 * @param {TreeNode*} node:树中的一个结点
 * @return {*}
 */
TreeNode* BSTree::getNodePredecessor(TreeNode* node)
{
    if(node->leftChild != nullptr)
    {
        return getTreeMaximum(node->leftChild);
    }
    //反向寻找，下面是寻找右子树最小结点的逆过程
    TreeNode* parentNode = node->parent;
    while(parentNode != nullptr && parentNode->leftChild == node)
    {
        node = parentNode;
        parentNode = parentNode->parent;
    } 
    return parentNode;  
}
/**
 * @description: 递归版本前序遍历
 * @param {TreeNode*} node:树中的一个结点
 * @return {*}
 */
void BSTree::preOrder(TreeNode* node)
{
    if(node == nullptr)
    {
        return;
    }
    visit(node);
    preOrder(node->leftChild);
    preOrder(node->rightChild);
}
/**
 * @description: 使用栈的前序遍历
 * @param {void (*) (TreeNode* node)} f：对结点的操作函数，可以为打印或者其他，由用户决定
 * @return {*}
 */
void BSTree::preOrderWithStack(void (*f) (TreeNode* node))
{
    if (_root == nullptr)
    {
        return ;
    }
    visit = f;
    std::stack<TreeNode* > stack1;
    TreeNode* targetNode = _root;//从根结点开始遍历
    do
    {
        while(targetNode != nullptr)
        {
            visit(targetNode);//先访问结点
            stack1.push(targetNode);//再入栈
            targetNode = targetNode->leftChild;//下降到左子结点(前序遍历左子结点)
        }
        targetNode = stack1.top();
        stack1.pop();
        targetNode = targetNode->rightChild;//下降到右子结点(前序遍历右子结点)
    } while (!stack1.empty() || targetNode != nullptr);
}
/**
 * @description: 莫里斯前序遍历
* @param {void (*) (TreeNode* node)} f：对结点的操作函数，可以为打印或者其他，由用户决定
 * @return {*}
 */
void BSTree::morrisPreOrderTraversal(void (*f)(TreeNode* node))
{
    visit = f;
    TreeNode* currentNode = _root;
    while (currentNode != nullptr)
    {
        if (currentNode->leftChild == nullptr)
        {
            visit(currentNode);
            currentNode = currentNode->rightChild;//右子结点
        }
        else//找到右子树的最右结点
        {
            TreeNode* preNode = currentNode->leftChild;
            while (preNode->rightChild != nullptr && preNode->rightChild != currentNode)
            {
                preNode = preNode->rightChild;
            }
            if (preNode->rightChild == nullptr)
            {
                visit(currentNode);//访问该结点
                preNode->rightChild = currentNode;//连接其当前结点
            }
            else if(preNode->rightChild == currentNode)
            {
                preNode->rightChild = nullptr;//置为空还原树结构
                currentNode = currentNode->rightChild;//接着再次遍历右子树
                continue;
            }
            currentNode = currentNode->leftChild;//下降到左子结点。
        }
    }
}
/**
 * @description: 递归版本中序遍历
 * @param {TreeNode*} node：树中一个结点
 * @return {*}
 */
void BSTree::inOrder(TreeNode* node)
{
    if (node == nullptr)
    {
        return;
    }
    inOrder(node->leftChild);
    visit(node);
    inOrder(node->rightChild);
}
/**
 * @description: 使用栈的中序遍历
 * @param {void (*) (TreeNode* node)} f：对结点的操作函数，可以为打印或者其他，由用户决定
 * @return {*}
 */
void BSTree::inOrderWithStack(void (*f)(TreeNode* node))
{
    if (_root == nullptr)
    {
        return ;
    }
    visit = f;
    std::stack<TreeNode* > stack1;
    TreeNode* targetNode = _root;//从根结点开始遍历
    do
    {
        while(targetNode != nullptr)
        {
            stack1.push(targetNode);//入栈
            targetNode = targetNode->leftChild;//下降到左子树
        }
        //当targetNode为空时，就能访问栈中结点了
        targetNode = stack1.top();
        stack1.pop();
        visit(targetNode);
        targetNode = targetNode->rightChild;//下降到右子树(中序遍历右子树)
    } while(!stack1.empty() || targetNode != nullptr);
}
/**
 * @description: 莫里斯中序遍历
 * @param {void (*) (TreeNode* node)} f：对结点的操作函数，可以为打印或者其他，由用户决定
 * @return {*}
 */
void BSTree::morrisInOrderTraversal(void (*f)(TreeNode* node))
{
    visit = f;
    TreeNode* currentNode = _root;
    while (currentNode != nullptr)
    {
        if (currentNode->leftChild == nullptr)
        {
            visit(currentNode);
            currentNode = currentNode->rightChild;//右子结点
        }
        else//找到右子树的最右结点
        {
            TreeNode* preNode = currentNode->leftChild;
            while (preNode->rightChild != nullptr && preNode->rightChild != currentNode)
            {
                preNode = preNode->rightChild;
            }
            if (preNode->rightChild == nullptr)
            {
                preNode->rightChild = currentNode;//连接其当前结点
            }
            else if(preNode->rightChild == currentNode)
            {
                preNode->rightChild = nullptr;//置为空还原树结构
                visit(currentNode);//访问该结点
                currentNode = currentNode->rightChild;//接着再次遍历右子树
                continue;
            }
            currentNode = currentNode->leftChild;//下降到左子结点。
        }
    }
}
/**
 * @description: 递归版本后序遍历
 * @param {TreeNode*} node:树中的一个结点
 * @return {*}
 */
void BSTree::postOrder(TreeNode* node)
{
    if(node == nullptr)
    {
        return;
    }
    postOrder(node->leftChild);
    postOrder(node->rightChild);
    visit(node);
}
/**
 * @description: 使用栈的后序遍历
 * @param {void (*) (TreeNode* node)} f：对结点的操作函数，可以为打印或者其他，由用户决定
 * @return {*}
 */
void BSTree::postOrderWithStack(void (*f)(TreeNode* node))
{
    if (_root == nullptr)
    {
        return ;
    }
    visit = f;
    std::stack<std::pair<TreeNode* , bool> > stack1;
    TreeNode* targetNode = _root;//从根结点开始遍历
    do
    {
        while(targetNode != nullptr)
        {
            stack1.push({targetNode, false});//最开始压栈时，压入false信息
            targetNode = targetNode->leftChild;
        }
        auto temp = stack1.top();
        //下面分情况讨论
        if (!temp.second)//这种情况戏需要访问其右子树
        {
            targetNode = temp.first->rightChild;
            stack1.top().second = true;//修改为true
        }
        else
        {
            while(temp.second)//如果为true，说明是从右子树返回
            {
                visit(temp.first);//访问该结点
                stack1.pop();//弹栈
                if(stack1.empty())
                {
                    break;
                }
                temp = stack1.top();//获取下一个栈顶，
            }   
        }
    } while (!stack1.empty() || targetNode != nullptr);
}
/**
 * @description: 莫里斯后序遍历
 * @param {void (*) (TreeNode* node)} f：对结点的操作函数，可以为打印或者其他，由用户决定
 * @return {*}
 */
void BSTree::morrisPostOrderTraversal(void (*f)(TreeNode* node))
{
    visit = f;
    TreeNode* currentNode = _root;
    while (currentNode != nullptr)
    {
        if (currentNode->leftChild == nullptr)
        {
            currentNode = currentNode->rightChild;//右子结点
        }
        else//找到右子树的最右结点
        {
            TreeNode* preNode = currentNode->leftChild;
            while (preNode->rightChild != nullptr && preNode->rightChild != currentNode)
            {
                preNode = preNode->rightChild;
            }
            if (preNode->rightChild == nullptr)
            {
                preNode->rightChild = currentNode;//连接其当前结点
            }
            else if(preNode->rightChild == currentNode)
            {
                preNode->rightChild = nullptr;//置为空还原树结构
                visitNodeString(currentNode->leftChild);//逆序访问方框中的所有结点
                currentNode = currentNode->rightChild;//接着再次遍历右子树
                continue;
            }
            currentNode = currentNode->leftChild;//下降到左子结点。
        }
    }
    visitNodeString(_root);//最后需要访问根结点
}
/**
 * @description: 层序遍历
 * @param {void (*) (TreeNode* node)} f：对结点的操作函数，可以为打印或者其他，由用户决定
 * @return {*}
 */
void BSTree::levelOrder(void (*f)(TreeNode* node))
{
    if (_root == nullptr)
    {
        return ;
    }
    visit = f;
    std::queue<TreeNode* > queue1;
    TreeNode* targetNode = nullptr;
    queue1.push(_root);//根结点先入队列˚
    while (!queue1.empty())
    {
        targetNode = queue1.front();
        queue1.pop();
        visit(targetNode);
        if(targetNode->leftChild != nullptr)
        {
            queue1.push(targetNode->leftChild);
        }
        if(targetNode->rightChild != nullptr)
        {
            queue1.push(targetNode->rightChild);
        }
    }
}
/**
 * @description: 访问一个结点串，为莫里斯后序遍历的辅助函数
 * @param {TreeNode*} node:树中的一个结点
 * @return {*}
 */
void BSTree::visitNodeString(TreeNode* node)
{
    node = auxReverseNode(node);
    TreeNode* tempNode = node;//保存其副本
    while(node != nullptr)
    {
        visit(node);
        node = node->rightChild;
    }
    auxReverseNode(tempNode);//还原
}
/**
 * @description: 释放结点的空间
 * @param {TreeNode*} node:树中的一个结点
 * @return {*}
 */
void BSTree::deleteNode(TreeNode* node)
{
    delete node;
}
/**
 * @description: 将一串结点中的所有右指针反转，其为visitNodeString的辅助函数
 * @param {TreeNode*} node:树中的一个结点
 * @return {*}
 */
TreeNode* BSTree::auxReverseNode(TreeNode* node)
{
    TreeNode* preNode = nullptr;
    TreeNode* nextNode = nullptr;
    //反转方框内的搜有结点，我们需要借助额外两个指针完成这个工作
    while(node != nullptr)
    {
        nextNode = node->rightChild;//现货区当前结点右子结点
        node->rightChild = preNode;//然后将新的右子结点设为前面的结点
        preNode = node;//前驱结点置为当前结点
        node = nextNode;//跳到下一个结点
    } 
    return preNode;//返回新的处在第一个位置的结点
}