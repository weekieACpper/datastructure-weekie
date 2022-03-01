# AVL平衡树

## 一、一些在学习此内容所需要的准备

### 1 需要提前了解的知识

​	1、了解树的定义，熟悉二叉搜索树的基本操作。

​	2、知道什么是内部结点，什么是根节点，什么是叶子结点，以及在二叉搜索树当中的==前驱==和==后继==的概念。

​	3、时间复杂度的计算，以及记号。

​	4、熟悉c或c++的基本语法。

## 二、AVL树的定义

​		AVL树是一颗二叉搜索树，她的名字来源于这棵树的发明者G. M. Adelson-Velsky和E. M. Landis(取名字的首字母)，它解决了二叉搜索树在的一个显著的问题，那就是在极端情况下二叉搜索树安递增的顺序插入元素会导致二叉搜索树==倾斜==，从而导致查询的时间复杂度$O(h)=O(n)$，退化为链表。AVL树在插入和删除时采用平衡调节，使得树中以每个结点为根的**子树** 的左右子树高度差不超过1来保证$O(h)=O(\log_{2}{n})$ 。我们先看下图的示例。

==示例:==

![AVL树1](https://tva1.sinaimg.cn/large/008i3skNgy1gz9y0el4ncj309e04st8r.jpg)

​		如上图所示以21为根的子树，21的左子树和右子树的高度相差1，满足平衡条件。

### 1 平衡因子

​		在正式介绍AVL树的性质之前，首先引出平衡因子的概念。我们假设结点$x$，则以这个结点为根子树的高度为$h(x)$，$x	$的左子树为$x_{left}$，右子树为$x_{right}$。那么我们定义==平衡因子==:
$$
\nu = |h(x_{left})-h(x_{right})|
$$

### 2 AVL树的性质:

​		1、AVL树满足二叉搜索树的所有性质

​		2、树中每个结点的平衡因子$\nu \le 1$。

### 3 AVL树的高度:

​		我们观察二叉搜索树的叶结点，当叶结点处在同一层时，我们说这时的二叉搜索树是完全平衡的，此时树的高度为$O(h) = O(\log_{2}{n})$，在AVL树中叶结点之间最多只相差一层，所以近似可以认为AVL树的高度为$O(h) = O(\log_{2}{n})=O(\lg{n})$。

## 三、AVL树的基本操作

​		以下代码都将用c++语言实现，不过将不会用到c++的一些不同于c的特性，最多只涉及类以及构造函数、析构函数的概念，只会c也是能看懂的。

### 1 AVL树结点的定义

​		以下AVL树结点的定义:

==实现代码:==

```c++
struct TreeNode
{
    TreeNode():height(0){}
    TreeNode* leftChild;
    TreeNode* rightChild;
    int value;
    int height;
};
```

​		我们给每个结点一个高度，并假设初始根结点的高度为0，处在同一层的结点的高度相等，同时每个结点高度的大小等于其自身到距离自己最远的叶结点的**距离**。具体见如下示例。

==示例:==

![AVL树2](https://tva1.sinaimg.cn/large/008i3skNgy1gza2b9kk8oj309x04m3yh.jpg)

​		从上面的这张图我们可以看到，由于21到距离自己最远的叶结点5或者15要经过3个点(21,10,5)所以其高度为3。而25本身就是叶结点，也结点到其自身的距离为1，所以其高度为1。除此之外，我们定义所有空结点的高度为0。

### 2 AVL树的查找

​		AVL树的查找和二叉搜索树是一样的，这里直接给出实现代码，具体可以见二叉搜索树那一篇markdown。

==实现代码:==

```c++
TreeNode* AVLTree::find(const int & value)
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

==时间复杂度:==为树的高度$O(h) = O(\lg{n})$。

### 3 AVL树的旋转

​		前面提到过AVL树的性质是需要保持每个结点的平衡因子要小于等于1，当插入一个值或者删除一个值，可能会导致一个结点的平衡因子发生变化(这是由于结点高度的变化引起的)然后破环AVL树平衡因子的限制。因此，需要适当的操作来使得一个结点的左右两子树的高度差重新等于小于1。需要注意的时由于我们保证每个结点左右两子树的高度差小于等于1，所以当破坏平衡因子这一性质时，左右两子树的高度差只可能是2，因为插入或者删除一次只插入一个结点，从而我们可以知道对于一个结点当平衡因子为2时，可能出现种情况:

​	1、该结点左子树比右子树的高度大2。

​	2、或者该结点的右子树比左子树的高度大2。

​	这两种情况中有细分了两种小情况。我们以结点的右子树比左子树高2举例，先看下图示。

==示例:==

![AVL树3](https://tva1.sinaimg.cn/large/008i3skNgy1gza47jpu5kj30c60e3dg5.jpg)

​		其中带有颜色的椭圆代表了子树。并且约定紫色椭圆代表的子树的高度比红色椭圆多1。设红色椭圆代表子树的高度为$h(21_{left})$ ，那么紫色就为$h(21_{left})+1$，那么$h(21_{right})=h(21_{left})+2$ (紫色椭圆的高度加上25这个结点)。这因此21的右子树比左子树高2，破坏了平衡因子的条件。注意到图(1)和图(2)的区别在于25的紫色椭圆代表子树和红色椭圆所代表子树的位置。

​		这时可能会有人心生疑惑，为什么在21的右子树比左子树高2情况下，其右子树的两棵子树(即图(1)和图(2)中25左右两个不同颜色的椭圆)的高度必然会相差1呢？是不是会漏考虑两个子树高度相等情况?

​		首先我们考虑25的左右两个椭圆同时为红色的情况。我们可以知道这种情况必然不可能出现，因为我们假设右子树比左子树高2，但如果是红色椭圆的话，那么$h(21_{right})=h(21_{left})+1$，这并未破坏平衡因子的条件。但由于这里讨论的平衡因子条件被破坏的条件下的情况，所以自然排除。 

​		其次考虑25的左右两个椭圆同时都为紫色的情况，这种情况确实满足了右子树比左子树高2，但这种情况依然不会出现。你需要注意的是我们的AVL树插入一次只操纵一个结点。以插入为例，一个紫色椭圆必然是一个红色椭圆插入一个结点后得到的，因为只有插入结点才有可能使得红色椭圆所代表的子树的高度加1，而由于我们一次只插入一个结点，所以，在插入一个结点后，当25的左右都为紫色椭圆时，其在插入前必然经历上面示例中的图(1)或者图(2)的情况。由于图(1)和图(2)已经破坏了21的平衡因子的条件，所以其会被调整，所以根本不会进入到25的左右两个椭圆都为紫色的情况。(但需要注意的是删除就不一样了，删除时可能出现的，这种情况将在删除那部分单独讲)

​	下面我们将分讨论(插入一个结点时)四种不平衡的情况，以及对应的四种操作来保持平衡，我们把类似于这种保持平衡的操作称之为旋转。



#### 3.1 RR非平衡状态

​		首先观察第一种一个结点的平衡因子不平衡的情况，我们称之为RR非平衡状态，先看示例:

==示例:==

![AVL树4](https://tva1.sinaimg.cn/large/008i3skNgy1gzayo99pt0j305y05r748.jpg)

 		在这种状态下，当前结点(21，它的平衡因子为2)的右子树比左子树的高度大2，当前结点**右子树的根结点**为25，然后紫色椭圆为25的右子树。叫RR非平衡状态可以记为right right，其第一个right表示了当前结点(21)中子树高度较高的子树(这种情况中为右子树，所以是第一个right)，第二个right表示了紫色椭圆在其子树中得分位置(在这种情况中为25的右子树，所以为right)。

​		现在考虑如何将这一状态调整至平衡。由于21的右子树比左子树高度大2，所以最好的方法是左子树从右子树能获得一个结点，并且这个结点能使得左子树的高度增加1，右子树的高度减小1。我们可以将21下降一层，然后25去代替21的位置，具体看如下示例:

==示例:==

![AVL树5](https://tva1.sinaimg.cn/large/008i3skNgy1gzazx1z8qvj30de0jx3z8.jpg)

​		

​		上述调整至平衡的状态的操作称为==左旋转==(箭头的方向是朝左)，这时我们可以看到当前结点(25)的平衡因子为0，原因是当前结点的左子树的高度为$h(25_{left})=h(21_{left})+1$，而当前结点的右子树为紫色椭圆代表的子树其高度为红色椭圆代表的高度加1，所以为$h(25_{right})=h(21_{left})+1$。所以25这个结点的平衡因子$\nu = |h(25_{left})-h(25_{right})|=0$。这样，结点就平衡了，并且可以轻易证明这旋转操作并没有破坏二叉搜索树的性质。

#### 3.2 RL非平衡状态

​		在上一小节的基础上，我们就可以知道RL非平衡状态下紫色椭圆在25的左边，如下图所示。

==示例:==

![AVL树6](https://tva1.sinaimg.cn/large/008i3skNgy1gzb0ganu5yj305j05ct8m.jpg)

​		但这次并不能像上面一样简单的进行一次左旋转就完成了，原因是在左旋转后25的右子树不是紫色椭圆，而是红色，而紫色椭圆出现在了左子树中，这样就导致了左子树比右子树高2了，并没有解决平衡问题，如下图所示为旋转后的状态。

==示例:==

![AVL树7](https://tva1.sinaimg.cn/large/008i3skNgy1gzb0r0r2qqj305q059q2u.jpg)

​		所以，我们的解决方案是，把紫色椭圆变为25的右子树(而非现在25的左子树)，从而进入RR状态，这样就能使用左旋转达到平衡状态。具体做法，就是25的右子树(红色椭圆)向25的左子树(紫色椭圆)借一个结点，且能保证这个结点能使红色椭圆所代表的子树高度加1变为紫色椭圆，原来的紫色椭圆代表的子树高度减少1变为红色椭圆。我们假设这一小节第一张示例中的紫色椭圆所代表的子树的根结点为23，我们可以将23替换25所在的位置，然后25下降到其右子树中，使得右子树高度增加1变成紫色椭圆。最后施行左旋转，这一系列操作如下所示。

==示例:==

![AVL树8](https://tva1.sinaimg.cn/large/008i3skNgy1gzb25yy0n4j30bn0thwfm.jpg)

​		上述图里中的第二张图可以看做为==右旋转==(箭头方向)，经过一个右旋转和左旋转后，当前结点(旋转后为23)就平衡了。所以RL旋转可以通过一次右旋转和一次左旋转完成平衡调整。

#### 3.3 LL非平衡状态

​		这个可以看做是RR的镜像，执行一次右旋转即可，不再赘述。

#### 3.4 LR非平衡状态

​		这个可以看做是RL的镜像，先执行一次左旋转再执行右旋转即可，不在赘述。

#### 3.5 旋转的实现

​		这一小节将实现旋转的具体代码，分为左旋转和右旋转。

==实现代码:==

```c++
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
TreeNode* AVLTree::rightRotation(TreeNode* node)
{
    TreeNode* leftNode = node->leftChild;
    node->leftChild = leftNode->rightChild;
    leftNode->rightChild = node;
    node->height = std::max(getNodeHeight(node->rightChild), getNodeHeight(node->leftChild)) + 1;
    leftNode->height = std::max(getNodeHeight(node), getNodeHeight(leftNode->leftChild)) + 1;
    return leftNode;
}
```

​		可以看到实现代码非常简单，右旋转是左旋转的镜像过程。在这其中需要注意的时我们需要把原来node所在的位置的结点在旋转后作为返回值返回，原因是原来node的父结点指针还是指向node，并不是指向旋转后的新结点，所以我们需要把这个新结点返回，以便node原来的父结点调整指针指向。除此之外旋转也改变了结点的高度，这也需要调整，getNodeHeight函数为获取一个结点的高度。对于空结点高度为0，别的结点直接返回TreeNode结构体中定义的height成员。

==时间复杂度:==$O(1)$。

### 4 AVL树的插入

​		在有了旋转着姐内容的保持平衡后，插入操作就变的简单了起来，我们可以按照二叉搜索树一样找到插入的位置，然后插入新结点，之后我们可以从这个新结点往回回溯，检查经过路径上每个结点的平衡因子，然后根据平衡因子进行适当的旋转操作，这个过程可以用递归或者使用栈来完成。这里我们使用栈来保存插入过程中经过的结点，另外一种递归的方法，读者可以自行完成并检验。

==实现步骤:==

​		(1) 从根结点开始搜索，找到待插入的位置，并在途中将经过路径上的结点入栈。

​		(2) 插入新的结点，然后取出栈顶的结点，并将其弹出栈。

​		(3) 重新设置刚刚从栈中取出结点的高度为其左右子结点中较大的那个高度加1。

​		(4) 然后根据该结点的平衡因子判断该结点是否需要平衡操作。

​			a. 如果需要，则根据四种非平衡状态进行旋转。

​			b. 否则直接进入步骤(5)

​		(5) 检查栈是否为空，如果为结束流程，否则跳转到步骤(2)。

==示例:==

![AVL树9](https://tva1.sinaimg.cn/large/008i3skNgy1gzcsyq14r9j30fv150ac3.jpg)



​		在上述的示例中，我们首先和二叉搜索树一样的算法，插入结点11，途中我们将路径上经过的结点(它们用紫色线连接)放入栈中。然后对栈中的每个结点设置其高度为左右子结点中较大的高度加1并判断其平衡因子进行适当的平衡体调整，最后直到栈为空。

==实现代码:==

```c++
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
        int leftHeight = getNodeHeight(targetNode->leftNode), rightHeight = getNodeHeight(targetNode->rightNode);
        targetNode->height = std::max(leftHeight, rightHeight); + 1;//调整高度
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
                _root = rightRotation(targetNode);
            }
        }
        else if(rightHeight > leftHeight + 1)//R~ 与上面的镜像
        {
            tempNode = targetNode->rightChild;
            if(getNodeHeight(tempNode->rightChild) > getNodeHeight(tempNode->leftChild))//判断紫色椭圆的位置
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
                _root = leftRotation(targetNode);
            }
        }
    }
    _treeSize++;
}
```

​		如果使用递归代码将更为简洁，主要看平衡的部分，if语句先分为两大类(L～和R～)，再细分为两小类(以L为例，LL或LR)，由于旋转会导致结点位置变换，所以targetNode的父结点的孩子指针需要调整，父结点存放在栈中。

==时间复杂度:== $O(2\lg{n})=O(2h)=O(h)$，其中旋转操作为$O(1)$，对总的时间复杂度无影响。

==空间复杂度:== $O(\lg{n})=O(h)$，为栈存储路径上的结点。

### 5 AVL树的删除

#### 5.1 一些删除特有的非平衡状态

​		删除中会出现在不平衡的情况主要出现在一个结点的其中一棵子树比另一棵高1，然后删除的结点在另外一棵子树中，恰好导致该子树的高度减小1，从而导致该结点平衡因子不满足要求。

​		仔细考虑一下，删除中是会出现较高的那个子树中出现两个紫色椭圆的情况的，我们先看如下示例。

==示例:==

![AVL树10](https://tva1.sinaimg.cn/large/008i3skNgy1gzdb6i8ilzj304005bmx3.jpg)

​		这张图中，10结点右子树无结点，15的左右子树都为一个结点，由之前作出的假设。我们设10的右子树为红色椭圆代表的子树，那么15的左右两个子树都应该用紫色椭圆表示。出现这种情况是因为15的两棵子树高度在删除之前等于10的右子树高度。删除之后正好10的右子树高度减少1，从而产生了两个紫色椭圆。

​		这种情况调整平衡，只需要一步单旋转即可，上述图中左旋转即可满足平衡条件。

​		至于在插入中出现的其他情况，删除都会出现，和插入的处理方法是一样的。

#### 5.2 正式实现AVL树的删除算法

​		AVL树的删除操作也没有那么复杂，我们先要找到待删除的值所在的结点，然后和二叉搜索树一样，如果其左子树或者右子树不为空，找到其==前驱==或者==后继==，然后用该叶结点的值代替这个结点，删除==后继==或者==前驱==结点，在操作之中我们仍然可以使用栈保存路径上经过的结点(或者递归)。完成这部操作之后，我们检查经过路径上的所有平衡因子，执行对应的旋转操作。

==实现步骤:==

​		这里我们找到待删除值所在的结点后，如果其右子树不为空，之后统一寻找其==后继==结点。

​		(1) 我们首先和二叉搜索树的删除一样，找到待删除的结点，途中保存路径上经过的结点。

​		(2) 找到后寻找其后继结点，如果其右子树为空，就删除当前结点，将其左子结点替换自己的位置(如果有的话)。

​		(3) 删除后检查保存的每个结点平衡因子，和插入一样进行适当的调整。

==示例:==

![AVL树11](https://tva1.sinaimg.cn/large/008i3skNgy1gzdc1ebputj30g50l63zc.jpg)

==实现代码:==

​		这里将实现递归版本的AVL树的删除，读者可以自行实现使用栈版本的删除。

```c++
void AVLTree::erase(const int & value)
{
    _sentinelNode->leftChild = _root = auxErase(_root, value, nullptr);
}
void AVLTree::auxErase(TreeNode* node, const int & value, TreeNode* storedNode)
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
            delete targetNode;
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
            return nullptr；
        }
        node->leftChild = auxErase(node->leftChild, value, storedNode);
    }
    else
    {
        node->rightChild = auxErase(node->rightChild, value, storedNode);
    }
    int leftHeight = getNodeHeight(node->leftChild), rightHeight = getNodeHeight(node->rightChild);
    //递归回来后要进行高度设置和平衡调整
    node->height = std::max(leftChild, rightChild) + 1;
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
```

​		其中，_sentinelNode为哨兵结点，其没有值，但是它的左子结点永远指向_root，类似于链表中的头结点，这里可加可不加(可忽略)。

==时间复杂度:==和插入一样，$O(2\lg{n})=O(2h)\approx O(h)$

==空间复杂度:== 递归栈长度为$O(h)=O(\lg{n})$

## 四、应用场景

​		AVL树由于保持了严格的平衡，所以适用于查找较多的场景，但如果在插入和删除较多的场景下，其性能可能会有些影响，因为其中将会有大量的操作浪费在保持平衡上。

## 五、如有错误或描述不当，请多多指教！不胜感激！

![9604aebe9fe04daaed57d3a218dc965a](https://tva1.sinaimg.cn/large/008i3skNgy1gzdiar0wqlj30as09pweq.jpg)

​																																																	  2022年2月15日

​                                                                                                   	                                                                     weekie_OUO