/*
 * @Author: weekie
 * @Date: 2021-12-23 20:34:19
 * @LastEditTime: 2022-01-18 22:48:35
 * @LastEditors: Please set LastEditors
 * @Description: 本文件主要是B一些操作的实现部分
 * @FilePath: /datastructure/B-tree/B_tree.cpp
 */
#include<B-tree.hpp>
/**
 * @description: B树的构造函数
 * @param {*}
 * @return {*}
 */
BTree::BTree()
{
    _root = new TreeNode();
    _root->isLeaf = true;//只有一个根结点的时候，根结点既是根也是叶子结点
}
/**
 * @description: B树的析构函数。调用辅助析构函数析构
 * @param {*}
 * @return {*}
 */
BTree::~BTree()
{
    auxDestructor(_root);
}
/**
 * @description: 用于获取B树中关键字的数量
 * @param {*}
 * @return {*}
 */
int BTree::size()
{
    return _elementNum;
}
/**
 * @description: 判断B树是否为空，也即无关键字
 * @param {*}
 * @return {bool};B树为空时返回true反之为false
 */
bool BTree::empty()
{
    return !(_elementNum == 0);
}
/**
 * @description: 非递归的查找关键字value
 * @param {int &} value:待查找的关键字
 * @return {std::pair<TreeNode*,int>}：返回关键字所在的结点和关键字的索引，没找到返回空指针和-1
 */
std::pair<TreeNode*,int>  BTree::find(const int & value)
{
    if(_root == nullptr)
    {
        return {nullptr,-1};
    }
    TreeNode* targetNode = _root;//定义搜寻的结点指针
    size_t i = 0;
    while(!targetNode->isLeaf)
    {
        for(i = 0; i < targetNode->nodeSize && targetNode->valueList[i] < value; i++);
        if(i!= targetNode->nodeSize && targetNode->valueList[i] == value)//如果在该结点中找到对应的关键字，返回该结点，以及其对应的索引
        {
            return{targetNode,i};
        }
        targetNode = targetNode->pointList[i];//否则就下降到孩子结点，在孩子结点中寻找
    }
    //循环后targetNode处在叶子结点
    for(i = 0; i < targetNode->nodeSize && targetNode->valueList[i] < value; i++);
    if(i!= targetNode->nodeSize && targetNode->valueList[i] == value)
    {
        return {targetNode,i};
    }
    //如果叶子结点中也未找到关键字，则返回空指针以及-1的索引  
    return {nullptr,-1};
}
/**
 * @description: 递归的查找关键字value
 * @param {int &} value:待查找的关键字
 * @return {std::pair<TreeNode*,int>}：返回关键字所在的结点和关键字的索引，没找到返回空指针和-1
 */
std::pair<TreeNode*,int> BTree::recursiveFind(const int & value)
{
    if(_root == nullptr)
    {
        return {nullptr,-1};
    }
    return auxRecursiveFind(_root,value);//递归调用辅助函数
}
/**
 * @description: 在B树中插入一个关键字为value的元素
 * @param {int &} value:待插入的关键字
 * @return {*}
 */
void BTree::insert(const int & value)
{
    //如果根节点关键字个数满了，我们需要创建一个新的根节点接收原来根结点的中间关键字
    if(_root->nodeSize == 2 * MINIMUM_DEGREE - 1)
    {
        TreeNode* newRoot = new TreeNode();//构造出来的新根默认不是叶子节点，请参考TreeNode构造函数
        newRoot->pointList[0] = _root;//将新的根结点的第一个孩子指针先指向原来的根结点
        splitTreeNode(newRoot, 0);//分裂原来的根结点
        _root = newRoot;//将根结点转换为新的根结点
    }
    TreeNode* targetNode = _root;
    size_t i = 0;    
    while(!targetNode->isLeaf)
    {
        //首先在内部关键字数组中循环，找到下降的位置。
        for(i = 0; i < targetNode->nodeSize && targetNode->valueList[i] < value; i++);
        //如果找到了相等的关键字，直接返回，(该B树不支持重复关键字)，如果要支持
        //可以忽略下面这个if语句
        if(i!= targetNode->nodeSize && targetNode->valueList[i] == value)
        {
            return;
        }
        //检查即将下降到的孩子结点的关键字个数，如果其满了，则将其分裂
        if(targetNode->pointList[i]->nodeSize == 2 * MINIMUM_DEGREE - 1)
        {
            splitTreeNode(targetNode,i);
            //这时候我们需要比较value和刚刚上升的中间关键字的大小，已决定从这个关键左侧或者右侧下降
            if(value > targetNode->valueList[i])//如果比刚刚上升的中间关键字大，从右侧下降
            {
                i += 1;//右侧下降，也即非裂后的右子结点
            }
        }
        targetNode = targetNode->pointList[i];//下降至子结点
    }
    //循环结束后，已经下降到叶子结点，直接插入即可，叶子结点必然不是满的(详情见BTree.md)
    for(i = 0; i < targetNode->nodeSize && targetNode->valueList[i] < value; i++);
    int temp = value;
    for(;i <= targetNode->nodeSize; i++)//插入位置后面的关键字需要向后移动
    {
        std::swap(temp, targetNode->valueList[i]);
    }
    ++targetNode->nodeSize;//该叶子结点关键字数量+1
    ++_elementNum;
}
/**
 * @description: 在B树中删除一个关键字
 * @param {int &} value:待删除的关键字
 * @return {*}
 */
