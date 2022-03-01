/*
 * @Author: weekie
 * @Date: 2021-12-26 17:06:52
 * @LastEditTime: 2022-02-28 15:33:47
 * @LastEditors: Please set LastEditors
 * @Description: 红黑树操作的实现部分
 * @FilePath: /datastructure/RBtree/RBTree.cpp
 */
#include<RBTree.hpp>
#include<iostream>
/**
 * @description: 红黑树的构造函数
 * @param {*}
 * @return {*}
 */
RBTree::RBTree():_root(nullptr),_treeSize(0)
{
    _sentinelNode = new TreeNode();//初始化哨兵结点.
    _sentinelNode->color = false;//颜色置为黑色
}
/**
 * @description: 红黑树的析构函数
 * @param {*}
 * @return {*}
 */
RBTree::~RBTree()
{
    auxDestructor(_root);
    delete _sentinelNode;
}
/**
 * @description: 获得红黑树中的关键字个数
 * @param {*}
 * @return {int} 红黑树中的关键字个数
 */
int RBTree::size()
{
    return _treeSize;
}
/**
 * @description: 判断红黑树是否为空
 * @param {*}
 * @return {bool} true为空
 */
bool RBTree::empty()
{
    return !(_treeSize == 0);
}
/**
 * @description: 检验这棵树是否为红黑树
 * @param {*}
 * @return {bool} 如果为红黑树，返回true，否则返回false
 */
bool RBTree::isRBTree()
{
    if(isBST(_root) && !hasConsequentRedNode(_root) && countBlackNode(_root))
    {
        return true;
    }
    return false;
}
/**
 * @description: 查找关键字为value的函数
 * @param {int &} value：待查找的关键字
 * @return {TreeNode*} 如果没找到，为nullptr，否则为还有该关键字的结点
 */
TreeNode* RBTree::find(const int & value)
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
 * @description: 在树中插入关键字为value的元素
 * @param {int &} value:待插入的关键字
 * @return {*}
 */
void RBTree::insert(const int & value)
{
    TreeNode* targetNode = _root, *preNode = _sentinelNode;//preNode为targetNode的父结点,初始化为哨兵结点
    TreeNode* grandParentNode = nullptr, *preGrandParentNode = nullptr;//当遇到要下降的点为红子结点是，记录当前的结点以及其父结点
    bool flag = true;//记录targetNode是preNode的左子结点还是右子结点,左为true，右为false
    //额外两个标记，mark1用于记录grandParentNode是在preGrandParentNode的左边还是右边
    //mark2用于记录从grandParentNode下降的子结点在grandParent的左边还是右边,左为true，右为false
    bool mark1 = true,mark2 = true;
    while(targetNode != nullptr)
    {
        if(targetNode->value == value)//如果在树中找到了待插入的关键字，返回本例中不考虑重复关键字
        {
            return;
        }
        //由于必然要从targetNode左边或者右边下降，先考虑其是否为4-结点,如果是进行颜色转换.
        if(isRed(targetNode->leftChild) && isRed(targetNode->rightChild))
        {
            flipColor(targetNode);//颜色变换
            //检查父结点颜色是否为红色，如果为红色，则要进行调整
            //此时grandParentNode必然是存在的
            if(isRed(preNode))
            {
                TreeNode* tempNode = nullptr;
                if(mark2 != flag)//不是同为左或者同为右，对preNode执行相应旋转
                {
                    //如果flag为true，说明其targetNode为左子结点，由于mark2和其不同，所以preNode必然是grandParentNode的右子结点
                    if(flag)
                    {
                        grandParentNode->rightChild = rightRotation(preNode);
                    }
                    else
                    {
                        grandParentNode->leftChild = leftRotation(preNode);
                    }
                }
                if(mark2)//执行单旋转
                {
                    tempNode = rightRotation(grandParentNode);
                }
                else
                {
                    tempNode = leftRotation(grandParentNode);
                }
                if(mark1)//调整指针
                {
                    preGrandParentNode->leftChild = tempNode;
                }
                else
                {
                    preGrandParentNode->rightChild = tempNode;
                }
            }
        }
        if(targetNode->value > value)//下降点为左子结点
        {
            if(isRed(targetNode->leftChild))//判断左子结点是否红色结点
            {
                preGrandParentNode = preNode;
                grandParentNode = targetNode;
                mark1 = flag;//更新标记
                mark2 = true;
            }
            flag = true;
            preNode = targetNode;
            targetNode = targetNode->leftChild;
        }
        else //下降点为右子结点
        {
            if(isRed(targetNode->rightChild))//判断右子结点是否红色结点
            {
                preGrandParentNode = preNode;
                grandParentNode = targetNode;
                mark1 = flag;//更新标记
                mark2 = false;
            }
            flag = false;
            preNode = targetNode;
            targetNode = targetNode->rightChild;
        }
    }
    //下面插入新结点,由已知得，我们只可能插入到2-结点或者3-结点，插入3-结点可能需要额外处理
    targetNode = new TreeNode(value);
    if(_root == nullptr)
    {
        _root = targetNode;//根结点为空设置根结点
        _root->color = false;
    }
    if(flag)
    {
        preNode->leftChild = targetNode;
    }
    else
    {
        preNode->rightChild = targetNode;
    }
    //插入3-结点时检查父结点是否为红色，如果是，旋转进行调整.这和上面是一样的
    if(isRed(preNode))
    {
        TreeNode* tempNode = nullptr;
        if(mark2 != flag)//不是同为左或者同为右，对preNode执行相应旋转
        {
            if(flag)
            {
                grandParentNode->rightChild = rightRotation(preNode);
            }
            else
            {
                grandParentNode->leftChild = leftRotation(preNode);
            }
        }
        if(mark2)
        {
            tempNode = rightRotation(grandParentNode);
        }
        else
        {
            tempNode = leftRotation(grandParentNode);
        }
        if(mark1)
        {
            preGrandParentNode->leftChild = tempNode;
        }
        else
        {
            preGrandParentNode->rightChild = tempNode;
        }
    }
    //每次插入完后得更新根结点，因为根结点可能会因为旋转而改变，但哨兵结点的左结点永远指向正确的根结点
    _root = _sentinelNode->leftChild;
    ++_treeSize;
}
/**
 * @description: 插入函数的递归版本
 * @param {int &} value:待插入的关键字
 * @return {*}
 */
