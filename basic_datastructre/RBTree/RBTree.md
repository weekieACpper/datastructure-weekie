# 红黑树

## 一、一些在学习此内容所需要的准备

### 1 需要提前了解的知识

​	1、了解树的定义，熟悉二叉搜索树的基本操作。

​	2、知道什么是内部结点，什么是根节点，什么是叶子结点，以及在二叉搜索树当中的==前驱==和==后继==的概念。

​	3、熟悉AVL树的旋转操作。

​	4、熟悉2-3树和2-3-4树和B树。

​	5、熟悉数学归纳法。

​	6、时间复杂度的计算，以及记号。

​	7、熟悉c或c++的基本语法。

### 2 一些命名上的说明

​	==2-结点==:带有一个关键字和2个指向孩子结点指针的结点，其余n-类推

​	==关键字==:结点中的键值。

​	==中间关键字==:如果一个结点有多个关键字，那么该结点关键字的数量/2对应索引的关键字。

## 二、红黑树的定义

​		红黑树是一个二叉平衡搜索树，有两种树结点，红色结点和黑色结点。具有非常良好的插入、删除和搜寻性能。在正式介绍之前特性，我想先说明一下它和2-3树的关系，正确的来说，一棵红黑树可以等价的用一棵2-3树(或者2-3-4树)来表示，所以在介绍这里之前，读者必须有2-3树的知识。

​		一颗典型的红黑树像下面这样:

![RB_Tree1](https://tva1.sinaimg.cn/large/008i3skNgy1gxp5tnoxfbj309k05c0sr.jpg)



​		其中N代表空结点，也叫外部结点。

### 1 红黑树和2-3树

​		我们把2-3树的的链接(结点与结点之间的线)分为两种类型:

​		1、红色链接:在2-3树中，将两个2-结点连接起来构成3-结点的链接。

​		2、黑色链接:在2-3树中的普通的链接。

​		上面的概念乍一看有点难以理解，其实就是正常2-3树的连接(也就是连接2-3树每个结点的线)为黑色链接。而红色链接在2-3树中是看不到的，因为它是对于每个-3结点==内部==有一条“隐形”的链接把两个关键字链接在一起。如下图:

​		                                ![RB_Tree2](https://tva1.sinaimg.cn/large/008i3skNgy1gxp6f9frmzj3086066jrd.jpg)



​		现在我们将带有红色链接的3-结点展开。把它表示为由一个==左斜== 的红色链接连接两个2-结点，我们对上图运用这个展开，它会变成如下所示二叉搜索树:

![RB_Tree7](https://tva1.sinaimg.cn/large/008i3skNgy1gxqexldb1qj305y05ea9y.jpg)



​		我们将上面这样的树称为红黑树(只存在左红链接，到后面进阶部分我们可以构造出既存在左红链接又存在右红链接的红黑树)。

​		不难发现，这样生成的红黑树有3种特性:

​			1、红链接均为左链接。(这是因为定义左斜产生的结果)

​			2、不可能同时出现两条2条红色链接连接同一个结点，比如

![RB_Tree8](https://tva1.sinaimg.cn/large/008i3skNgy1gxqeytyuv4j302e04qq2q.jpg)

​			证明:根据定义，一个3-结点内部的2-结点连接为红链接，3-结点中2-结点只有两个，内部只可能有一个红链接，而外部-3结点和其他结点必然为黑链接，所以不可能用时出现2条红链接连接同一个点

​			3、该树是完美黑色平衡，从根结点到各个树的外部结点(叶子结点)的**路径**所经过的黑色链接的数量是一样的。

​			证明:2-3树是完美平衡的，每个叶子结点(红黑树中的外部结点)的高度相同，所以每个路径上的黑色链接数量相同。经过上面的展开后，由于展开不产生黑色链接，只产生红色链接，所以，每个路径上黑色链接数量还是相同的。

​		根据上面的特性，更近一步，我们把指向结点自己的链接(树从由上往下看，每个结点被唯一一条线指向，自己又发出两条线指向自己的两个孩子)的颜色赋予结点，这样就产生了红色结点和黑色结点，如下图为刚刚2-3树展开后的正式红黑树表示:

​                                ![RB_Tree6](https://tva1.sinaimg.cn/large/008i3skNgy1gxqeopime6j307h05m74b.jpg)



### 2 红黑树的性质:

​		<!--红黑树和二叉搜索树不同的是，它把叶子结点定义为了外部结点(N)，也是空结点。-->

​		1、每个结点是红色或者是黑色。	

​		2、根结点是黑色。

​		3、每个叶结点(外部结点)是黑色。

​		4、如果一个结点是红色，那么它的两个孩子结点必然是黑色。(原因是不存在两个红链接连接到同一个结点)。

​		5、对每个结点，从该结点到期所有的叶结点上路径上的黑结点数目相同(因为黑色链接的数量相同)。

### 3 红黑树的高度:

​		**==黑高==**:从某个结点出发到达叶结点是路上经过黑色结点的数量。我们用bh(x)表示从x结点开始到叶子结点所经过的黑色结点的数量。

​		==引理1:==(在红黑树里)一个以结点x为根的子树中至少包含了2^bh(x)^ -1个内部结点。		

​		我以两种方式证明.

​		**证明1:**由已知的黑高为bh(x),我们可以知道，这棵子树中x到每个叶子结点的高度至少为bh(x)(全为黑色结点),所以我们假设取x到每个叶子的高度为bh(x),根据性质5，我们可以知道这上面的所有节点都只能是黑色结点，这时总共有2^bh(x)^ -1个内部结点，在这里面每增加一个红色结点，或者把黑色结点换成红色结点都将增加内部结点的数量，因此得证。

​		**证明2:**归纳法，当高度为1时(只有一个根结点)，2^bh(x)^ -1=2^1^-1=1成立,假设有一个结点x，其有两个子结点，x的黑高为bh(x)，，则其两个孩子结点的黑高为至少为bh(x)-1(当x为红色结点时，黑高为bh(x),黑色时为bh(x)-1),假设孩子结点满足引理1，那么我们只要证明x满足引理1即可得证，每个孩子有内部结点2^bh(x)-1^-1，那两个孩子就是2^bh(x)^ -2，在加上x结点本身，得到结果2^bh(x)^ -1，得证。

​		==引理2:== 红黑树的高度最多为黑高的两倍。

​		**证明:**红黑树中从根结点到各个叶子结点上的每个路径上的黑色结点数量相同，所以树的高度(至少为黑高)，由于不存在连续的两个红色结点，所以要想使一个路径上结点数量最多(路径上的每多个结点都会使树的高度增加1)，在每一个黑色结点后都多连一个红色的结点。这样既满足了红黑树的性质，又使得树高增加了一倍，即为2倍的黑高。

​		==引理3:==红黑树的高度最多为2log~2~^n+1^ 

​		**证明:**由引理1可以得2^bh(root)^ -1 $\le$n，n为内部结点的数量。因此可以推出log~2~^n+1^ $\ge$ bh(root)，由引理2，可以得出2 * bh(root)$\ge$ h,(h为树高)，两者联立，推出2log~2~^n+1^ $\ge$ h，得证。

## 三、红黑树的基本操作

以下代码都将用c++语言实现，不过将不会用到c++的一些不同于c的特性，最多只涉及类以及构造函数、析构函数的概念，只会c也是能看懂的。

### 1 红黑树结点的定义

以下是红黑树结点的定义:

==实现代码:==

```c++
struct TreeNode
{
    TreeNode():color(true),leftChild(nullptr),rightChild(nullptr){}//默认构造函数
    TreeNode(const int x):color(true),value(x),leftChild(nullptr),rightChild(nullptr){}//接收关键字值的构造函数
    bool color;//结点的颜色红色为true，黑色为false
    int value;//关键字
    TreeNode* leftChild;//该结点的左孩子
    TreeNode* rightChild;//该结点有孩子·
};
```



### 2 红黑树的查找

​		红黑树的查找和二叉搜索树的查找是一样的操作,详情请见二叉搜索树。

==实现代码:==

```c++
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
```

==时间复杂度:== 最坏情况下，红黑树搜索一个关键字需要2log~2~^n+1^ (树高)次操作，因此为O(2log~2~^n+1^)=O($\lg n$).

### 3 红黑树的旋转

​		本质上AVL树中的LL和RR，具体请参考AVL树，==红黑树中的旋转要注意旋转的两结点的颜色也要交换==(后面不会强调这个，所以请好好记住)，不再赘述。这里只给出代码。

==实现代码:==

```c++
TreeNode* RBTree::leftRotation(TreeNode* node)
{
    TreeNode* rightNode = node->rightChild;//获取右子结点
    bool tempColor = node->color;//保存自己的结点颜色
    node->color = rightNode->color;//下面两行为两结点互相交换颜色
    rightNode->color = tempColor;
    node->rightChild = rightNode->leftChild;//将右子结点的左孩子给node
    rightNode->leftChild = node;//将右子结点的左孩子设为node
    return rightNode;//原来的右子结点
}
TreeNode* RBTree::rightRotation(TreeNode* node)
{
    //和左旋转是镜像的
    TreeNode* leftNode = node->leftChild;
    bool tempColor = node->color;
    node->color = leftNode->color;
    leftNode->color = tempColor;
    node->leftChild = leftNode->rightChild;
    leftNode->rightChild = node;
    return leftNode;//返回原来的左子结点
}
```

==时间复杂度:==O(1)。

### 4 红黑树的插入

​		红黑树插入的前半部分和二叉搜索树是一样的，每经过一个结点，就跟这个结点进行比较，如比当前结点的关键字小，则下降到左侧结点，反之下降到右侧结点。最后在一个空的叶子结点处插入关键字。但要注意的是，插入的结点颜色必须为红色(也就是用红链接连接)，因为如果插入黑色，将违反红黑树的性质5，将会有一条路径上的黑色结点数量比其他路径多1。在这个要求之后，我们需要考虑的就是不要违反连续两个红色结点相连。我们将在下面讨论。

#### 4.1 结合2-3树讨论红黑树的插入

​		这里将逐层深入通过2-3树来理解红黑树的插入,首先说明一下2-3树中的2-结点和3-结点分别在红黑树中的表示。

==2-结点:==

![RB_Tree21](https://tva1.sinaimg.cn/large/008i3skNgy1gxzifp7y1hj304902bq2r.jpg)

==3-结点:==

​                                                                                	![RB_Tree22](https://tva1.sinaimg.cn/large/008i3skNgy1gxzihxpvyfj304902xq2s.jpg)  

##### 4.1.1 向单个(整个树中只有一个结点)2-结点插入新关键字

​		当插入的关键字比2-结点(2-结点只有一个关键字)小时，红色链接出现在2-结点左侧，为左红链接，满足最开始定义的红链接局势左链接的性质。如果比其大，则为右链接连接，我们通过左旋转将其转变为左红链接。

​		==示例:==

![RB_Tree9](https://tva1.sinaimg.cn/large/008i3skNgy1gxr18hxdtoj30al0eut8z.jpg)

##### 		4.1.2 向树的底部的2-结点插入新的关键字

​				和上面的操作是一样的这里不再赘述。

​		==示例:==

![RB_tree10](https://tva1.sinaimg.cn/large/008i3skNgy1gxr1jc3yjej30cs0n5q3o.jpg)

##### 4.1.3 向单个3-结点插入(整个树中只有这一个3-结点)新的关键字

​		这理需要分三种情况讨论:

​		a、新插入的关键字比3-结点中的两个关键字都大，这时新关键字插入在根结点右侧，且为红链接，违反了左红链接性质，但由于根结点的左链接也是红链接(2-3树中3-结点内部两个关键字的链接为红链接)，我们可以直接将这两个红色链接转换成黑色链接，同样满足了红黑树的性质，转换成3个2-结点，这就好像是我们分裂了3-结点(2-3树中提到)这两者的操作是等价的，在分裂中我们是把==中间关键字==上提，但在这种情况中，中间关键字在红黑树中本来就在上一层，无需额外操作，由于分裂后2-3树是平衡的，所以可以验证，这个操作后这棵树也是平衡的，如下面示例中的最左侧图。

​		b、如果新插入的关键字小于原来两个关键字，那么将会出现两个连续的红色链接，我们可以通过右旋转将其转换成上述的情况a，然后执行同样的操作，如下面示例的中间图，中间关键字为25，通过右旋上提，相当于分裂中的上提。

​		c、如果新插入的关键字的值在3-结点两个关键字之间，那么它将被插入较小键的左侧子结点，这时我们需要先通过左旋转，转化为上述b再通过右旋转，转化为上述a，然后执行同样的操作，如下面示例的最右侧张图。

==示例:==

​                                                     ![RB_Tree13](https://tva1.sinaimg.cn/large/008i3skNgy1gxraxtc0glj308402odfq.jpg)

​				![RB_Tree11](https://tva1.sinaimg.cn/large/008i3skNgy1gxr4luupwqj30s90lugnp.jpg)

##### 4.1.4 向树底部的3-结点插入新的关键字

​		这里面的操作和在单个3-结点中相同，但有一点要注意，就是当把两个红链接转化为黑链接时，中间结点(三个关键字中关键字大小排在中间的那一个)的颜色可以为红色或者黑色，但由于要满足红黑树的性质5，每个路径上的黑色结点数量相同，所以其必须为红色（除了根结点外），这相当于把红色链接往上移了因为(中间结点为红色，指向它的链接必为红链接),向上传递红链接就像在2-3树中把其中间的关键字往父结点传递，而分裂成的两个2-结点正好对应于两个颜色变黑的子结点。这时候如果父结点是个2-结点，直接插入即可，即对应在红黑树中的黑色结点(也就是说中间结点的父亲结点为黑色结点，这满足红黑树的性质)，这时调节就结束了。但如果父结点为父结点为3-结点，则应该继续分裂，这就对应到了红黑树中红链接传递上去之后，发现中间结点的父结点仍然是红结点，所以我们应当继续执行等价于2-3树分裂的操作(之前讨论的三种情况)，直到遇到黑色结点，这个分裂上升的过程必然会结束，因为根结点是黑色结点。我们可以清楚的看到从2-3树理解红黑树插入，是非常轻松的。

==示例:==

​		以下的示例同样也拿对应的2-3树作为参照

![RB_Tree12](https://tva1.sinaimg.cn/large/008i3skNgy1gxy7kqk4kzj30u011hq7t.jpg)

#### 4.2 正式实现第一种红黑树的插入算法(只存在左红链接的红黑树):

​		有了以上的铺垫，我们可以看出红黑树的插入其实非常简单，我们将实现递归版本的红黑树插入。

==基本步骤:==

​		a、最开始和二叉搜索树的插入操作一样，然后找到待插入的叶子结点的父结点。

​		b、判断这个父结点的颜色，如果为红色，说明这是个3-结点中的一个，(另外一个为这个红色结点的父结点)，否则为2-结点。

​		c、根据之前的介绍对2-结点和3-结点插入执行不同的操作即可。

​		但实现起来可以更加灵活一点，不用参照上面的步骤，可以观察到插入进2-结点情况2的操作和插入进3-结点情况3的第一步操作是一样的插入进3-结点情况1和情况2的操作都包含在情况3的第二步和第三步中。

==实现代码:==

首先介绍其中的部件函数颜色转换:

```c++
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
```

递归版本:

```c++
void RBTree::recursiveInsert(const int & value)
{
    _sentinelNode->leftNode = _root = auxRecursiveInsert(_root, value);//_sentinelNode为哨兵结点，不好关键字，但其左子结点永远为根结点
    _root->color = false;
}
TreeNode* RBTree::auxRecursiveInsert(TreeNode* node,const int & value)
{
    //首先寻找待插入位置
    if(node == nullptr)//为空结点，说明找到了代插入的位置,构造新节点并直接返回
    {
        node = new TreeNode(value);
        ++_treeSize;
        return node;
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
    //接下来更具情况维持树的平衡
    if(isRed(node->rightChild))//如果其右结点为红色，需要转换成左红链接
    {
        //这里对应于插入2-结点的情况2，或者插入-3结点情况3的第一步
        node = leftRotation(node);
    }
    if(isRed(node->leftChild) && isRed(node->leftChild->leftChild))//如果该结点的左子结点以及左子结点的左子结点都为红色，右旋转
    {
        //这里对应于插入3-结点情况3的第二步、或者插入-3结点情况2的第一步
        node = rightRotation(node);
    }   
    if(isRed(node->leftChild) && isRed(node->rightChild))//如果左右两个孩子结点都是红色，则进行颜色转换
    {
        //这里对应于插入3-结点的情况3的最后一步，或者插入3-结点的情况2的最后一步，或者插入-3结点情况1的第一步
        flipColor(node);
    }
    return node;//返回这个结点
}
```

==时间复杂度:==找到对应插入的位置和回溯所需要的操作和树的高度有关，因此为O($2\log_2^{n+1}$)=O($\lg{n}$),然后中间的颜色交换和旋转都为O(1)，所以最终时间复杂度为O($1*\lg{n}$)=O($\lg{n}$).

#### 4.3 进阶:结合2-3-4树讨论红黑树的插入

##### 4.3.1 允许一个结点连接子结点的两个链接都为红色的红黑树(一个小热身)

​		之前我们讨论了结合2-3树和红黑树的关系，但其实我们也可以用2-3-4树来讨论的红黑树的插入,2-3树只允许红黑树中的一个结点左链接为红色，但事实上，一个黑色结点的左右子结点可以同时为红色(此时这三个结点组成4-结点)。我们知道2-3-4树为t=2时的B树，B树的插入是单程向下的，在寻找待插入的位置时，遇到一个关键字满的结点就会执行分解操作，2-3-4树也是一样，遇到4-结点直接分裂为两个子结点，然后将中间关键字上移到父结点中。这样做最终会保证插入在叶结点时，叶结点肯定不是满的。

​		从2-3-4树的插入到启发，我们也可以用红黑树实现对应的版本，这样的话我们的红黑树中就允许一个黑色结点下有两个红色孩子结点，且即允许了右红链接的存在。我们做如下的等效转换.

​		  (1) 将4-结点看成红黑树中的一个黑结点用两个红色链接连接两个红结点组成的结点。

​		  (2) 在寻找插入位置的过程中模拟分解4-结点,等效为对黑色结点和两个孩子为红色结点进行颜色转换，将(1)中的黑变成红，红变成黑。其实这就相当于分裂成两个2-结点，因为2-结点是黑色。

​		  (3)插入结点后递归向上的过程中只用旋转进行调平。

​		大家可能对(3)有点疑惑，下面将用图例解释。我们知道，4-结点分解后其中一个关键字会插入到父结点，父结点可能是2-结点或者3-结点(注意我们在寻找过程中遇到4-结点就分裂的，所以不可能分裂后的中间关键字插入到的父结点为4-结点)，2-结点在红黑树中为黑色结点，3-结点为一个黑色结点带着一个左红链接的红色结点，我们在红黑树中对应的2-3-4树中的分裂操作是颜色转换，两个孩子结点变黑，父结点(即为4-结点的中间关键字)变红。如果待分裂的4-结点的父结点为-2结点，那是没事的，因为中间关键字颜色变为红不管插入到2-结点(单个黑色结点)左边还是右边(注意现在允许了右红链接存在)，都不破坏红黑树的性质。但是，如果分裂4-结点的父结点是3-结点就有问题了，如果4-结点中的被提到其父结点中的中间关键字的大小不是在父结点中唯二两个关键字大小之间(理论上这种情况对应到红黑树必然存在)，将出现如下情况:

==示例:==

![RB_Tree14](https://tva1.sinaimg.cn/large/008i3skNgy1gxx8apdkuzj30c70ryq4b.jpg)

​		

​		上面就是对(3)的示例，这是树中的一部分，首先在我们寻找待插入位置的时候，下降到10时，检查发现两个子结点都为红色(4-结点)，进行上述(2)的颜色转换，等效于分裂成2个(2-结点)，最后插入11时，必然是在非满的结点插入，然后向上回溯到20时，发现其左子结点以及左子结点的左子结点都为红色，通过右旋转调平。当然也有镜像的情况，这时候左旋转即可，不再给出图片。

​		所以在我们根据2-3-4树的插入转换到红黑树时，在回溯时也包含了对2-3树的插入情况，也就是对3-结点的插入，不同于2-3树转换到红黑树的插入的是，我们提前进行颜色转换，也就是说在回溯过程中插入到3-结点的所有情况的最后一步，即颜色转换都是不可能会出现的(如果忘了，根据目录跳转到向单个3-结点插入新的关键字那里，总共3种情况，情况a不可能出现，情况a也为其他情况的最后一步操作)。因此我们只需要在原来的根据2-3树转换为红黑树的插入代码上稍作修改即可。

==示例:==

```c++
void RBTree::recursiveInsert(const int & value)
{
    _sentinelNode->leftNode = _root = auxRecursiveInsert(_root, value);
    _root->color = false;
}
TreeNode* RBTree::auxInsert(TreeNode* node,const int & value)
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
    //接下来根具情况维持树的平衡
    if(isRed(node->rightChild))//如果其右结点为红色，需要转换成左红链接
    {
        //这里对应于插入2-结点的情况2，或者插入-3结点情况3的第一步
        node = leftRotation2(node);
    }
    if(isRed(node->leftChild) && isRed(node->leftChild->leftChild))//如果该结点的左子结点以及左子结点的左子结点都为红色，右旋转
    {
        //这里对应于插入3-结点情况3的第二步、或者插入-3结点情况2的第一步
        node = rightRotation2(node);
    }   
    return node;//返回这个结点
}
```

​		可以看到和前面的唯一区别是我们把最后一个颜色转换的if放到了递归调用的前面。在多线程同时访问一棵树的情况下，这个算法相较于之前的要快，因为每一步操作只涉及两个链接。而之前的算法可能需要涉及最多四个链接，原因是进入第三种情况的时候最后3个if都要执行，这将涉及到该结点连接两个子结点的链接以及左子结点的左链接和右链接，而在多线程情况下1个链接只能被一个线程访问否则会出现竞争条件(具体参考操作系统相关书籍)。因此，单个线程单个操作访问到的链接越少，能在同一棵树种运行的线程就越多，程序速度就越快。但这里将结合2-3-4树看红黑树插入并不是为了说明其在多线程下的优势，只是为了下面做铺垫。我们可以发现这种红黑树虽然支持一个黑色结点有两个红色孩子结点，但是它还是并不允许单的红色右链接出现，也就是黑色结点的右子结点为红色，但左子结点不为红色。我们将在下面解决这个问题

==时间复杂度:==同上为O($\lg{n}$)。

##### 4.3.2 允许左、右红链接的红黑树(最终版)

​		现在，让我们重新定义3-结点，之前我们定义的3-结点由一个黑色结点连接和一个由黑色结点左红链接连接的红结点组成，现在我们将这个定义放宽，左红链接和右红链接都可以。

​		我们知道2-3-4树是的插入和B树一样都是单程向下的，不会有上一小节中的红黑树需要回溯的情况。具体原因出在哪里呢？我们观察上面一节给出的图例，第二步颜色转换出现了两个连续的红色结点，我们其实是向一个3-结点(15、20组成)中添加这个2-结点(10)，它应该变成一个4-结点，一个4-结点的正确表示应该是向图例中最后一张图表示的那样(也就是回溯完以后)。所以我们发现造成这样的原因，其实是我们只规定了左红链接的存在，所以15必须作为20的左子红色结点存在，而不能是20作为15的右子红色结点存在。但是现在我们允许了右红链接的存在，因此，我们就有可能遇到后一种情况了，这时候，不用回溯也是正确的。如下图所示:

==示例:==

![RB_Tree16](https://tva1.sinaimg.cn/large/008i3skNgy1gxya4r9h2aj30970dz3yx.jpg)



###### 下降过程中的两种调整

​		我们可以看到，这时候插入完后红黑树就是正确的，无需回溯。大家可能会有疑惑，只是说20可能是15的右子红结点呢？虽然支持了右红链接，但并不代表下降到这里的时候20一定就是15的右子红结点，也有可能回到上面的情况，就是15是20的左红子结点，因为左红链接也是允许存在的。其实也不是很难，我们将做部分调整，先看示例，假设插入关键字大小为4。

==示例1:==

<img src="https://tva1.sinaimg.cn/large/008i3skNgy1gxzaz7qio7j30sb17ogpy.jpg" alt="RB_Tree18" style="zoom:67%;" />



​		上面这个示例展示了这种情况的处理。我们首先处于30这个结点时，检查其将要下降的子结点10，我们发现这个结点为红色结点，我们标记这个当前所在的结点为爷爷结点(用一个变量保存),然后继续向下下降。当下降到10时，10为父结点，发现下一个下降的结点为黑色6，直接下降到6，然后在6时检查其两个子结点的发现子结点的颜色都为红色，说明6、3、8这三个结点共同组成4-结点，我们和上面一样进行颜色转换。然后，不同的是，我们检查其父结点，看其是否为红色，如果是，我们发现两个红色结点都是其父结点的左子结点，所以直接对刚刚用变量保存的爷爷结点进行右旋转，这样就保证了红黑树的性质，无需因为调整而回溯，并且和2-3-4树最终的结果是等价的。这种情况有一种镜像的情况，这时只要对爷爷结点左旋转即可，不再介绍。下面我们将介绍下降过程中的另一种情况，这时候需要双旋转才能调平，请先看示例。

==示例2:==

<img src="https://tva1.sinaimg.cn/large/008i3skNgy1gxzce2gz57j30r0163790.jpg" alt="RB_Tree19" style="zoom:67%;" />

​		以上就是示例，我们可以看到，不同于上面的情况就是我们在下降到这个由30和10组成的3-结点时，待插入的值是在10和30之间(上一种比10和30都要小),因此，我们下降到10的右子结点处，然后调整其两个子结点颜色，由于15的父结点为10为红色结点，我们需要旋转，我们知道在2-3-4树中分裂结点的**中间关键字**提升到其父结点时要确认好其所在的位置。现在在对应的红黑树中，15就是待上升的**中间关键字**，也就是它需要上升到10和30的中间，所以我们先对父结点实施左旋转，这时候我们在红黑树上看,15就被10和30"夹在了中间"了，然后在爷爷结点执行右旋转(这是因为我们规定的4-结点是一个黑色结点和其两个红色子结点组成)，然后作中获得了符合规定的红黑树，同样，插入完后无需回溯。另外一种镜像的情况不在介绍。

​		其实都过上面两个案例，不难发现。2-3-4树中下降对4-结点的分裂和红黑树的下降过程是一样的。可以看到，将4-分裂成两个2-结点相当于颜色转换将两个孩子结点变黑，然后**中间关键字**上升并且在其父结点中找到对应位置，相当于红黑树中4-结点中的黑色结点变红，然后和其父结点以及爷爷结点进行旋转操作，相当于2-3-4树中结点的中间关键字上升到父结点，并且在其中找到合适的位置。也有时候不用旋转，那就是其父结点为2-结点(此时为黑色)的时候。

​		事实上还没有结束，虽然在上面不断分裂4-结点，并且进行调整，到最后我们插入到的结点只可能是2-结点或者3-结点，但插入3-结点还是有一点变术。但如果理解了这一小节，那下面也没问题。我们在下一霞姐中介绍如何处理往最终3-结点插入关键字的情况。

###### 处理最终插入结点为3-结点的情况

​		在上面我们下降过程中的调整保证了最后插入到的结点只可能为2-或者3-结点，但插入3-结点仍然会有问题，如下所示:

==示例:==

![RB_Tree20](https://tva1.sinaimg.cn/large/008i3skNgy1gxzdjoo291j30a90510ss.jpg)

​		其实和上面调整一样解决就好了，执行对应的旋转，插入3-结点，我们通过旋转，找到其对应的位置，就和在2-3-4树中，通过遍历结点中的关键字来找到其对应的插入位置一样。比如上面第二张图，先对10右旋，再对8左旋就行了。

#### 4.4 进阶:正式实现第二种红黑树插入算法(自顶向下,允许左红链接、右红链接的红黑树)

​		有了前面的大量铺垫，我们将正式实现单程向下的红黑树插入算法。注意之前所有的插入算法生成的也都是红黑树，它们并不是错误的,只不过它们不允许单独的右红链接存在。我们将新的关键字作为红色结点插入，先说明一下命名:

==命名:==

​		**targetNode**: 当前所在的结点。

​		**preNode**: 当前所在结点的前一个结点。

​		**flag**: 一个标记，用于表明targetNode是preNode的左子结点还是右子结点。

​		**_sentinelNode**: 哨兵结点，根结点的前一个结点，其没有关键字，其左子结点永远为根结点，右子结点为空。

​		**grandParentNode**: 用于临时保存爷爷结点的结点。

​		**preGrandParentNode**: 用于临时保存爷爷结点的前一个结点的结点。

​		**mark1**：用于标记grandParentNode是preGrandParentNode的左子结点还是右子结点。

​		**mark2**: 用于标记将要下降子结点是grandParentNode的左子结点还是右子结点。

​		接下来讲述具体步骤。

==具体步骤:==

​		(1) 初始化targetNode为_root，preNode为 _sentinelNode，flag为左子结点标记，然后自顶向下地下降，途中不断更新flag，targetNode，preNode。如果将要下降到的子结点结点为黑色，直接下降到这个结点即可，无需任何操作。

​		(2) 如果当前要下降到的子结点为红色结点，用grandParentNode保存当前targetNode的值，preGrandParentNode保存preNode的值,mark1保存当前flag的值，然后根据要下降的位置，设置mark2,然后继续下降。

​		(3) 如果将要下降到的子结点为红色，且其兄弟结点也为红色，说明其为4-结点，执行颜色转换，当前所在结点变红，然后将要下降到的子结点和其兄弟结点变黑，然后检查preNode的颜色，如果为红色执行以下操作。

​		a. 如果mark2和flag为相同的，说明父结点的孩子结点都是其左子结点或右子结点，(根据mark2)对grandParentNode执行单旋转即可,然后得重新根据mark2调整preGrandParentNode，因为旋转后其孩子结点不在是grandParentNode。

​		b. 如果mark2和flag不相同，则需要执行双旋转，先对preNode执行旋转，在对grandParentNode执行旋转,同时跟a一样调整指针指向。

​       (4) 重复以上步骤，直到下降到待插入位置，这时直接插入，然后再判断preNode的颜色，执行(3)a或者(3)b，结束。

​		由于我们做的所有操作，在2-3-4树中都是等价的，2-3-4树是一颗完美平衡的树，所以执行删除后的红黑树仍然是完美黑色平衡的。

==实现代码:==

```c++
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
```

==时间复杂度:==同上为O($\lg{n}$)。

​		可以看到的是当我们允许右红链接的时候，情况明显会变复杂很多，代码也稍显臃肿(其实也还好，只是大括号单独占用的行比较多，嘿嘿)。

#### 4.5 进阶:正式实现第三种红黑树插入算法(自底向上)

​		这种自底向上的红黑树插入算法实际上只是4.3.1那里的小热身上的改进，让其支持单独右红链接即可，这里直接给出代码,具体自己思考，可以根据自顶向下反向思考(我懒，不想码字了)。

==实现代码:==

```c++
void RBTree::recursiveInsert(const int & value)
{
    _sentinelNode->leftChild = _root = auxRecursiveInsert(_root, value);
    _root->color = false;
}
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
```

==时间复杂度:==同上为O($\lg{n}$)。

​			自底向上的的代码会更简单，但函数递归调用会造成对很多参数压栈的额外开销。

### 5 进阶:红黑树的删除

​		通过以上我们可以看出2-3-4树中与红黑树的对应是2-结点为单个黑色结点，3-结点为黑色结点加上一个左红链接连接或者右红链接链接的红子结点，4-结点为一个黑色结点加上两个红色链接连接的红色结点。把这个熟记于心，接下来开始讲解删除。

​		现在我们假设，在下降找到待删除结点的位置后，我们统一寻找它的==后继==结点（即右子树的最小结点）的关键字作为其替代关键字，然后只需要删除后继节点即可                             。

#### 5.1 结合2-3-4树讨论红黑树的删除

​		首先，我们知道删除为2-结点的叶子结点会导致2-3-4树不平衡，所以应该**避免删除为2-结点的叶子结点**。我们在2-3-4树中的做法是，我们在寻找待删除的结点时，检查将要下降的结点，如果该结点为2-结点。我们有两种方案，一种从兄弟结点借，一种是合并(详情参考B树的删除，我假设你把该文章开头要求的所需要的前置知识都掌握了)，这样保证我们下降到叶子结点时，其必不可能是2-结点。所以我们将使用红黑树效仿这种做法。

==情况讨论:==

​		(1)下降到3-或者4-结点时直接下降即可。

​		(2)如果下降到的结点为2-结点，需要从兄弟结点借或者合并，方法是检查兄弟结点的左子结点是否为红色(因为3-结点和4-结点的共同特点是左子结点都为红色)，如果为红色则向兄弟结点借，否则合并。

​		a. 如果兄弟结点为2-结点，其必为黑色结点，我们将待下降到的结点和兄弟结点以及父结点中的对应的结点合并成4-结点。具体对应	红黑树的做法是将两个黑色子结点变红，让后父结点变为黑即可,我们将会在下面演示。

​		b. 如果兄弟结点不为2-结点，则通过左旋转或者右旋转从兄弟结点处获得一个结点变为3-结点，我们将会在下面演示。

##### 5.1.1 合并在红黑树中的等价操作

==示例:==

![RB_Tree15](https://tva1.sinaimg.cn/large/008i3skNgy1gxy6x44q7vj30fw09ogm7.jpg)



​		从上面的示例中我们可以看到合并在红黑树中只需要将待操作结点的两个子结点变红，然后自己由红变为黑(这个过程相当于离开了自己原来所在的4-或者3-结点)，这将会生成一个新的4-结点。

##### 5.1.2 从兄弟结点借在红黑树中的等价操作

​		从兄弟结点借就会复杂很多，甚至可能出现这种情况:在2-3-4树中2-结点的兄弟结点，但在红黑树中却不是其兄弟结点(原因后面会说)。我们将分两大类情况讨论。

###### 2-3-4树中对应结点的兄弟结点就是红黑树中对应结点的兄弟结点

​		这种情况较为简单，我们只需要仿照2-3-4树中从兄弟结点借结点的方法，在红黑树中对应实现即可，先看看示例1:

==示例1:==

![RB_Tree23](https://tva1.sinaimg.cn/large/008i3skNgy1gxzlz94pn7j30i00f5mya.jpg)



​		这里，我们将要下降到4，它为2-结点，然后其兄弟结点正好为(15和16组成，这在2-3-4树中也是对应的兄弟结点)3-结点，我们可以向其借结点，2-3-4树中通过旋转完成，它将3-结点中的最小关键字上提到父结点，然后父结点对应关键字下移到2-结点。我们在红黑树中也是等价的操作，左旋转，将15这个最小关键字上移，然后3-父结点中的7下移至原来的2-结点。但这里要注意颜色，4得变成红色得以和7组成3-结点，然后16必须变为黑色得以使之成为2-结点。经过这般转换后，明显感受到，和2-3-4树是等价的。

​		再看看示例2:

==示例2:==

​                                       ![RB_Tree24](https://tva1.sinaimg.cn/large/008i3skNgy1gxzmvljve8j30ij0j5dhi.jpg)                             



​		这里，我们仍然是要下降到这个2-结点(4)，兄弟结点仍然为3-结点，不同点在于，我们的要提取的最小关键字，在3-结点中的黑色结点的左子结点。我们只需要一步右旋转将这个最小关键字结点(12)变为黑色结点就能转换到上面那种情况了。最后发现也是等价的。这里说明一下，如果兄弟结点为4-结点，那么所做的操作和这里是一样的。因为我们要和2-3-4树中一样提取其最小关键字，最小关键字在4-结点中的黑色结点的左子结点,本质上也是一样的,这里就不在单独拿出来讲了，大家都是聪明人。同样的，虽然我们这里举例子用的父结点都是3-结点(比如上面两例中为7和30组成的3-结点),但父结点为4-结点本质也是一样的。

​		同样的，上面两种状况有其镜像的情况，这里就不举例了，下面将讨论一个较为复杂的情况。

###### 2-3-4树中对应结点的兄弟结点不是红黑树中对应结点的兄弟结点

​		这里直接先说，可能讲不明白，但大家一看示例估计就一目了然了，所以先看看示例吧。

==示例:==	

<img src="https://tva1.sinaimg.cn/large/008i3skNgy1gxzotmzj3dj30om125ae5.jpg" alt="RB_Tree25" style="zoom:67%;" />

​		

​		

​		这里我们将会下降到40这个2-结点，但我们发现其兄弟结点为红色，其真正的兄弟结点应该在其当前兄弟结点的右子结点。这时候，我们只需要进行右旋转，那么由15、12、20这三个结点组成的兄弟结点就会成为40的兄弟结点了，之后的流程和之前的状况是一样的，这里不在赘述。最后生成的红黑树仍然适合2-3-4树是等价的。

​		这种情况只会发生在父结点时3-结点的情况，原因是红黑树是二叉树，所以3-结点中间的链接只能成为其左右链接中的一个链接的兄弟。另一个就只能被迫和红色结点做兄弟了。还有另一种情况是镜像的，这里不在赘述，**<u>注意，也有可能真正的兄弟结点为2-结点，这时候执行合并(之前漏了，哈哈，其实应该在合并那里讲的)，这里我就不讲了</u>**

##### 5.1.3 最后在3-结点或者4-结点的删除操作

​		现在这个就简单了，由于我们在下降过程中保证了不会出现2-结点，所以最后的删除肯定是在3-结点或者4-结点中，我们之前假定了，在找到删除的结点后我们找到它的==后继==结点的关键字代替，然后继续下降删除后继结点。所以如果最终删除的后继点在4-结点中，我们直接删除，因为==后继==结点必然是4-结点中黑色结点的左子红色结点(如果不明白请去二叉搜索树中了解后继节点的概念)，这种情况最简单。但是3-结点会有点不同，如果是拥有左红链接的3-结点，那也是和之前4-结点一样直接删除。但是拥有右红链接的3-结点就不能直接删除了，因为此时的==后继==节点恰好为黑色结点,我们只需要交换3-节点中两个结点的值，然后删除红色子结点就好了，如下所示。

==示例:==

![RB_Tree26](https://tva1.sinaimg.cn/large/008i3skNgy1gxzrew61lhj305c09h3yj.jpg)

##### 5.1.4 一个应该考虑的特殊情况

​		这里讨论一下特殊情况，就是当我们在找到待删除关键字所在特殊结点后，我们需要寻找其==后继==，寻找其==后继==需要先往右下降到右子结点，然后不断朝左下降。但考虑这样一种情况，我们在上述右下降的右子结点刚好为2-结点，这时候如果兄弟结点为3-结点或4-结点或红结点，都将发生右旋转。但这个过程中，我们待删除关键字所在的特殊结点也下降了。原来找到的结点就不是待删除关键字所在的结点了，如下所示:

==示例:==

![RB_Tree27](https://tva1.sinaimg.cn/large/008i3skNgy1gy0sb1sn5xj309u0aimxc.jpg)

​		如这张图所示，如果我们一找到待删除关键字就直接去寻找其后继是不对的，因为当待删除就关键字所在结点为2-时。需找后继，由于往右下降发生旋转而导致待删除关键字所在结点下降，我们就不是从其右子结点一直朝左下降寻找关键字了，而是从其自身一直朝左下降寻找关键字了，所以这有在找到待删除关键字所在的结点并且其右子结点不为2-结点，我们才可以开始寻找起后继。==不过，我们可以通过一些编程技巧避免出现这种情况==，我们将寻找后继的情况也并到了前面的寻找待删除关键字的代码中，详情请看后面的代码。

#### 5.2 进阶:正式实现红黑树的第一种删除算法(自顶向下)

​		有了以上的铺垫，以及2-3-4树，实现红黑树的删除算法就简单了很多，红黑树的删除算法会比插入情况考虑的更多，虽然说这一节篇幅并不多，但这也归功于插入时做的层层铺垫。说句实在话，这样讲解我认为比直接硬生生的讲插入的各种情况，删除的各种情况会好很多。正所谓知其所以然。

==基本步骤:==

​		从根结点开始下降,执行以下步骤:

​		(1) 如果结点的关键字不等于待删除关键字，检查其将要下降的孩子结点，如果不为空其为3-结点或者4-结点，直接下降。

​		(2) 如果当前结点的关键字不等于待删除的关键字，其将要下降到的孩子结点为2-结点，检查其兄弟结点。

​				a. 兄弟结点为黑色，且为3-或者4-结点，根据5.1.2中的第一种状况执行对应旋转。

​				b. 兄弟结点为黑色，但其为2-结点，执行5.1.1的合并操作。

​				c. 兄弟结点为红色，进入5.1.2第二种状况，这是后我们需要根据情况进行旋转，然后转换到(2)a或者(2)b。

​		(3) 如果当前结点点的关键字等于待删除的关键字，考虑5.1.4，如果满足，我们用一个临时结点保存该结点。然后重复(1)(2)的步骤寻找其==后继==(如果其右子树不为空，右子树为空时，直接删除当前结点，然后将其左子结点接上，此时已经平衡)，否则继续之前的操作。

​		(4) 找到==后继==结点后，我们将后继结点关键字的值赋予刚刚保存的结点。检查后继结点是否为红色结点，如果不是，那么它必然存在右红子结点(因为下降到最后只可能出现3-或者4-结点，同时我们是寻找==后继==)。执行5.1.3的情况。

​		由于我们做的所有操作，在2-3-4树中都是等价的，2-3-4树是一颗完美平衡的树，所以执行删除后的红黑树仍然是完美黑色平衡的。

​		下面先给出辅助函数的代码。

==实现代码:==

​		首先介绍颜色转换的第二个版本(区别于插入时的颜色转换):

```c++
void RBTree::flipColor2(TreeNode* node)
{
    node->leftChild->color = node->rightChild->color = true;
    node->color = false;
}
```

​		其次是一个判断是否为2-结点的函数:

```c++
bool RBTree::is2_Node(TreeNode* node)
{
    if(node == nullptr)//空结点不为2-结点
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
```

​		介绍完了辅助函数，下面正式给出删除函数的代码。

==实现代码:==

```c++
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
```

​		这里先解释下tempNode的作用，如下图:

==示例:==

![RB_tree28](https://tva1.sinaimg.cn/large/008i3skNgy1gy0tqvl69wj30jo0uz77k.jpg)

​		

​		这是之前的图，但要注意蓝色箭头标记，其实tempNode是用来应对表面兄弟的情况的，当不存在这种情况是，它和targetNode相等,它主要作用在于targetNode由于表面兄弟旋转而下降后,继承targetNode原来的位置，以便调整targetNode原来父亲结点preNode的指向孩子指针，还有就是调整自己的孩子指针指向新的targetNode。

​		这里面我们把寻找后继结点的步骤也放在while大循环里了，我们需要在发现目标结点的值时判断其右子结点是否存在,如果不存在说明无法寻找后继了，那执行对应操作删除当前结点。然后如果存在的话我们保存当前结点，进入下一个if，由于下面的if判断时和value比大小，当前targetNode->value恰好等于value，所以进入targetNode->value <= value的情况，刚好下降到右子结点，之后也正好不停地随着左子结点下降，然后在当前结点的左子结点为空并且storedNode不为空时，删除后继结点。由于删除情况很多，所以代码略显臃肿，但此种方法无需回溯。

==时间复杂度:==和插入算法一样为O($\lg{n}$)。

#### 5.3 进阶:实现第二种红黑树删除算法(自底向上)

​		自底向上的算法我们需要采用逆向思维就好。我们在寻找过程中不做任何调整，先找到待删除的关键字，然后仍然不做任何调整下降到后继结点,注意这个过程都是递归下降的，看情况删除后继结点(后继结点也有可能就是含有待删除关键字结点本身)，回溯时候再进行调整。这里的主要麻烦在于删除2-结点后的回溯调整，由于其并不是那么直接。这些情况将有下面介绍，

##### 5.3.1 被删除后继结点的兄弟结点为3-结点或4-结点

​		这种情况我们通过旋转，像兄弟结点借结点，如下所示:

==示例:==

​		                              ![RB_Tree29](https://tva1.sinaimg.cn/large/008i3skNgy1gy1ho3u82kj30h30ir0u6.jpg)



​		这种情况和5.1.2中第一种状况的差不多，区别在于之前5.1.2中是下降到2-结点做出旋转，防止删除20结点，而这里是给空结点补充结点。上面的示例介绍的是双旋转的情况，还有中双单转的情况和5.1.2中第一种状况是一样的，以上面为例，当7的右子结点为15然后其只有一个右子结点20时，执行单个左旋转。还有就是镜像的情况不在赘述。

##### 5.3.2 被删除后继结点的兄弟结点为2-结点

​		这里可以再细分为两种情况，如下:

###### 父结点为3-或者4-结点

​		这种情况较为简单，我们可以将兄弟结点，和父结点的对应关键字合并，然后就一节调平了。

==示例:==                                       		

​                                      ![RB_Tree30](https://tva1.sinaimg.cn/large/008i3skNgy1gy1idzwuzpj30e70d8t9c.jpg)   7和15颜色交换



​		可以看到上面这种情况相当从父结点借了个关键字和兄弟结点组成3-结点，然后父结点正好损失了一个孩子结点指针(指向被删除2-结点),以及一个关键字，满足2-3-4树的性质。等价的红黑树操作只需要将兄弟结点和父亲结点(3-或4-结点)中的红色结点交换颜色就行。

###### 父结点为2-结点

​		这时候较为复杂，父结点和兄弟结点都无法借，我们只能先将，兄弟结结点和父结点组合然后递归向上寻找是否有可以借的结点。知道根结点结束。

==示例:==

![RB_Tree31](https://tva1.sinaimg.cn/large/008i3skNgy1gy1k3yjhb8j30hc0lj76q.jpg)



​		我们可以看到，上述示例(这是一颗完整的树，而不是部分的，注意哦，18是根结点)从被删除的结点到根结点的回溯过程都是无结点可以结点，我们不断地把被删除结点和被标记的结点的兄弟结点变红，这样就可以平衡每条路径上黑色结点的数量，知道根结点，自然就平衡了，这在2-3-4树中的等效是一样的。因为2-3-4一条路径上的2-结点被删，会导致这条路径的长度减少-1，破坏2-3-4性质,如果我们在回溯过程中都无结点可借，我们最终只能向唯一的2-根结点借，从而使其他路所有路径也都减少1，这样就平衡了。注意，这里只给出了在回溯过程中全程没有结点可以借的情况，实际上如果有结点可以借，将会转化到5.3.1和5.3.2的第一种情况。注意我们一般在红黑树中把像7这样被标记的结点称为==双黑==结点(老实说我也不知道为啥这样叫，挺无聊的叫法)。

##### 5.3.3 被删除的后继结点(或者双黑结点)的兄弟结点是"表面兄弟"

​		"表面兄弟"的概念在5.1.2中已经提过了，我们直接看示例。

==示例:==

​                ![RB_Tree32](https://tva1.sinaimg.cn/large/008i3skNgy1gy1t2mhuf6j30hf0gut9q.jpg)



​		可以看到我们需要通过旋转，找到其兄弟结点，然后，就能根据5.3.1和5.3.2来进行判断，但其不会触发5.3.2的第二种情况,因为父结点为3-结点。

##### 5.3.4 正式实现

==具体步骤:==

​		(1) 从根结点不做任何调整，先递归下降到待删除关键字结点，然后递归下降到后继结点。

​		(2) 检查后继结点，如果为红色结点，说明其在3-或者4-结点中，保存其值，然后直接删除即可。回溯过程中赋给含有待删除关键字的结点，无需任何调整。

​		(3) 否则检查其左子结点后者右子结点。如果有一个结点为红色，则删除后继结点，将红色子结点变为黑色，然后返回。回溯过程中无需调整。

​		(4) 如果(2)、(3)都不成立，回溯过程中将根据5.3.1、5.3.2、5.3.3来进行调整,直到平衡。

==实现代码:==

```c++
void RBTree::recursiveErase(const int & value)
{
    _sentinelNode->leftChild = _root = auxRecursiveErase(_root, value);
}
```

```c++
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
                TreeNode* tempNode = node->leftChild;//保存其左孩子
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
      	//然后将会寻找其后继，注意这里并没有直接下降到右子结点，因为我们可以在跳出这个if后和value继续进行判断，因为value == node->value,所以会在下一个if中满足value>=node->value,因此也会从右子结点下降，这样的好处是我们在回溯过程中可以直接利用下面的用于调整的代码，否则在这里直接下降的话还得单独写一个调整平衡的代码，这样显然不够简洁。
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
                node->rightChild->color = false;//调整颜色
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
        //这个过程和上面是镜像的，怒在赘述
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
```

​		主要是递归实现，代码没什么好说的，里面用了两个静态局部变量，flag用来表示是否有双黑结点，如果有双黑结点，我们才进行调整，另外一个适用于找到后继结点覆盖含有待删除关键字结点的关键字用的，最终删除后继结点。

==时间复杂度:==和插入算法一样为O($\lg{n}$)。

 		噗，终于结束了。最后唠叨一句，注意，删除的自底向上方法也可以用非递归的形式实现，但这样的话TreeNode里面得额外存储一个变量，TreeNode* parent,这样就可以访问父结点了。这里就不演示了。留给同志们自己解决。

## 四、应用场景

​		红黑树相对于AVL树平衡没那么严格，所以查找相对于AVL树会慢一些，但不会像AVL树那样应为平衡实施大量的旋转，所以适合插入删除较为频繁的场景，数据量再不是特别大到需要用磁盘存储的情况下，表现优异。Linux文件系统，c++的std::map都用到了红黑树。

## 五、如有错误或描述不当，请多多指教！不胜感激！

​		完整代码中提供了2-3-4树结合讨论的自顶向下的插入和删除操作以及自底向上的插入和删除操作函数以及其他函数，但并未提供左红链接红黑树的代码，可以参考本md文件自己撰写。此外，本完整代码提供了红黑树的校验函数，可以用于检验红黑树。

![d026c6935c5a8c840be8e16ff5425256](https://tva1.sinaimg.cn/large/008i3skNgy1gy1zxfh7d1j30u018e78b.jpg)

​																																				                                                2022年 1月4日

​                                                                                                   	                                                                     weekie_OUO