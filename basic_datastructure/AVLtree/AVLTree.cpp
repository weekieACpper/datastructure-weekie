/*
 * @Author: your name
 * @Date: 2022-02-11 23:56:46
 * @LastEditTime: 2022-02-17 17:23:35
 * @LastEditors: weekie
 * @Description: AVL树的实现部分
 * @FilePath: /datastructure/AVLtree/AVLTree.cpp
 */
#include<AVLTree.hpp>
#include<stack>
#include<iostream>
/**
 * @description: AVL树构造函数
 * @param {*}
 * @return {*}
 */
AVLTree::AVLTree():_root(nullptr),_treeSize(0)
{
    _sentinelNode = new TreeNode();
}
/**
 * @description: AVL树析构函数
 * @param {*}
 * @return {*}
 */
AVLTree::~AVLTree()
{
    auxDestructor(_root);
    delete _sentinelNode;
}
/**
 * @description: 用于获取AVL树中结点数量的函数
 * @param {*}
 * @return {int}: 返回树中结点的个数
 */
int AVLTree::size()
{
    return _treeSize;
}
/**
 * @description: 判断AVL树是否为空
 * @param {*}
 * @return {bool}: true表示为空树，
 */
bool AVLTree::empty()
{
    return (_treeSize == 0);
}
/**
 * @description: 查找函数，用于找到树中给定的值
 * @param {int &} value: 待寻找的值
 * @return {TreeNode *}: 如果找到了返回该值所在的结点，否则返回空指针
 */
TreeNode* AVLTree::find(const int & value)
{
	if(_root == nullptr)
    {
      	return nullptr;
    }
    TreeNode* targetNode = _root;
    while(targetNode != nullptr)
    {
        if(targetNode->value == value)//如果找到直接返回
        {
            return targetNode;
        }
        else if(targetNode->value < value)//当前结点的关键字的值比待寻找关键字小，从右侧下降
        {
            targetNode = targetNode->rightChild;
        }
        else//反之从左侧下降
        {
            targetNode = targetNode->leftChild;
        }
    }
    return nullptr;//没有找到，返回空结点
}
/**
 * @description: AVL树的插入函数
 * @param {int &} value: 待插入的值
 * @return {*}
 */
void AVLTree::insert(const int & value)
{
    if(_root == nullptr)
    {
        _sentinelNode->leftChild = _root = new TreeNode(value);
        _treeSize++;
        return ;
    }
    std::stack<TreeNode* > tmpStack;//调用c++标准库的栈
    TreeNode* targetNode = _root;
    //第一个循环找到插入的位置
    while(targetNode != nullptr)
    {
        tmpStack.push(targetNode);//当前所在的结点压栈
        if(targetNode->value == value)//假定不支持重复键
        {
            return;
        }
        else if(targetNode->value > value)
        {
            targetNode = targetNode->leftChild;
        }
        else
        {
            targetNode = targetNode->rightChild;
        }
    }
    targetNode = tmpStack.top();//接下来要确定新结点是插入在栈顶结点的左子结点还是右子结点
    if (targetNode->value > value)
    {
        targetNode->leftChild = new TreeNode(value);
    }
    else
    {
        targetNode->rightChild = new TreeNode(value);
    }
    TreeNode* tempNode = nullptr;
    //第二个循环对路径上的结点进行高度修改和平衡调整
    while(!tmpStack.empty())
    {
        targetNode = tmpStack.top();//取栈顶元素
        tmpStack.pop();//弹栈
        int leftHeight = getNodeHeight(targetNode->leftChild), rightHeight = getNodeHeight(targetNode->rightChild);
        targetNode->height = std::max(leftHeight, rightHeight) + 1; ;//调整高度
        //下面判断是那种非平衡状态,先判断非平衡状态的第一个字母是R还是L
        if(leftHeight > rightHeight + 1)//L~
        {
            tempNode = targetNode->leftChild;
            if(getNodeHeight(tempNode->rightChild) > getNodeHeight(tempNode->leftChild))//判断紫色椭圆的位置
            {
                targetNode->leftChild = leftRotation(tempNode);//LR先左旋转
            }
            //进行右旋转
            if (!tmpStack.empty() && targetNode == tmpStack.top()->rightChild)//以下调整targetNode父结点的指针
            {
                tmpStack.top()->rightChild = rightRotation(targetNode);
            }
            else if (!tmpStack.empty())
            {
                tmpStack.top()->leftChild = rightRotation(targetNode);
            }
            else//栈为空，说明根结点被替换了
            {
                _sentinelNode->leftChild = _root = rightRotation(targetNode);
            }
        }
        else if(rightHeight > leftHeight + 1)//R~ 与上面的镜像
        {
            tempNode = targetNode->rightChild;
            if(getNodeHeight(tempNode->leftChild) > getNodeHeight(tempNode->rightChild))//判断紫色椭圆的位置
            {
                targetNode->rightChild = rightRotation(tempNode);//LR先左旋转
            }
            //进行右旋转
            if (!tmpStack.empty() && targetNode == tmpStack.top()->rightChild)//以下调整targetNode父结点的指针
            {
                tmpStack.top()->rightChild = leftRotation(targetNode);
            }
            else if (!tmpStack.empty())
            {
                tmpStack.top()->leftChild = leftRotation(targetNode);
            }
            else
            {
                _sentinelNode->leftChild = _root = leftRotation(targetNode);
            }
        }
    }
    _treeSize++;
}
/**
 * @description: AVL树的删除函数
 * @param {int &} value: 待删除的值
 * @return {*}
 */
