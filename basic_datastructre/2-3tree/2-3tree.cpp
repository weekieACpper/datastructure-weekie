/**
 * @Author: weekie
 * @Date: 2021-12-16 23:37:07
 * @LastEditTime: 2021-12-22 20:10:08
 * @LastEditors: weekie
 * @Description: 2-3树的实现部分，主要实现了插入、删除、查找
 * @FilePath: /datastructure/2-3tree/2_3tree.cpp
 */
#include"2-3tree.hpp"
#include<iostream>
/**
 * @description: 2-3树的无参构造函数
 * @param {*}
 * @return {*}
 */
twoThreeTree::twoThreeTree():_treeSize(0)
{
    _root = new TreeNode();
    _root->isLeaf = true;//最开始根结点就是叶结点。
}
/**
 * @description: 2-3树的析构函数，通过调用辅助函数aux_destructor来回收结点
 * @param {*}
 * @return {*}
 */
twoThreeTree::~twoThreeTree()//析构函数
{
    auxDestructor(_root);
}
/**
 * @description: 此函数用来判断2-3树是否为空，为空返回true
 * @param {*}
 * @return {bool}返回值用于判断树是否为空
 */
bool twoThreeTree::empty()
{
    return !(_treeSize==0);
}
/**
 * @description: 此函数用于获取2-3树中关键字的数量
 * @param {*}
 * @return {int}
 */
int twoThreeTree::size()
{
    return _treeSize;
}
/**
 * @description: 该函数用于寻找关键字为value的节点
 * @param {int &} value:待寻找的关键字。
 * @return {std::pair<TreeNode*,int>} 返回值为含有需要寻找的关键字结点及其索引
 */
std::pair<TreeNode*, int> twoThreeTree::find(const int & value)
{
    if(_root == nullptr)
    {
        return {nullptr,-1};
    }
    TreeNode* targetNode = _root;
    int i = 0;
    while(targetNode != nullptr)
    {
        for(i = 0; i < targetNode-> nodeSize && value > targetNode->valueList[i]; i++);
        if(i != targetNode->nodeSize && targetNode->valueList[i] == value)
            return {targetNode,i};
        else
            targetNode = targetNode->pointList[i];
    }
    return {targetNode,-1};
}
/**
 * @description: 插入一个关键字为value的元素
 * @param {int &} value:关键字
 * @return {*}
 */
void twoThreeTree::insert(const int & value)
{
    _root = auxInsert(_root, value);
    if(_root->nodeSize == 3)//检查根结点是否含有3个关键字
    {
        TreeNode* newRoot = new TreeNode();//构造新的根结点
        newRoot->pointList[0] = _root;//指向原来的根结点
        splitNode(newRoot, 0);//分裂原来根结点
        _root = newRoot;
    }
    ++_treeSize;
}
/**
 * @description: 在树中删除关键字唯value的元素
 * @param {int &} value:待删除的关键字元素
 * @return {*}
 */
void twoThreeTree::erase(const int & value)
{
    _root = auxErase(_root, value);
    if(_root->nodeSize == 0)
    {
        _root = _root->pointList[0];
    }
    --_treeSize;
}
/**
 * @description: 打印整棵2-3树
 * @param {*}
 * @return {*}
 */
void twoThreeTree::print()
{
    auxPrint(_root);
}
/**
 * @description: 析构函数的辅助函数，采用递归式后序遍历，释放搜有结点
 * @param {TreeNode*} Node:遍历的起始结点
 * @return {*}
 */
void twoThreeTree::auxDestructor(TreeNode* Node)
{
    if(Node->isLeaf){//如果为叶子结点删除并且返回
        delete Node;
        return;
    }
    int tmpNodeSize = Node->nodeSize;
    for(int i = 0; i <= tmpNodeSize; i++){
        auxDestructor(Node->pointList[i]);
    }
    delete Node;   
}
/**
 * @description: 辅助打印函数，采用递归式中序遍历，打印所有结点
 * @param {TreeNode*} Node:遍历的q起始结点
 * @return {*}
 */
void twoThreeTree::auxPrint(TreeNode* Node)
{
    if (Node->isLeaf)
    {
        for (size_t i = 0; i < Node->nodeSize; i++)
        {
            std::cout << Node->valueList[i] << " ";
        }
        return;
    }
    int tmpNodeSize = Node->nodeSize;
    for(int i = 0; i < tmpNodeSize; i++)
    {
        auxPrint(Node->pointList[i]);
        std::cout << Node->valueList[i] << " ";
    }
    auxPrint(Node->pointList[tmpNodeSize]);
}
/**
 * @description: 插入函数的辅助函数
 * @param {TreeNode*} node:待下降到的结点
 * @param {int &} value:待插入的关键字的值
 * @return {TreeNode*} 返回一个结点
 */