void RBTree::recursiveInsert(const int & value)
{
    _sentinelNode->leftChild = _root = auxRecursiveInsert(_root, value);
    _root->color = false;
}
/**
 * @description: 从树中删除关键字为value的结点
 * @param {int &} value:待删除的关键字
 * @return {*}
 */
void RBTree::erase(const int & value)
{
    TreeNode* targetNode = _root, *preNode = _sentinelNode, *storedNode = nullptr;//分别初始化为哨兵结点和根结点
    bool flag = true;//标记targetNode为preNode的左子结点还是右子结点
    //首先找到待删除的关键字所在的结点
    while(targetNode != nullptr)
    {
        if(targetNode->value == value)
        {
            if(targetNode->rightChild == nullptr)//判断右子结点是否为空，为空就无法寻找后继结点
            {
                //由于下降到这里该结点必然不是2-结点（除了根结点）。如果其自身为黑色，那么其左子结点必然为红色。
                if(!isRed(targetNode) && targetNode != _root) 
                {
                    std::swap(targetNode->value, targetNode->leftChild->value);//交换两者的值
                    delete targetNode->leftChild;//删除右子结点
                    targetNode->leftChild = nullptr;
                }
                else//否则该结点为红色结点，直接删除
                {
                    delete targetNode;
                    if(flag)
                    {
                        preNode->leftChild = nullptr;
                    }
                    else
                    {
                        preNode->rightChild = nullptr;
                    }
                }
                break;//直接跳出循环
            }
            //否则保存当前结点，下降到右子结点寻找后继结点
            storedNode = targetNode;
        }        
        if(targetNode->value > value)//下降到左子结点
        {
            if(is2_Node(targetNode->leftChild))//如果下降到左子结点为2-结点，则向兄弟结点借
            {
                TreeNode* tempNode = targetNode;//临时变量
                if(isRed(targetNode->rightChild))//先检查兄弟结点是否是红色，如果是红色，要先左旋转找到真正的兄弟结点
                {
                    tempNode = leftRotation(targetNode);
                }
                if(is2_Node(targetNode->rightChild))//如果兄弟结点也为2-结点,颜色转换
                {
                    flipColor2(targetNode);
                }
                else//否则因该从兄弟结点借
                {   //首先得判断兄弟结点的左子结点是否为红色来判断是否执行双旋转,
                    if(isRed(targetNode->rightChild->leftChild))//如果为红色，先执行右旋
                    {
                        targetNode->rightChild = rightRotation(targetNode->rightChild);
                    }
                    //执行左旋,同时调整颜色
                    targetNode->leftChild->color = true;
                    if(tempNode!=targetNode)//说明最初兄弟结点为红色(已经发生了一次旋转)，这时候得调整指针
                    {
                        targetNode = leftRotation(targetNode);
                        tempNode->leftChild = targetNode;
                    }
                    else
                    {
                        tempNode = targetNode = leftRotation(targetNode);
                    }
                    targetNode->rightChild->color = false;
                }
                if(flag)//调整父结点孩子指针的指向
                {
                    preNode->leftChild = tempNode;
                }
                else
                {
                    preNode->rightChild = tempNode;
                }
            }//不是2-结点,要先判断是否找到了后继结点，否则直接下降
            if(storedNode != nullptr && targetNode->leftChild == nullptr)//如果下个下降的结点为空，并且storedNode不为空，说明找到了后继
            {
                storedNode->value = targetNode->value;//覆盖
                if(!isRed(targetNode))//如果后继结点为黑色，那么其右子结点必然为红色，因为下降到这里时其不可能为2-结点
                 {
                    std::swap(targetNode->value, targetNode->rightChild->value);//交换两者的值
                    delete targetNode->rightChild;//删除右子结点
                    targetNode->rightChild = nullptr;
                }
                else//否则该结点为红色结点，直接删除
                {
                    delete targetNode;
                    if(flag)
                    {
                        preNode->leftChild = nullptr;
                    }
                    else
                    {
                        preNode->rightChild = nullptr;
                    }
                }
                break;//直接跳出循环
            }
            flag = true;
            preNode = targetNode;
            targetNode = targetNode->leftChild;
        }
        else//和上面是镜像的情况，下降到右结点，不在赘述
        {
            if(is2_Node(targetNode->rightChild))
            {
                TreeNode* tempNode = targetNode;
                if(isRed(targetNode->leftChild))
                {
                    tempNode = rightRotation(targetNode);
                }
                if(is2_Node(targetNode->leftChild))
                {
                    flipColor2(targetNode);
                }
                else
                {   
                    if(isRed(targetNode->leftChild->rightChild))
                    {
                        targetNode->leftChild = leftRotation(targetNode->leftChild);
                    }
                    targetNode->rightChild->color = true;
                    if(tempNode!=targetNode)
                    {
                        targetNode = rightRotation(targetNode);                    
                        tempNode->rightChild = targetNode;
                    }
                    else
                    {
                        tempNode = targetNode = rightRotation(targetNode);
                    }
                    targetNode->leftChild->color = false;
                }
                if(flag)
                {
                    preNode->leftChild = tempNode;
                }
                else
                {
                    preNode->rightChild = tempNode;
                }
            }
            flag = false;
            preNode = targetNode;
            targetNode = targetNode->rightChild;
        }
    }
    if(targetNode == nullptr)//如果没找到直接返回
    {
        return;
    }
    _root = _sentinelNode->leftChild;//根结点可能会因为旋转变换位置，需要修正。
    --_treeSize;
}
/**
 * @description: 删除函数的递归版本
 * @param {int &} value：待删除的关键字
 * @return {*}
 */