void BTree::erase(const int & value)
{
    TreeNode* targetNode = _root;
    //首先要对根结点的关键字个数以及其孩子结点的关键字个数进行判断根结点的唯一关键字是否要下降
    if(_root->nodeSize == 1 && !_root->isLeaf && _root->pointList[0]->nodeSize == MINIMUM_DEGREE - 1 && _root->pointList[1]->nodeSize == MINIMUM_DEGREE - 1)
    {
        targetNode = targetNode->pointList[0];
        //下面合并根结点的两个孩子，删除原来根结点，让合并后的结点成为新的根结点
        mergeTreeNode(_root, 0);
        delete _root;//删除原来的根结点
        _root = targetNode;
    }
    size_t index = 0;
    while(!targetNode->isLeaf)
    {
        for(index = 0; index < targetNode->nodeSize && value > targetNode->valueList[index]; ++index);//先通过循环找到下降点
        if (index != targetNode->nodeSize && targetNode->valueList[index] == value)//如果找到了，我们跳出循环，继续下一步操作
        {
            break;
        }
        //接下来，根据下降点，判断需要下降到子结点的关键字数量，看是否进入情况3
        if(targetNode->pointList[index]->nodeSize == MINIMUM_DEGREE - 1)//如果该子结点关键字个数为t-1，则进入情况3
        {
            auxErase(targetNode, index);
        }
        //下降到该结点上
        targetNode = targetNode->pointList[index];
    }
    //退出循环后，可以知道这里可能是情况2也可能是情况1，先判断targetNode是不是叶子结点
    if(!targetNode->isLeaf)//如果不是叶子结点，进入情况2
    {
        TreeNode* tempNode = targetNode;//临时保存待删除关键字所在的结点，方便后面覆盖
        //这里的变量和循环解释一下，原因就是情况2c执行完之后，我们下降会再次进入情况2，不像另外2a和2c，单程向下直接结束
        //所以执行完情况2c时，得递归进入情况2，而我这里采用了循环控制，不进入情况2c时，这个循环就会结束，进入时，将flag
        //置为true，循环继续        
        bool flag = true;
        while (flag)
        {
            flag = false;
            //以下进入情况2，我们需要找到其前驱(2a)或者后继(2b),必要情况下还需要合并(2c),首先我们得先判断下降点关键字两侧兄弟节点的关键字数量
            if(targetNode->pointList[index]->nodeSize > MINIMUM_DEGREE - 1)//判断下降点关键字左侧孩子结点的关键字数量是否大雨t-1
            {
                //如果成立，我们将下降到左子结点去寻找前驱
                targetNode = targetNode->pointList[index];
                while(!targetNode->isLeaf)
                {
                    //在寻找前驱的过程中，可能会遇到情况3，首先检查要下降的子结点关键字数量是否满足要求
                    if(targetNode->pointList[targetNode->nodeSize]->nodeSize == MINIMUM_DEGREE - 1)//如果下降到的子结点关键字数量为t-1，进入情况3
                    {
                        auxErase(targetNode, targetNode->nodeSize);
                    }
                    targetNode = targetNode->pointList[targetNode->nodeSize];//下降
                }
                //循环结束，必然为叶子结点，其前驱就是叶子结点关键字数组中的最后一个关键字
                tempNode->valueList[index] = targetNode->valueList[targetNode->nodeSize - 1];//覆盖掉之前找到的待删除关键字
                --targetNode->nodeSize;//将叶子结点的关键字数量-1,限制访问
                --_elementNum;//树中关键字元素-1
                return;
            }
            else if (targetNode->pointList[index + 1]->nodeSize >MINIMUM_DEGREE - 1)//如果上一个不成立，检查下降点右侧子结点的关键字数量
            {
                //如果成立，我们下降到右子结点寻找后继
                targetNode = targetNode->pointList[index + 1];
                while(!targetNode->isLeaf)
                {
                    if(targetNode->pointList[targetNode->nodeSize]->nodeSize == MINIMUM_DEGREE - 1)//如果下降到的子结点关键字数量为t-1，进入情况3
                    {
                        auxErase(targetNode, 0);
                    }
                    targetNode = targetNode->pointList[0];
                }
                //和上面不同的是，用叶子结点第一个关键字覆盖
                tempNode->valueList[index] = targetNode->valueList[0];//覆盖掉之前找到的待删除关键字
                //由于使用第一个关键字覆盖，需要将后面的关键字往前移动
                for(size_t i = 1; i < targetNode->nodeSize; ++i)
                {
                    targetNode->valueList[i - 1] = targetNode->valueList[i];
                }
                --targetNode->nodeSize;//将叶子结点的关键字数量-1,
                --_elementNum;//树中关键字元素-1
                return;
            }
            else//如果两者都不成立，我们将下降点的左右两子结点合并，此后，会再此进入情况2，因此在外面会有个大循环
            {
                mergeTreeNode(targetNode, index);
                targetNode = targetNode->pointList[index];
                if(targetNode->isLeaf)//这里的情况是因为，刚好把要删除的关键字下降和两个叶子结点合并了，这是应该跳出循环，交给后面情况1代码部分处理
                {
                    break;
                }
                flag = true;//置为true，因为这里会再次进入了情况2
            }        
        }        
    }
    //如果为叶子结点，有可能是因为这个节点根本就不在树里，直接返回，否则则直接将索引处的关键字删除，待删除删除关键字后面的关键字往前面平移
    for(index = 0; index < targetNode->nodeSize && targetNode->valueList[index] < value; ++index);//在叶子结点中寻找待删除的结点
    if(index == targetNode->nodeSize)
    {
        return;
    }
    for(int i = index + 1; i < targetNode->nodeSize; ++i)
    {
        targetNode->valueList[i - 1] = targetNode->valueList[i];
    }
    --targetNode->nodeSize;//叶子结点的关键字数量-1
    --_elementNum;//树中关键字元素-1
}
/**
 * @description: ，通过调用辅助打印函数打印B树
 * @param {*}
 * @return {*}
 */