TreeNode* twoThreeTree::auxInsert(TreeNode* node, const int & value)
{
    if(node->isLeaf)//如果找到了待插入的叶结点，插入关键字
    {
        int i, temp = value;//临时变量
        for(i = 0; i < node->nodeSize && value > node->valueList[i]; i++);//先找到对应的插入位置的索引
        for(;i <= node->nodeSize; ++i)//正式插入
        {
            std::swap(temp, node->valueList[i]);
        }
        ++node->nodeSize;//关键字个数加1;
        return node;
    }
    int i;
    for(i = 0; i < node->nodeSize && value > node->valueList[i]; ++i);//找到下降孩子结点对应的索引值
    node->pointList[i] = auxInsert(node->pointList[i], value);//递归下降到对应的孩子结点
    if(node->pointList[i]->nodeSize == 3)//回溯后如果孩子结点有三个关键字
    {
        //分裂该孩子结点
        splitNode(node, i);
    }
    return node;
}
/**
 * @description: erase的辅助函数
 * @param {TreeNode*} targetNode：待删除的节点
 * @return {*}
 */
TreeNode* twoThreeTree::auxErase(TreeNode* node, const int & value)
{
    static TreeNode* storedNode = nullptr;//保存待删除关键字的静态局部变量
    static int storedIndex = -1;//保存待删除关键字的索引    
    if (node->isLeaf)//说明找到后继结点，或者待删除关键字就在叶子结点或者树中根本就没有这个关键字
    {
        int j = 0;
        if(storedNode == nullptr)
        {
            for(; j < node->nodeSize && node->valueList[j] < value; ++j);
            if(node->valueList[j] != value)//说明没找到，树中不存在这个关键字
            {
                return nullptr;//返回空指针
            }//不然的话就是存在        
            j += 1;
        }
        else
        {
            storedNode->valueList[storedIndex] = node->valueList[j];//覆盖
        }
        for(;j <= node->nodeSize; ++j)//将该结点后面的关键字往前移，覆盖该结点
        {
            node->valueList[j - 1] = node->valueList[j];
        }
        --node->nodeSize;
        return node;
    } 
    int i;
    for(i = 0; i < node->nodeSize && value >= node->valueList[i]; ++i);//找到下降孩子结点或待删除关键字对应的索引值
    if(i != node->nodeSize && node->valueList[i] == value)//找到待删除关键字结点
    {
        storedNode = node;
        storedIndex = i;
        i += 1;//这是为了保证待会寻找后继从右边下降
    }
    auxErase(node->pointList[i], value);//下降
    //以下根据情况做出调整
    if(node->pointList[i]->nodeSize == 0)//如果子结点关键字个数为0,则需要调整
    {
        TreeNode* childNode = node->pointList[i];
        if(i - 1 >=0 && node->pointList[i - 1]->nodeSize == 2)//首先检查兄弟结点是否可以借,这里先向左兄弟结点借
        {
            TreeNode* brotherNode = node->pointList[i - 1];//获取兄弟结点
            childNode->valueList[0] = node->valueList[i - 1];//父结点中对应关键字下降
            node->valueList[i - 1] = brotherNode->valueList[brotherNode->nodeSize - 1];//兄弟结点最后一个关键字上升
            childNode->pointList[1] = childNode->pointList[0];//第一个指针往后移动，挪出位置
            childNode->pointList[0] = brotherNode->pointList[brotherNode->nodeSize];//将兄弟结点最后一个指针移动至这里
            --brotherNode->nodeSize;//兄弟结点关键字减少1
            ++childNode->nodeSize;
        }
        else if(i + 1 <= node->nodeSize && node->pointList[i + 1]->nodeSize == 2)//向可能存在的右兄弟结结点借.
        {
            TreeNode* brotherNode = node->pointList[i + 1];//后去兄弟结点
            childNode->valueList[0] = node->valueList[i];//父亲结点对应关键字下降
            node->valueList[i] = brotherNode->valueList[0];//兄弟结点第一个关键字提升
            childNode->pointList[1] = brotherNode->pointList[0];//将兄弟结第一个指针移动至这里
            brotherNode->pointList[0] = brotherNode->pointList[1];//由于失去了第一个孩子指针和关键字，后面的得往前移动
            brotherNode->pointList[1] = brotherNode->pointList[2];
            brotherNode->valueList[0] = brotherNode->valueList[1];
            --brotherNode->nodeSize;
            ++childNode->nodeSize; //从兄弟结点处借了一个，因此+1；
        }
        else if(node->nodeSize == 2)//如果父结点关键字为3-
        {
            int brotherIndex = (i == 0) ? 1 : i - 1;
            int index = (i > brotherIndex) ? (i-1) : i;//防止越界访问,下面是不能直接用i的
            int tempValue = node->valueList[index];
            TreeNode* tempNode = childNode->pointList[0];
            TreeNode* brotherNode = node->pointList[brotherIndex];
            if(brotherIndex > index)//兄弟结点在其右边
            {
                for(index += 1; index < node->nodeSize; ++index)//父结点少了一个关键字和一个指向孩子结点的指针，调整
                {
                    node->valueList[index - 1] = node->valueList[index];//往前移动；
                    node->pointList[index - 1] = node->pointList[index];
                }
                node->pointList[index - 1] = node->pointList[index];
                int j = 0;
                for(;j <= brotherNode->nodeSize; ++j)
                {
                    std::swap(tempValue, brotherNode->valueList[j]);
                    std::swap(tempNode, brotherNode->pointList[j]);
                }

                brotherNode->pointList[j] = tempNode;
            }
            else//兄弟结点在左边
            {
                for(index += 1; index < node->nodeSize; ++index)//父结点少了一个关键字和一个指向孩子结点的指针，调整
                {
                    node->valueList[index - 1] = node->valueList[index];//往前移动；
                    node->pointList[index] = node->pointList[index + 1];
                }
                brotherNode->valueList[1] = tempValue;
                brotherNode->pointList[2] = tempNode;
            }
            delete childNode;
            --node->nodeSize;
            ++brotherNode->nodeSize;
        }
        else
        { 
            mergeNode(node, i);
        }
    }
    return node;
}
/**
 * @description: 分解一个树结点，产生两个新结点，中间关键字将会移至父结点，剩下的关键字对半分。
 * @param {TreeNode*} parentNode:node的父结点
 * @param {int} index:待分裂结点在父结点中的索引
 * @return {*}
 */