void RBTree::recursiveErase(const int & value)
{
    _sentinelNode->leftChild = _root = auxRecursiveErase(_root, value);
}
/**
 * @description: 红黑树的打印函数，答应所有结点的值
 * @param {*}
 * @return {*}
 */
void RBTree::print()
{
    auxPrint(_root);
}
/**
 * @description: 判断这个结点是否为红色
 * @param {TreeNode*} node:树中的一个结点
 * @return {bool} 红色结点返回true，黑色返回false，空结点返回false
 */
bool RBTree::isRed(TreeNode* node)
{
    if(node == nullptr)
    {
        return false;
    }
    return node->color;
}
/**
 * @description: 判断一个结点是否是2-结点
 * @param {TreeNode*} node:树中的一个结点
 * @return {bool} 是2-结点返回true，否则返回false，空结点返回false
 */
bool RBTree::is2_Node(TreeNode* node)
{
    if(node == nullptr)
    {
        return false;
    }
    //自身为黑，并且左右子结点都不为红，才是2-结点
    if(!isRed(node) && !isRed(node->rightChild) && !isRed(node->leftChild))
    {
        return true;
    }
    return false;
}
/**
 * @description: 判断这棵树是否是有序的用于检验是否是二叉搜索树
 * @param {TreeNode*} node: 树中的一个结点
 * @return {bool} 如果有序返回true，否则返回false
 */