void BTree::print()
{
    auxPrint(_root);//调用辅助函数递归打印整棵树
}
/**
 * @description: 辅助的打印函数，采用中序遍历打印b树中的关键字
 * @param {TreeNode*} node
 * @return {*}
 */
void BTree::auxPrint(TreeNode* node)
{
    if(node->isLeaf)//如果该结点为叶子结点，打印该叶子结点的关键字并返回
    {   
        for (size_t i = 0; i < node->nodeSize; i++)
        {
            std::cout << node->valueList[i] << " ";
        }
        return;
    }
    //中序遍历
    size_t i = 0;
    for(; i < node->nodeSize; i++)
    {
        auxPrint(node->pointList[i]);
        std::cout << node->valueList[i] << " ";
    }
    auxPrint(node->pointList[i]);
}
/**
 * @description: 辅助析构函数，采用后序遍历释放B树结点
 * @param {TreeNode*} node
 * @return {*}
 */
void BTree::auxDestructor(TreeNode* node)
{
    if(node->isLeaf)//如果为叶子结点就回收并且返回
    {
        delete node;
        return;
    }
    for(int i = 0; i < node->nodeSize; ++i)
    {
        auxDestructor(node->pointList[i]);
    }
    delete node;
}
/**
 * @description: 删除辅助函数，主要用来处理情况3
 * @param {TreeNode*} node:父结点
 * @param {int} index:待下降结点在父结点pointlist中的索引
 * @return {*}
 */
