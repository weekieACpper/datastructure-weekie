# B-树

## 一、一些在学习此内容所需要的准备

### 1 需要提前了解的知识	

​	1、了解树的定义，熟悉二叉搜索树的基本操作。

​	2、知道什么是内部结点，什么是根节点，什么是叶子结点，以及在二叉搜索树当中的==前驱==和==后继==的概念。

​	3、数列的基本概念，等比数列求和公式。

​	4、时间复杂度的计算，以及记号。

​	5、熟悉c或 c++的基本语法。

### 2 一些命名上的说明

​	假设x为树中的一个结点，==x.key==代表了结点的关键字，==x.key~i~== 代表了x节点中第i个关键字，

​	==x.c==代表结点下的孩子结点，==x.c~i~==代表x结点所对应的其第i个孩子结点，一般x中存储的是指向孩子结点的指针。

## 二、B-树的定义

​	B-树是一颗多路查找树，每个节点相对于二叉搜索树来说可以有更多的分支因子(一个结点的孩子结点数量)，分支因子的数量取决于关键字相对于磁盘一块的大小，我们称一个B-树结点可能含有最大分支因子数(孩子结点数)为B-树的==阶数==。

​	==如下图为一颗5阶B树:==(这个图是网上随手找的，之后演示示例的图是自己做的)

​	