bool RBTree::isOrdered(TreeNode* node)
{
    if(node == nullptr)
    {
        return true;
    }
    bool flag1 = false, flag2 = false;
    flag1 = isOrdered(node->leftChild);
    flag2 = isOrdered(node->rightChild);
    if(!flag1 || !flag2)//如果子树中有一个不满足，返回false
    {
        return false;
    }
    else
    {
        if(node->leftChild != nullptr && node->leftChild->value > node->value)//如果左子结点大雨当前结点返回false
        {
            return false;
        }
        if(node->rightChild != nullptr && node->rightChild->value < node->value)//同上，不解释
        {
            return false;
        }
        return true;
    }
}
/**
 * @description: 检验时树中是否出现了连续的红结点
 * @param {TreeNode*} node:树中的一个结点
 * @return {bool} 如果出现了连续的红结点返回true，否则返回false
 */
bool RBTree::hasConsequentRedNode(TreeNode* node)
{
    if(node == nullptr)
    {
        return false;
    }
    bool flag1 = false, flag2 = false;
    flag1 = hasConsequentRedNode(node->leftChild);
    flag2 = hasConsequentRedNode(node->rightChild);
    if(flag1 || flag2)//只要有一颗子树有连续红色结点，返回true
    {
        return true;
    }
    else
    {
        if(!isRed(node) || !isRed(node->rightChild) || !isRed(node->leftChild))
        {
            return false;
        }
        return true;
    }
}
/**
 * @description: 检验是否为二叉搜索树
 * @param {TreeNode*} node:树中的一个结点
 * @return {bool}如果是返回true，否则返回false
 */
bool RBTree::isBST(TreeNode* node)
{
    if(isOrdered(_root) && _treeSize == countNode(_root))//两者同时成立财位二叉搜索树
    {
        return true;
    }
    return false;
}
/**
 * @description: 统计树中的结点数量
 * @param {TreeNode*} node:树中的一个结点
 * @return {int} 返回树中的结点数量
 */
int RBTree::countNode(TreeNode* node)
{
    int size1 = 0, size2 = 0;
    if(node == nullptr)
    {
        return 0;
    }
    size1 = countNode(node->leftChild);//获取左子树的结点数
    size2 = countNode(node->rightChild);//获取右子树的结点数
    return size1 + size2 + 1;
}
/**
 * @description: 检验树中每条路径上的黑色结点数量是否相同
 * @param {TreeNode*} node:树中的一个结点
 * @return {int}：如果相同，返回值为黑高，否则为0
 */
int RBTree::countBlackNode(TreeNode* node)
{
    int size1 = 0, size2 = 0;
    if(node == nullptr)
    {
        return 1;
    }
    size1 = countBlackNode(node->leftChild);
    size2 = countBlackNode(node->rightChild);
    if (size1 != size2)//如果两个路径上的黑色结点不相等，返回0
    {
        return 0;
    }
    else
    {
        if(!isRed(node))//如果当前结点为黑色结点，返回size+1
        {
            return size1+1;
        }
        else//反之返回size
        {
            return size1;
        }
    }
}
/**
 * @description: 插入用的颜色转换函数，等效于2-3-4树中的分裂
 * @param {TreeNode*} node:树中的一个结点
 * @return {}: 
 */
void RBTree::flipColor(TreeNode* node)
{
    if(node == nullptr)
    {
        return;
    }
    node->leftChild->color = node->rightChild->color = false;
    if(node != _root)//根结点始终为黑色
    {
        node->color = true;
    }
}
/**
 * @description: 删除用的颜色转换函数
 * @param {TreeNode*} node:树中的一个结点
 * @return {*}
 */
void RBTree::flipColor2(TreeNode* node)
{
    node->leftChild->color = node->rightChild->color = true;
    node->color = false;
}
/**
 * @description: 左旋转函数
 * @param {TreeNode*} node:树中的一个结点
 * @return {TreeNode*} 返回原先node所在位置的新结点
 */
