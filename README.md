# DataStructure 

## 介绍

​		本项目为作者在学习数据结构中的一些笔记和一些实现代码，现阶段主要完成了本科期间的一些基本数据结构的撰写，后序会陆续添加一些高阶数据结构。

## 内容说明

### 文件说明

​		这里对项目中的一些内容进行说明，一般在每一个数据结构所在的文件夹下，你可以看到如下三种类型的文件:

```sh
*.cpp
*.hpp
*.md
```

​		其中*.cpp和 *.hpp文件是有关数据结构的代码文件，主要由c++撰写。 *.md文件主要是有关数据结构的讲解文件，由于GitHub上对数学公式的显示支持不是很好，这里建议读者下载这个markdown文件进行阅读。

### 笔记部分内容说明

​		正如上面所见每个数据结构的文件夹下都含有一个对该数据结构进行讲解的markdown文件。每个markdown文件最开始部分提供了你需要在学习此知识时需要的前置知识。如下图所示:

![截屏2022-04-12 上午10.27.23](https://tva1.sinaimg.cn/large/e6c9d24egy1h16qan0fnhj213v0gj40d.jpg)

​		读者可以根据自己所学的知识并根据这块内容合理的组织自己的学习顺序。当然作者在basic_datastructre文件夹下也包含了一个README.md文件，里面给出了作者推荐的学习顺序，读者也可以参考那个进行学习。有些需要的知识点可能在本项目中并没有显式地提及，例如有关数学方面，读者可以自己查找有关方面的资料去学习这方面的知识。对于一些较为困难或者需要更多其他知识的内容，作者在标题旁会加“(拓展)”，例如:

![截屏2022-04-12 上午10.37.05](https://tva1.sinaimg.cn/large/e6c9d24egy1h16qkd98xzj20u301yq33.jpg)

​		读者可以根据自身的能力去学习这方面的内容。

### 测试数据结构

​		你可以对该项目中的数据结构进行测试，这是你需要撰写一个测试文件，并编写你想要测试的代码。你需要在你编写的测试文件中包含有关数据结构的头文件的预处理代码:

```c++
#include <*.hpp>//*为对应数据结构文件夹下头文件的名字
int main(int argc, char** argv)
{
  	/*你的测试代码*/
    return 0；
}
```

​		编写完成后如果你想编译并运行，你需要指定好编译标准，本项目中的代码至少需要c++11，所以你应该在编译选项中添加`-std=c++11`。其次还需要包含`-I 头文件所在的位置`，然后还需要`源文件 -o 生成的目标文件名`。以MacOS下的clang++为例，假设测试文件为AVLTree_test.cpp，在命令行中可以键入如下命令:

```sh
clang++ -std=c++11 -I ./basic_datastructre/AVLtree ./basic_datastructre/AVLtree/AVLTree.cpp ./test/AVLTree_test.cpp 
-o ./test/bin/AVLtree_test
```

 		更多有关这方面的细节你需要参考你使用编译器的说明。

## 参考书籍

​		每个数据结构文件中的markdown文件参考了一些有关算法和数据结构的经典书籍，这里列出一部分。

​		***Introduction To Algorithms*** By Thomas H. Cormen, Charles E Leiserson, Ronald L Rivest , Clifford Stein · 2001

​		***Algorithm Design*** By Jon Kleinberg, Eva Tardos ·2013

## 联系作者

​		如果你对本项目的内容有什么更好的指导性的意见或者建议，可以通过邮箱联系作者。

​		Google邮箱:3394417523@gmail.com

​		qq邮箱:3394417523@qq.com