void AVLTree::erase(const int & value)
{
    _sentinelNode->leftChild = _root = auxErase(_root, value, (TreeNode* )nullptr);
}
/**
 * @description: AVL树的打印函数
 * @param {*}
 * @return {*}
 */
void AVLTree::print()
{
    auxPrint(_root);
}
/**
 * @description: 获取AVL树中一个结点的高度
 * @param {TreeNode*} node: 树中的一个结点
 * @return {int}: 树的高度
 */
int AVLTree::getNodeHeight(TreeNode* node)
{
    if(node != nullptr)
    {
        return node->height;
    }
    return 0;//空结点的高度为0；
}
/**
 * @description: 左旋转函数
 * @param {TreeNode*} node: 树中的一个结点
 * @return {TreeNode *}: 返回值为原来node所在位置上的结点
 */
TreeNode* AVLTree::leftRotation(TreeNode* node)
{
    TreeNode* rightNode = node->rightChild;
    node->rightChild = rightNode->leftChild;//原结点的右子结点变更为其原来右子结点的左子结点
    rightNode->leftChild = node;//原来右子结点的左子结点变为node
    //重新设置两个旋转结点的高度，为其左右子树中高度较大的那个高度加1
    node->height = std::max(getNodeHeight(node->rightChild), getNodeHeight(node->leftChild)) + 1;
    rightNode->height = std::max(getNodeHeight(node), getNodeHeight(rightNode->rightChild)) + 1;
    return rightNode;
}
/**
 * @description: 右旋转函数
 * @param {TreeNode*} node: 树中的一个结点
 * @return {TreeNode *}: 返回值为原来node所在位置上的结点
 */
TreeNode* AVLTree::rightRotation(TreeNode* node)
{
    TreeNode* leftNode = node->leftChild;
    node->leftChild = leftNode->rightChild;
    /**
     * @description: 
     * @param {*}
     * @return {*}
     */
    leftNode->rightChild = node;
    node->height = std::max(getNodeHeight(node->rightChild), getNodeHeight(node->leftChild)) + 1;
    leftNode->height = std::max(getNodeHeight(node), getNodeHeight(leftNode->leftChild)) + 1;
    return leftNode;
}
/**
 * @description: 打印函数的辅助函数，采用中序遍历打印结点
 * @param {TreeNode*} node: 树中的一个结点
 * @return {*}
 */
void AVLTree::auxPrint(TreeNode* node)
{
    if(node == nullptr)
    {
        return;
    }
    auxPrint(node->leftChild);
    std::cout << node->value << " ";
    auxPrint(node->rightChild);
}
/**
 * @description: 删除函数的我辅助函数
 * @param {TreeNode*} node: 树中的一个结点
 * @param {int &} value: 待删除的值
 * @param {TreeNode*} storedNode: 含有待删除关键字的结点
 * @return {TreeNode*}: 返回值为原来node所在位置上的结点
 */
TreeNode* AVLTree::auxErase(TreeNode* node, const int & value, TreeNode* storedNode)
{
    if(node == nullptr)
    {
        return nullptr;
    }
    if (node->value == value)//找到含有待删除值的结点后，寻找其后继
    {
        if(node->rightChild == nullptr)//如果右子结点为空，删除这个结点，并用左子结点代替
        {
            --_treeSize;
            TreeNode* tempNode = node->leftChild;
            delete node;
            return tempNode;
        }
        //否则寻找其后继
        storedNode = node;//存储含有待删除关键字的结点
        node->rightChild = auxErase(node->rightChild, value, storedNode);
    }
    else if(node->value > value)
    {
        if(storedNode != nullptr && node->leftChild == nullptr)//说明已经找到了后继结点
        {
            storedNode->value = node->value;//用后继结点的值替代该结点
            //然后删除后继结点
            if(node->rightChild != nullptr)//如果后继结点有右子结点
            {
                TreeNode* tempNode = node->rightChild;
                delete node;
                return tempNode;//返回右子结点
            }
            delete node;
            return nullptr;
        }
        node->leftChild = auxErase(node->leftChild, value, storedNode);
    }
    else
    {
        node->rightChild = auxErase(node->rightChild, value, storedNode);
    }
    int leftHeight = getNodeHeight(node->leftChild), rightHeight = getNodeHeight(node->rightChild);
    //递归回来后要进行高度设置和平衡调整
    node->height = std::max(leftHeight, rightHeight) + 1;
    if(leftHeight > rightHeight + 1)
    {
        TreeNode* tempNode = node->leftChild;
        if(getNodeHeight(tempNode->leftChild) < getNodeHeight(tempNode->rightChild))
        {
            //执行左旋转
            node->leftChild = leftRotation(tempNode);
        }
        //执行右旋转
        node = rightRotation(node);
    }
    else if(rightHeight > leftHeight + 1)
    {
        TreeNode* tempNode = node->rightChild;
        if(getNodeHeight(tempNode->leftChild) > getNodeHeight(tempNode->rightChild))
        {
            node->rightChild = rightRotation(tempNode);
        }
        node = leftRotation(node);
    }
    return node;
}
/**
 * @description: 析构函数的辅助函数，采用后序遍历释放每个结点占用的内存空间
 * @param {TreeNode*} node: 树中的一个结点
 * @return {*}
 */
void AVLTree::auxDestructor(TreeNode* node)
{
    if(node == nullptr)
    {
        return;
    }
    auxDestructor(node->leftChild);
    auxDestructor(node->rightChild);
    delete node;
}