TreeNode* RBTree::leftRotation(TreeNode* node)
{
    TreeNode* rightNode = node->rightChild;//获取右子结点
    bool tempColor = node->color;//保存自己的结点颜色
    node->color = rightNode->color;//下面两行为两结点互相交换颜色
    rightNode->color = tempColor;
    node->rightChild = rightNode->leftChild;//将右子结点的左孩子给node
    rightNode->leftChild = node;//将右子结点的左孩子设为node
    return rightNode;
}
/**
 * @description: 右旋转函数
 * @param {TreeNode*} node:树中的一个结点
 * @return {TreeNode*} 返回原先node所在位置的新结点
 */
TreeNode* RBTree::rightRotation(TreeNode* node)
{
    //和左旋转时镜像的
    TreeNode* leftNode = node->leftChild;
    bool tempColor = node->color;
    node->color = leftNode->color;
    leftNode->color = tempColor;
    node->leftChild = leftNode->rightChild;
    leftNode->rightChild = node;
    return leftNode;
}
/**
 * @description: 递归版插入函数的辅助函数
 * @param {TreeNode*} node:树中的一个结点
 * @param {int &} value：待插入关键字的值
 * @return {TreeNode*}: 树结点
 */
TreeNode* RBTree::auxRecursiveInsert(TreeNode* node, const int & value)
{
    //首先寻找待插入位置
    if(node == nullptr)//为空结点，说明找到了代插入的位置,构造新节点并直接返回
    {
        node = new TreeNode(value);
        ++_treeSize;
        return node;
    }
    if(isRed(node->leftChild) && isRed(node->rightChild))//如果左右两个孩子结点都是红色，则进行颜色转换
    {
        //这里对应于插入3-结点的情况3的最后一步，或者插入3-结点的情况2的最后一步，或者插入-3结点情况1的第一步
        flipColor(node);
    }
    if (node->value < value)//如果比value小，递归下降到右子结点
    {
        node->rightChild = auxRecursiveInsert(node->rightChild, value);
    }
    else if(node->value > value)//否则下降到左子结点
    {
        node->leftChild = auxRecursiveInsert(node->leftChild, value);
    }
    else//如果找到该关键字，只接返回
    {
        return node;
    }
    //接下来更具情况维持树的平衡,由于这些情况在自顶向下的讨论中都见到了，不再赘述
    if(isRed(node->leftChild) && isRed(node->leftChild->leftChild))
    {
        //单旋转，调整位置
        node = rightRotation(node);
    }   
    else if(isRed(node->rightChild) && isRed(node->rightChild->rightChild))
    {
        //单旋转，调整位置
        node = leftRotation(node);
    }
    else if(isRed(node->leftChild) && isRed(node->leftChild->rightChild))
    {
        //双旋转，调整位置
        node->leftChild = leftRotation(node->leftChild);
        node = rightRotation(node);
    }
    else if(isRed(node->rightChild) && isRed(node->rightChild->leftChild))
    {
        //双旋转，调整位置
        node->rightChild = rightRotation(node->rightChild);
        node =leftRotation(node);
    }
    return node;//返回
}
/**
 * @description: 递归版删除函数的辅助函数
 * @param {TreeNode*} node:树中的一个结点
 * @param {int &} value：待删除关键字的值
 * @return {TreeNode*}: 树结点
 */