void twoThreeTree::splitNode(TreeNode* parentNode, int index)
{
    TreeNode* leftNode = parentNode->pointList[index];//获取待分裂的结点，同时它也是分裂后的左子结点
    int tempValue = leftNode->valueList[1];//获取中间关键字
    TreeNode* rightNode = new TreeNode();//构造新结点
    rightNode->isLeaf = leftNode->isLeaf;//分裂后的结点是否为叶子属性应该是相等的
    rightNode->valueList[0] = leftNode->valueList[2];//将原结点最后一个关键字给分裂后的结点
    rightNode->pointList[0] = leftNode->pointList[2];//同时将原结点最后两个指向孩子结点的指针赋予新结点对应位置
    rightNode->pointList[1] = leftNode->pointList[3];
    rightNode->nodeSize = 1;
    leftNode->nodeSize = 1;
    //下面将中间关键字插入父亲结点,同时调整指针
    TreeNode* tempNode = rightNode;
    for(int i = index; i <= parentNode->nodeSize; i++)
    {
        std::swap(tempValue, parentNode->valueList[i]);
        std::swap(tempNode, parentNode->pointList[i + 1]);
    }
    ++parentNode->nodeSize;//父结点关键字个数+1
}
/**
 * @description: 合并两个结点
 * @param {TreeNode*} parentNode:待合并结点的父结点
 * @param {int} index:待删除结点在父结点中的索引。
 * @return {*}
 */
void twoThreeTree::mergeNode(TreeNode* parentNode, int index)
{
    int brotherIndex = (index == 0) ? 1 : index - 1;
    TreeNode* brotherNode = parentNode->pointList[brotherIndex];
    //将父结点中
    int i, tempValue = parentNode->valueList[0];//获取父结点为1关键字
    TreeNode* tempNode = parentNode->pointList[index]->pointList[0];//获取待删除结点的孩子结点
    if(tempValue < brotherNode->valueList[0])//如果父亲结点的唯一关键字插入到兄弟结点的第一个位置
    {
        for(i = 0;i <= 1; ++i)//插入关键字，并且调整指针
        {
            std::swap(tempValue, brotherNode->valueList[i]);
            std::swap(tempNode, brotherNode->pointList[i]);
        }
        brotherNode->pointList[i] = tempNode;
    }
    else//父亲结点的唯一关键字插入到兄弟结点唯一关键字后
    {
        brotherNode->valueList[1] = tempValue;
        brotherNode->pointList[2] = tempNode;
    }
    ++brotherNode->nodeSize;//兄弟结点关键字数量+1;
    --parentNode->nodeSize;//父结点关键字个数减1(为0)
    delete parentNode->pointList[index];//释放待删除结点
    parentNode->pointList[0] = brotherNode;//调整父结点第一个指针为兄弟结点
}






 