void BTree::auxErase(TreeNode* node, int index)
{
    TreeNode* opNode = node->pointList[index];
    //接下来我们要检查其兄弟节点的关键字数量以判断是进入情况3a还是进入情况3b
    if(index - 1 >= 0 && node->pointList[index - 1]->nodeSize >= MINIMUM_DEGREE)//这种情况表示左兄弟节点有关键字可以借
    {
        //这里要右旋转，对于左边的兄弟结点，我们应该借其关键字数组中的最大值，因为旋转后要满足
        //性质5，这个值将会移动到父结点对应位置上，右旋转相当于把左兄弟结点的一个关键字往父结
        //点上移，而父结点中的关键字下降到node结点上，左兄弟的上移关键字的右孩子结点需要移到node上
        //这里需要注意的是父亲结点下来的到node上值必然比node上所有关键字要小，因此要放在第一个
        //同样，转移的右孩子结点指针也应该放在第一个，然后其余的向后移。
        TreeNode* brotherNode = node->pointList[index - 1];
        int opValue = node->valueList[index - 1];//保存父结点中待操作的关键字
        TreeNode* opChildPtr = brotherNode->pointList[brotherNode->nodeSize];//保存兄弟结点最大关键字的右侧孩子指针
        node->valueList[index - 1] = brotherNode->valueList[brotherNode->nodeSize - 1];//左兄弟结点最大关键字上移到父亲结点对应位置
        //将opValue下降到opNode第一个关键字位置，opChildPtr同理，同时调整指针和关键字
        for(int i = 0; i <= opNode->nodeSize; ++i)
        {
            std::swap(opValue, opNode->valueList[i]);
            std::swap(opChildPtr, opNode->pointList[i]);
        }
        opNode->pointList[opNode->nodeSize + 1] = opChildPtr;
        ++opNode->nodeSize;//操作完后该结点的关键字数量+1
        --brotherNode->nodeSize;//相应的兄弟结点关键字数量-1，同时限制了访问
    }
    else if(index + 1 <= 2 * MINIMUM_DEGREE - 1 && node->pointList[index + 1]->nodeSize >= MINIMUM_DEGREE)//表示右兄弟结点有关键字可以借
    {
        //我们将进行左旋转，和上面是对称的，但要注意，这里移动兄弟结点第一个关键字和第一个指针
        //并且父亲结点关键字必然下降到opNode当前最后关键字的后一位置
        TreeNode* brotherNode = node->pointList[index + 1];
        opNode->valueList[opNode->nodeSize] = node->valueList[index];
        node->valueList[index] = brotherNode->valueList[0];
        opNode->pointList[opNode->nodeSize + 1] = brotherNode->pointList[0];
        //由于兄弟结点第一个关键字和第一个孩子指针没有了，需要调整
        for(int i = 1; i < brotherNode->nodeSize; ++i)
        {
            brotherNode->valueList[i - 1] = brotherNode->valueList[i];
            brotherNode->pointList[i - 1] = brotherNode->pointList[i];
        }
        brotherNode->pointList[brotherNode->nodeSize - 1] = brotherNode->pointList[brotherNode->nodeSize];
        ++opNode->nodeSize;
        --brotherNode->nodeSize;
    }
    else//这种情况表示两兄弟都没有关键字可以借。
    {
        if(index + 1 <= 2 * MINIMUM_DEGREE - 1)//如果右兄结点存在，和右子结点合并
        {
            mergeTreeNode(node, index);
        }
        else//否则，和左兄弟结点合并,注意这种情况只有在寻找前驱的情况会发生，因为寻找前驱会从一个结点的最右侧的孩子指针下降，此时这个指针是没有右兄弟结点的
        {
            mergeTreeNode(node, index - 1);
        }
    }   
}
/**
 * @description: 辅助查找函数，在一个结点中查找关键字
 * @param {TreeNode*} node:待查找结点
 * @param {int &} value:待查找关键字
* @return {std::pair<TreeNode*,int>}：返回关键字所在的结点和关键字的索引，没找到返回空指针和-1
 */