TreeNode* RBTree::auxRecursiveErase(TreeNode* node, const int & value)
{
    static TreeNode* storedNode = nullptr;//用于保存含有待删除关键字结点
    static bool flag = false;//false表示双黑结点当前不存在
    if(node == nullptr)//空结点，直接返回
    {
        return nullptr;
    }
    if(node->value == value)
    {
        --_treeSize;
        //因为找到待删除关键字所在的结点后，我们要找到其后继及诶单，所以先检查其右子结点是否为空
        if(node->rightChild == nullptr)//如果其右孩子结点为空，说明其无后继结点可寻，需要删除这个结点
        {
            if(isRed(node->leftChild))//如果其有左孩子，将左孩子染黑，返回左孩子,同时删除当前结点
            {
                TreeNode* tempNode = node->leftChild;//保存其右孩子
                tempNode->color = false;//左孩子染黑
                delete node;//删除当前结点
                return tempNode;//返回左孩子
            }
            if(!isRed(node))//删除了黑色结点，更新标记
            {
                flag = true;
            }
            delete node;//否则直接删除当前结点，返回空指针
            return nullptr;
        }
        //如果存在右子结点我们就去寻找其后继，在这之前保存node
        storedNode = node;
    }
    if (value < node->value)//向左子结点下降
    {
        //由于我们是要寻找后继，如果从左下降的左子结点为空并且storedNde不为空，那么就说明找到了后继
        if(node->leftChild == nullptr && storedNode != nullptr)
        {
            storedNode->value = node->value;//将后继结点的值覆盖保存的含有待删除关键字所在的结点。
            storedNode = nullptr;//任务完成后置为空。
            //接下来要删除后继结点
            if(node->rightChild != nullptr)//检查右子结点是否存在,如果存在其必然为红色
            {
                TreeNode* tempNode = node->rightChild;
                tempNode->color = false;
                delete node;
                return tempNode;
            }
            if(!isRed(node))//删除了黑色结点，更新标记
            {
                flag = true;
            }
            delete node;
            return nullptr;
        }  
        node->leftChild = auxRecursiveErase(node->leftChild, value);
        //下面回溯时需要根据需要调整
        if(flag)
        {
            TreeNode* tempNode = node;//临时变量，保存node
            if(isRed(node->rightChild))//首先判断兄弟结点是否为红色，来判断是否为表面兄弟
            {
                tempNode = leftRotation(node);//如果是执行左旋转
            }
            if(!is2_Node(node->rightChild))//再判断兄弟结点是否有结点借(不是2-结点)，注意向兄弟结点借的优先级是比从父结点借高的
            {
                if(isRed(node->rightChild->leftChild))//根据兄弟结点中红色子结点的位置来判断是否需要双旋转
                {
                    node->rightChild = rightRotation(node->rightChild);
                }
                //下面执行左旋转
                if(tempNode != node)//这里的tempNode用法和自顶向下删除那里的用法是一样
                {
                    tempNode->leftChild = node = leftRotation(node);
                }
                else
                {
                    node = tempNode = leftRotation(node);
                }
                node->rightChild->color = false;
                //调整好后就平衡了，更新双黑结点标志返回
                flag = false;
            }//如果上面不行，这里就判断父亲结点是否有结点可借
            else if(isRed(node))//判断是否为红色
            {
                //如果是，两者交换颜色，让其和兄弟结点交换颜色，此时兄弟结点必然为2-结点
                std::swap(node->color, node->rightChild->color);
                flag =false;//平衡了，调整双黑结点标志
            }
            else//否则父结点和兄弟结点都为2-结点
            {
                node->rightChild->color = true;//将兄弟结点变为红色,继续向上回溯;
                if(node == _root)//当为根结点时，已经平衡，调整双黑结点标志
                {
                    flag = false;
                }
            }
            return tempNode;
        }
    }
    else//向右子结点下降
    {
        node->rightChild = auxRecursiveErase(node->rightChild, value);
        //这个过程和上面是镜像的，不在赘述
        if(flag)
        {
            TreeNode* tempNode = node;
            if(isRed(node->leftChild))
            {
                tempNode = rightRotation(node);
            }
            if(!is2_Node(node->leftChild))
            {
                if(isRed(node->leftChild->rightChild))
                {
                    node->leftChild = leftRotation(node->leftChild);
                }
                if(tempNode != node)
                {
                    tempNode->rightChild = node = rightRotation(node);
                }
                else
                {
                    node = tempNode = rightRotation(node);
                }
                node->leftChild->color = false;
                flag = false;
            }
            else if(isRed(node))
            {
                std::swap(node->color, node->leftChild->color);
                flag =false;
            }
            else
            {
                node->leftChild->color = true;
                if(node == _root)
                {
                    flag = false;
                }
            }
            return tempNode;
        }
    }
    return node;
}
/**
 * @description: 打印函数的辅助函数，采用中序遍历打印
 * @param {TreeNode*} node:树中的一个结点
 * @return {*}
 */
void RBTree::auxPrint(TreeNode* node)
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
 * @description: 析构函数的辅助函数，采用后序遍历释放内存空间
 * @param {TreeNode*} node:树中的一个结点
 * @return {*}
 */
void RBTree::auxDestructor(TreeNode* node)
{
    if(node == nullptr)
    {
        return;
    }
    auxDestructor(node->leftChild);
    auxDestructor(node->rightChild);
    delete node;
}