![src=http---upload-images.jianshu.io-upload_images-1653926-0e6ffb881b9f58ec.png&refer=http---upload-images.jianshu.io&app=2002&size=f9999,10000&q=a80&n=0&g=0n&fmt=jpeg](https://tva1.sinaimg.cn/large/008i3skNgy1gxl83xdgbqj30wc07274j.jpg)

### 1 B-树的特性

​	1、每个叶子节点具有相同的深度，即树的高度h。

​	2、每个(非叶子)节点有x个关键字，那么就一定有x+1个指向起孩子的指针，并且这x+1个指针都是非空的。

​	3、叶子节点没有孩子，所以其指向孩子的指针都为空指针。

​	4、节点中的每个关键字以非降序排放，即key~1~ $\le$ key~2~ $\le$...$\le$key~n~。

​	5、假设一个非叶子节点y，关键值为y.key~i~ ，指向孩子节点的指针为y.c~i~ ,k~i~ 为任意一个以y.c~i~ 为根的子树的关键字，则:

​							k~1~ $\le$ y.key~1~$\le$k~2~$\le$ y.key~2~ $\le$ ... $\le$y.key~n~ $\le$k~n+1~ 

​	6、每个节点包含的关键字个数有上界和下界，用一个称为**最小度数**t $\ge$ 2来表示这些界(t的大小和磁盘块有关)。

​		a. 除了根节点外每个节点至少有t-1个关键字，t个孩子，在树非空的情况下根节点至少有1个关键字，2个孩子。

​		b. 每个节点最多可包含2t-1个关键字，以及2t个孩子，当一个节点恰好有2t-1个关键字时，我们说这个节点是**满的**。

### 2 B-树的高度

​	B树上所有的操作和树的高度成正比，现在假设树总共存储了了n个关键字，最小度数为t，高度为h，则三者满足下列关系:	
$$
h\le\log_t~\frac{n+1}{2}
$$
​	==证明:==*根节点*至少有有一个关键字，两个孩子节点，高度为1时这两个孩子节点至少有2t-2个关键字，以及2t个
$$

$$
孩子，高度为2时，这2t个孩子至少有2t*(t-1)个关键字，2t^2^ 个孩子，依次类推，所以当高度为h时的关键字的个数为:			
$$
n\ge1+(t-1)\sum_{i=1}^{h}2t^{i-1}=1+2(t-1)(\frac{t^h-1}{t-1})=2^h-1
$$
​	因此，通过该不等式反算h即可得到上述结果。

​	==示例:==

![b树示例.image](https://tva1.sinaimg.cn/large/008i3skNgy1gxl9jbgc9rj30jc08r74h.jpg)

## 三、B-树的基本操作

​	以下代码实现将用c++语言描述B树的操作。

### 1 B-树结点的定义

以下是B树结点的定义:

==实现代码:==

```c++
struct TreeNode
{
    TreeNode():nodeSize(0),isLeaf(false){}//构造函数
    int nodeSize;//当前结点关键字个数
    bool isLeaf;//判断是否为叶子结点，如果是为true，反之为false
    int valueList[2 * MINIMUM_DEGREE - 1];//关键字数组，个数为最大2t-1
    TreeNode* pointList[2 * MINIMUM_DEGREE];//指向孩子结点的指针数组，一个结点最大拥有2t个孩子结点
};
```

### 2 搜索B-树

​	与二叉搜索树的算法类似，从根节点开始，判断要寻找的值和该节点值的大小关系，假设当前遍历到结点x，待搜索的值为v，如果x.key~i~ =v ，直接返回当前结点以及i。如果x.key~i~ <v就移动到结点中的下一个关键字，继续判断。但如果key~i~已经是最后一个关键字了，(非叶子结点)那么需要访问下一个孩子结点，即x.c~i+1~ ，不过要是当前结点为叶子节点，说明没有孩子结点，则这种情况下应该返回空值。最后就是当出现x.key~i~ >v的情况，(非叶子结点)此时应该访问孩子结点x.c~i~同样(我们把x.key~i~当前所在的位置称为==下降点==，也就是从这个位置下降去寻找关键字),如果该结点为叶子结点，直接返回空值。

==示例:==![b-树2](https://tva1.sinaimg.cn/large/008i3skNgy1gxlcasos5gj30b4054aa1.jpg)

在上图中的最小度数为2的B树，寻找关键值为14的节点首先从根节点开始判断，比20小向左子节点移动，然后判断10，比14小向右移动，再判断13比14小，向右移动，发现16比14大，沿着16(16所在位置为下降点)左边的指针向其孩子结点移动，最后找到目标结点。

==实现代码:==

**非递归版本:**

```c++
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
        if(i != targetNode->nodeSize && targetNode->valueList[i] == value)//如果在该结点中找到对应的关键字，返回该结点，以及其				对应的索引
        {
            return{targetNode,i};
        }
        targetNode = targetNode->pointList[i];//否则就下降到孩子结点，在孩子结点中寻找
    }
    //循环后targetNode处在叶子结点
    for(i = 0; i < targetNode->nodeSize && targetNode->valueList[i] < value; i++);
    if(i != targetNode->nodeSize && targetNode->valueList[i] == value)
    {
        return {targetNode,i};
    }
    //如果叶子结点中也未找到关键字，则返回空指针以及-1的索引  
    return {nullptr,-1};
}
```

**递归版本:**

```c++
std::pair<TreeNode*,int> BTree::recursiveFind(const int & value)
{
    if(_root == nullptr)
    {
        return {nullptr,-1};
    }
    return auxRecursiveFind(_root,value);//递归调用辅助函数
}
std::pair<TreeNode*,int> BTree::auxRecursiveFind(TreeNode* node,const int & value)
{
    size_t i = 0;
    for(; i < node->nodeSize && node->valueList[i] < value; i++);
    if (i != node->nodeSize && node->valueList[i] == value)//如果找到了，返回该结点的指针和索引
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
```

==时间复杂度:==最坏情况下，搜索结点为叶子节点，因此需要遍历O(h)个结点,每个节点内部需要O(t)次迭代,所以总共需要

O(th)=O(t$\log_t\frac{n+1}{2}$)=O(t$\log_tn$)=O($\lg n$)。

### 3 创建一棵空的B-树

这个较为简单，创建一个空的根节点即可。

==实现代码==:

```c++
BTree::BTree()
{
    _root = new TreeNode();
    _root->isLeaf = true;//只有一个根结点的时候，根结点既是根也是叶子结点
}
```

==时间复杂度:==O(1)。

### 4 向B-树中插入一个关键字

​	B树的插入操作和二叉搜索树不太一样，虽然刚开始都要去通过搜索寻找待插入的位置，但它不能直接创建一个新的叶结点。因为根据B树的性质，每个叶子结点的高度必须是一致的，如果在已有平衡的B树上再创建新的叶结点，那么这个新的叶结点相对于其他的叶结点的高度h必然是h+1，破坏了此性质，所以必须在已经存在的叶子结点上插入新关键字(注意不能在非叶子节点上插入新的值，即使该非叶子节点未满，原因是在非叶子结点插入新的关键字，必然要多出一个孩子结点对应，只有叶子结点不需要，因为叶子结点里面存储的是空指针，它没孩子，因为没有孩子结点对应，所以这破坏了B树的性质2，如果有n个关键字，必然有n+1个指向其孩子的结点)。当叶子结点已经满时，不能在插入新关键字了，此时必须执行==分裂操作== ，将一个满的结点(2t-1关键字)的结点根据==中间关键字==分裂成两个t-1关键字的结点,**中间关键字**提升到父结点(在父结点未满的情况下)，如果父结点已满则进行递归操作继续(分裂父结点,依次类推)。

​	但这样做会有个问题，从根节点找到插入的节点是向下的，回溯父节点的是向上的，分裂最终会向上传播，这造成了不必要的开销，为了做到单程向下，不能等到在插入过程中找到实际分裂的满节点才分裂，相反是在寻找插入位置的过程中，==遇到的每个满结点都分裂==，原因是，我们插入结点的节点走过了这条路,在插入位置节点(需要分裂的情况下)，回溯回来必然也会分裂该路径上的满结点，所以相当于提前把这件事做好，保证了不会有回溯的过程，实现单程向下的操作，提高了运行效率。 首先介绍分裂结点的操作。

#### 4.1 分裂结点

​	当一个B树的一个非满的内部结点x,假设其内部的当前的关键字数为2t-2(也就是说再插入一个就满了),将要被插入一个新的关键字，此时该结点应当被分裂。如果被分裂的结点为根节点，则应该创建一个新的空的根节点，新的根节点用来接收**中间关键字**，因此我们可以得出这样一个结论，B树为一增长高度的方法是通过分裂根节点实现的，此时高度+1。

==下图是一个**最小度数**为3(t=3)中一个节点的分裂:==

![b_树3](https://tva1.sinaimg.cn/large/008i3skNgy1gxlehxvz7vj30hh0583yn.jpg)

可以看到待分裂节点的**中间关键字**13被提上父结点对应的位置(10和20之间)。

==同样下面为根节点分裂的情况:==

![b_树4](https://tva1.sinaimg.cn/large/008i3skNgy1gxlepl6nolj30d206qwek.jpg)

==实现代码:==

```c++
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
    TreeNode* tempNode=rightNode;
    for(int i = index; i <= parentNode->nodeSize; i++)
    {
        std::swap(temp,parentNode->valueList[i]);
        std::swap(tempNode,parentNode->pointList[i + 1]);
    }
    //完成之后,父结点的关键字个数需要加1
    ++parentNode->nodeSize;
    leftNode->nodeSize=middle;//更改左孩子的关键字个数，限制访问后面的元素即可，无需把后面的元素清零
}   
```

==时间复杂度:==$\theta$(t),为循环中移动关键字的时间开销。

#### 4.2 单程向下插入关键字

​	寻找插入节点时沿途进行分裂操作，保证了最后插入的叶子节点绝对不是满的。

==基本步骤:==

​	（1）从根节点开始和搜索操作一样，对比每个结点的每个关键字，根据大小判断下降到那个孩子结点（注意如果只允许不重复的关键字，那么如果找到相同的关键字时，直接返回，如果支持重复则继续插入）。

​	（2）途中遇到满的孩子结点，进行分裂，其中有两种情况，非根节点和根节点。

​	（3）找到叶子节点，遍历内部关键值数组找到对应位置插入,(注意，由于我们在途中把满子结点都分裂了，所以可以轻易证明，叶子结点肯定是未满的，可以直接插入)。

==示例:==

![b_树5](https://tva1.sinaimg.cn/large/008i3skNgy1gxntsw2mrqj30dc0jldgk.jpg)

​	

​	上面的示例中，最小度数为2，我们插入15，首先发现根结点满了，我们创建一个新的根结点，然后接受原先满的根结点的**中间关键字**然后是指称为新的根结点，紧接着向下搜寻，在20那里，发现子节点(2,8,14)满了，将其分裂，中间关键字提到20所在的父结点中，然后继续下降到子节点(14),发现为叶子结点，插入到对应位置。

==实现代码:==

```c++
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
        if(i != targetNode->nodeSize && targetNode->valueList[i] == value)
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
```

==时间复杂度:==和搜索基本是一样的,O(t$\log_tn$)=O($\lg n$),其中有多出来的分裂操作，由于其时间复杂度为$\theta$(t)<O($\log_tn$),所以最终取上面的值。

### 5 从B-树中删除一个关键字

​	B树可以从任意节点中删除一个关键字，不同于插入，当删除一个关键字时，还需重新安排这个节点的孩子。在真正进行讨论前，我们首先考虑，当要删除的节点关键字个数为t-1时，删除后，该结点违反了B树的性质，因此这种情况会导致向上回溯（需要向父结点借来关键值，来满足最小度数的要求），就和插入一个关键值到一个满的叶子结点上，我们得保证在真正进行删除时，删除的节点中的关键字至少为t(注意最小度数为t-1，这里多一个)，这样可以避免向上的回溯操作(但有一个例外)，大多数情况是不需要回溯的)。

​	**在以上的保证下**，我们将分情况讨论:

==情况讨论:==

​	1、如果删除的关键字在叶子结点上，直接删除，并根据删除的位置在结点内部移动别的关键字(由于以上保证，下降到叶子结点关键字个数>t-1)。

​	2、如果关键字k在结点x中，x为内部结点，则再分以下三种**情况**:

​		首先我们假设在x中x.key~i~ =k, x.c~i~ =y, x.c~i+1~ =z, y、z分别为孩子结点。

​		a. 如果在y中的关键字结点的个数至少为t，那么，我们从y结点开始，单程向下搜索(搜索过程中要考虑情况3，因为k~s~不在内部结点x中)，找到其==前驱==k~s~, 并把k~s~的值赋给k也就是两者值交换，删除k~s~。

​		b. 如果y中的关键字结点的个数为t-1，但z为t，那么对称地从z处开始单程向下搜索,找到==后继==，操作同上。

​		c. 如果y、z中的关键字都为t-1，那么k和z全部合并进入y，这样做不会破坏B树的性质，因为合并之后的结点（假设为m）m的关键字个数为2t-1(t-1+t-1+1)，而孩子结点数正好为t+t为2t,(两结点合并的孩子数)，符合要求，二父结点在缺少一个关键字后，对应也要少一个孩子，而那个孩子正好是z结点。最后在m中递归删除k，这种情况需要回溯，因为关键字正好在刚刚合并的子结点中，因此下降到子结点时，将再次进入情况2。

​	3、如果k不在内部结点x中(假设k在树中，只是不在当前下降到的节点中)，则我们可以肯定它肯定在某个孩子结点(x.ci)为根的子树中,如果x.ci只有t-1个关键字（如果比这个大，这几往下继续下降即可），我们分为下面两个情况来保证下降至一个至少为含有t个关键字的结点:

​		a. 如果x.c~I~ 只含有t-1个关键字，但其相邻的兄弟节点(x.c~i-1~, x.c~i+1~)至少有一个包含t个关键字，我们将x中对应的关键字下降到x.c~i~ 中，兄弟节点的一个关键字上升到x中，这里需要把兄弟节点上升的关键字所对应的孩子结点移到x.c~i~中，目的是保持B树的性质2。因为兄弟结点损失了一个关键字，所以多了个孩子，得把这个孩子放到x.c~i~的对应地方。(等效于**AVL树**中的==旋转==，LL或RR，这里不了解也没关系)。然后在x.c~i~继续单程向下。

​		b. 如果所有相邻兄弟结点都只包含t-1个关键字，则将x.ci与其中一个兄弟节点合并,同样也要从x中拿下一个关键字，合并操作2c情况是一样的。

我们可以看到插入有分裂，删除有合并，天然平衡(题外话)。

==示例1:==

​		可见，B-树的删除非常复杂，这里将用几个较为复杂的案例说明，删除的过程时如和进行的，仍然令t=2(画图少画几个格子)。



​		     ![<!--b_树6-->](https://tva1.sinaimg.cn/large/008i3skNgy1gxnygqawp2j30lb0vjgo4.jpg)

​        

[^weekie]: 注意第三棵树的合并包括了67这个关键字。

​		在上面一张图的示例中，我们可以看到，删除的点60就在根节点中，所以进入了情况2。我们要检查在60左右两边的孩子结点中关键字的数量，发现其右子结点关键字个数>1，所以触发情况2b。我们开始寻找后继结点，根据后继结点的定义，我们从67左侧往下走，但是该结点的关键字为1(t-1)，触发情况3。我们检查其兄弟结点的关键字个数，发现右侧唯一兄弟结点的关键字个数也为1(t-1)，触发情况3b，我们从父结点取出67，将其和兄弟结点以及之前要下降到的结点(63所在)合并,下降到这个新的合并结点，发现其为叶子结点，所以我们找到后继63，由于其在叶子结点中将其和60交换(其实直接用63覆盖掉60就行)，然后删除它，结束。

==示例2:==

![b_树7](https://tva1.sinaimg.cn/large/008i3skNgy1gxo0a25oyej30gf0jfwfn.jpg)

​		这次在根节点处找到了待删除结点，进入情况2，但是之后两孩子结点关键字个数都为1，因此将两孩子结点和63合并，之后递归删除即可。

==示例3:==

​                       ![b_树8](https://tva1.sinaimg.cn/large/008i3skNgy1gxo15ywy71j30hx0oywg3.jpg)   

​		 

​		上面这个例子中，可以看到一开始就触发了合并操作，原先只有一个关键字的根结点消失，这将会导致整棵树的高度下降1。因此我们可以得出，b-树唯一下降高度的方式是通过合并操作。之后由于寻找的24不在节点中，因此触发了情况3a，此时只要进行右旋操作即可，注意不能把21提上来，因为这样违反了B-树的性质5，因为22>21,但22却在21的左子结点上。



<!--注意，合并操作和分裂，以及旋转都将涉及操作结点(比如上图中的22)的孩子结点的指针的位置转换，假如说上图中的22右侧有子结点，(在满足b树的性质下)，那么该子结点应当被连接到23的左侧子结点处，因为根据b树的性质该子结点比22大但比23小(假设允许浮点数，😂(22和23之间没整数了，图又画完了，就这样吧))，22上去了，该子结点不能被孤立，正好23下去了多一个位置，所以连接在那。示例中并未给出，这将在代码部分完成-->

#### 5.1 合并结点

​		在正式介绍删除操作的代码之前，我们需要先了解合并结点的操作，合并结点的操作可以看作是分裂结点操作的逆过程。

==实现代码:==

```c++
void BTree::mergeTreeNode(TreeNode* node,int index)
{
  	//node为父结点
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
```

==时间复杂度:==$\theta$(t),为循环中移动关键字的时间开销

==示例:==

![b_树9](https://tva1.sinaimg.cn/large/008i3skNgy1gxouc0g5nqj30a30bjmxa.jpg)



​		在上图中的事例中，根结点下的两个子结点和根结点的唯一关键字合并，原来的根结点消失，树的高度减1，合并后的结点成为新树根，可以看到，它是分裂的逆操作。当待合并结点的父结点关键字不唯一时，可以将分裂操作的图倒过来看，这里就不举例了。

#### 5.2 删除操作

​		下面正式给出删除操作的代码

==实现代码:==

**首先介绍处理情况3的函数:**

```c++
void BTree::auxErase(TreeNode* node, int index)
{
  	//node为父结点
    TreeNode* opNode = node->pointList[index];//获取从父结点将要下降到的结点
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
```

下面为正式的删除函数:

```c++
void BTree::erase(const int & value)
{
    TreeNode* targetNode = _root;
    //首先要对根结点的关键字个数以及其孩子结点的关键字个数进行判断根结点的唯一关键字是否要下降
    if(_root->nodeSize == 1 && !_root->isLeaf && _root->pointList[0]->nodeSize == MINIMUM_DEGREE - 1 && _root->pointList[1]->nodeSize == MINIMUM_DEGREE - 1)
    {
        targetNode = targetNode->pointList[0];
        //下面合并根结点的两个孩子，删除原来根结点，让合并后的结点成为新的根结点，树高度-1
        mergeTreeNode(_root, 0);
        delete _root;//删除原来的根结点
        _root = targetNode;
    }
    size_t index = 0;
    while(!targetNode->isLeaf)
    {
        for(index = 0; index < targetNode->nodeSize && value > targetNode->valueList[index]; ++index);//先通过循环找到下降点
        if (index != targetNode->nodeSize && targetNode->valueList[index] == value)//如果找到了要删除的关键字，我们跳出循环，继续下一步操作
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
            //以下进入情况2，我们需要找到其前驱(2a)或者后继(2b),必要情况下还需要合并(2c),首先我们得先	判断下降点关键字两侧兄弟节点的关键字数量
            if(targetNode->pointList[index]->nodeSize > MINIMUM_DEGREE - 1)//判断下降点关键字左侧孩子结点的关键字数量是否大雨t-1
            {
                //如果成立，我们将下降到左子结点去寻找前驱
                targetNode = targetNode->pointList[index];
                while(!targetNode->isLeaf)
                {
                    //在寻找钱去的过程中，可能会遇到情况3，首先检查要下降的子结点关键字数量是否满足要求
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
                tempNode->valueList[index] = targetNode->valueList[0];//覆盖掉之前找到的待删除																																				关键字
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
                if(targetNode->isLeaf)//这里的情况是因为，刚好把要删除的关键字下降和两个叶子结点合																					并了，这时应该跳出循环，交给后面情况1代码部分处理
                {
                    break;
                }
                flag = true;//置为true，因为这里会再次进入了情况2
            }        
        }        
    }
    //如果为叶子结点，有可能是因为这个节点根本就不在树里，直接返回，否则则直接将索引处的关键字删除，待删除删除关键字后面的关键字往前面平移
    for(index = 0; index < targetNode->nodeSize && targetNode->valueList[index] < value; ++index);//在叶子结点中寻找待删除的结点
    if(targetNode->valueList[index] != value)
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
```

==时间复杂度:==与插入是一样的，因为合并和分裂对等，旋转操作为O(1)。

## 四、应用场景

​		针对大量数据的情况，需要将额外的数据存储进磁盘等i/o设备，例如数据库，文件系统等。

## 五、如有错误或描述不当，请多多指教！不胜感激！

![](https://tva1.sinaimg.cn/large/008i3skNgy1gxljz3hdk0j30go0nkgms.jpg)

​																																											

​																																				                               2021年 12月21日

​                                                       	                                                                                                   weekie_OUO

​																																						

​																	