std::pair<TreeNode*,int> BTree::auxRecursiveFind(TreeNode* node,const int & value)
{
    size_t i = 0;
    for(; i < node->nodeSize && node->valueList[i] < value; i++);
    if (i!= node->nodeSize && node->valueList[i] == value)//如果找到了，返回该结点的指针和索引
    {
        return {node,i};
    }
    else if(node->isLeaf)//如果是叶子结点，并且未找到该关键字，返回空指针和-1；
    {
        return {nullptr,-1};
    }
    //否则继续下降递归搜寻下一个结点
    return auxRecursiveFind(node->pointList[i],value);
}
/**
 * @description: 将父亲结点对应索引处的孩子结点分裂成两个结点，父结点接收其中间关键字
 * @param {TreeNode*} parentNode:父亲结点
 * @param {int} index:待分裂孩子结点在父结点中point_list中的索引
 * @return {*}
 */
void BTree::splitTreeNode(TreeNode* parentNode,int index)
{
    //由于要将索引处的孩子结点非裂成左右两结点，这里保留原结点为左结点，为右结点申请新空间
    TreeNode* rightNode = new TreeNode(); 
    TreeNode* leftNode = parentNode->pointList[index];//获取原分裂结点作为左结点
    rightNode->isLeaf = leftNode->isLeaf;//右结点应该继承原结点是否也叶结点的属性
    int splitSize = leftNode->nodeSize;//获取待分裂结点的关键字个数
    int middle = splitSize >> 1;//获取中间关键字索引
    //将原结点的中间关键字后面的元素以及孩子结点的指针移动至右结点
    for(size_t i = middle + 1,j = 0; i < splitSize; i++,j++)
    {
        rightNode->valueList[j] = leftNode->valueList[i];
        rightNode->pointList[j] = leftNode->pointList[i];
    }
    rightNode->nodeSize = splitSize - middle - 1;
    rightNode->pointList[rightNode->nodeSize] = leftNode->pointList[splitSize];
    //下面将中间关键字插入到父结点中
    int temp = leftNode->valueList[middle];
    TreeNode* tempNode = rightNode;
    for(int i = index; i <= parentNode->nodeSize; i++)
    {
        std::swap(temp,parentNode->valueList[i]);
        std::swap(tempNode,parentNode->pointList[i + 1]);
    }
    //完成之后,父结点的关键字个数需要加1
    ++parentNode->nodeSize;
    leftNode->nodeSize = middle;//更改左孩子的关键字个数，限制访问后面的元素即可，无需把后面的元素清零
}   
/**
 * @description: 合并两个结点以及父结点中的一个关键字
 * @param {TreeNode*} node:父结点
 * @param {int} index:待下降结点在父结点中pointlist中的索引。
 * @return {*}
 */
void BTree::mergeTreeNode(TreeNode* node,int index)
{
    TreeNode* leftNode = node->pointList[index];//获得待合并的左子结点
    TreeNode* rightNode = node->pointList[index + 1];//获得待合并的右子节点
    int middleValue = node->valueList[index];//获得待合并的中间关键字
    int parentNodeSize = node->nodeSize;//获得父结点的关键字个数
    //合并的两个子结点各自的关键字个数必然是t-1
    leftNode->valueList[MINIMUM_DEGREE-1] = middleValue;//将中间关键字先填入左子结点的第t个关键字位置
    leftNode->pointList[MINIMUM_DEGREE] = rightNode->pointList[0];//把右子结点的第一个孩子指针赋给左子节点t+1处指针位置
    //将右子结点剩余的孩子结点指针和关键字全部并入左子结点
    for(size_t i = MINIMUM_DEGREE, j = 0; j < MINIMUM_DEGREE - 1; i++, j++)
    {
        leftNode->valueList[i] = rightNode->valueList[j];
        leftNode->pointList[i + 1] = rightNode->pointList[j+1];
    }
    leftNode->nodeSize = 2 * MINIMUM_DEGREE - 1;//调整左结点关键字个数
    delete rightNode;//释放右子结点的资源
    //下面将要把下降中间关键字的后面的关键字和孩子结点指针向前平移
    for(size_t i = index + 1; i < parentNodeSize; i++)
    {
        node->valueList[i - 1] = node->valueList[i];
        node->pointList[i] = node->pointList[i + 1];
    }
    --node->nodeSize;//父结点关键字个数减1
}