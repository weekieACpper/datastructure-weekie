/*
 * @Author: weekie
 * @Date: 2022-03-28 12:18:36
 * @LastEditTime: 2022-04-11 09:28:48
 * @LastEditors: weekie
 * @Description: 邻接链表法表示的图的接口实现部分
 * @FilePath: /datastructure/basic_datastructre/graph/adjacencyList.cpp
 */
#include <adjacencyList.hpp>
#include <algorithm>
#include <stdexcept>
/**
 * @description: 图构造函数
 * @param {*}
 * @return {*}
 */
adjacencyList::adjacencyList():_numOfVertex(0),_numOfEdge(0){}
/**
 * @description: 拷贝构造函数
 * @param {const adjacencyList &} other: 其他代拷贝的图
 * @return {*}
 */
adjacencyList::adjacencyList(const adjacencyList & other):_array(other._array.size(), nullptr),_freeSerialNum(other._freeSerialNum),_numOfVertex(other._numOfVertex),_numOfEdge(other._numOfEdge)
{
    //由于_array中为用户自己手动构建动态内存的的Node，因此拷贝是也应该手动为新的对象是实例申请动态内存，而不是直接使用原来实例的的同台内存
    int size = other._array.size();
    for (size_t i = 0; i < size; i++)
    {
        //以下相当于在拷贝链表
        Node* sourceNode = other._array[i];//代拷贝的第一个即诶单
        Node* targetNode = new Node(*sourceNode);
        _array[i] = targetNode;
        sourceNode = sourceNode->next;
        while(sourceNode != nullptr)
        {
            targetNode->next = new Node(*sourceNode);
            sourceNode = sourceNode->next;
            targetNode = targetNode->next;
        }
        targetNode->next = nullptr;//链表中最后一个结点应该指向空结点
    }
}
/**
 * @description: 拷贝赋值运算符，只有在该实例已经存在的情况下才会被调用
 * @param {const adjacencyList &} other: 赋值运算符右边的图
 * @return {adjacencyList}: 返回被赋值后的图
 */
adjacencyList & adjacencyList::operator=(const adjacencyList & other)
{
    //自赋值检测
    if (this != &other)
    {
        //先释放滋生的空间
        for (size_t i = 0; i < _array.size(); i++)
        {
            //以下过程和链表的析构函数是一样的
            Node* targetNode = _array[i];
            while (targetNode != nullptr)
            {
                Node* deleteNode = targetNode;
                targetNode = targetNode->next;
                delete deleteNode;
            }
        }
        _array.resize(other._array.size());//重新改变大小
        //之后的操作和在拷贝构造函数中是一样的
        int size = other._array.size();
        for (size_t i = 0; i < size; i++)
        {
            //以下相当于在拷贝链表
            Node* sourceNode = other._array[i];//代拷贝的第一个即诶单
            Node* targetNode = new Node(*sourceNode);
            _array[i] = targetNode;
            sourceNode = sourceNode->next;
            while(sourceNode != nullptr)
            {
                targetNode->next = new Node(*sourceNode);
                sourceNode = sourceNode->next;
                targetNode = targetNode->next;
            }
            targetNode->next = nullptr;//链表中最后一个结点应该指向空结点
        }
        _freeSerialNum = other._freeSerialNum;
        _numOfVertex = other._numOfVertex;
        _numOfEdge = other._numOfEdge;
    }
    return *this;
}
/**
 * @description: 图的析构函数
 * @param {*}
 * @return {*}
 */
adjacencyList::~adjacencyList()
{
    for (size_t i = 0; i < _array.size(); i++)
    {
        //以下过程和链表的析构函数是一样的
        Node* targetNode = _array[i];
        while (targetNode != nullptr)
        {
            Node* deleteNode = targetNode;
            targetNode = targetNode->next;
            delete deleteNode;
        }
    }
}
/**
 * @description: 获取图中实际存在的点的数量
 * @param {*}
 * @return {int}:结点数量
 */
int adjacencyList::getVertexNum()
{
    return _numOfVertex;
}
/**
 * @description: 获取图中实际存在边的数量
 * @param {*}
 * @return {int}:边的数量
 */
int adjacencyList::getEdgeNum()
{
    return _numOfEdge;
}
/**
 * @description: 获取图中某条边的权值
 * @param {int} vertex1: 边的出发结点
 * @param {int} vertex2: 边的结束结点
 * @return {int}: 返回边的权值大小
 */
int adjacencyList::getWeight(int vertex1, int vertex2)
{
    if (vertex1 < 0 || vertex1 >= _array.size() || vertex2 < 0 || vertex2 >= _array.size())
    {
        throw std::invalid_argument("the serial number of vertex should be > 0 and < the max serial Of vertices!,");
    }
    Node* targetNode = _array[vertex1]->next;
    while (targetNode != nullptr && targetNode->vertex2 != vertex2)  
    {
        targetNode = targetNode->next;
    }
    if (targetNode == nullptr)
    {
        return 0;
    }
    return targetNode->weight;
}
/**
 * @description: 在图中插入一条边
 * @param {int} a: 边的起始结点
 * @param {int} b: 边的结束结点
 * @param {int} value: 边的权值
 * @return {*}
 */
void adjacencyList::insertEdge(int a, int b, int value)
{
    //w我们可以直接在链表的头结点插入边，但是我如果过考虑插入重复边的情况下
    //我们应该先检测这条边是否存在了
    if (getWeight(a, b) != 0)
    {
        std::cout << "insert failed, the edge already existed. you can use modifyWeight() to modify weight." << std::endl;
        return;
    }
    _array[a]->next = new Node(a, b, value);
    _numOfEdge++;
}
/**
 * @description:在图中插入一个新结点 
 * @param {*}
 * @return {int}: 返回结点在图中的编号
 */
int adjacencyList::insertVertex()
{
    if (_freeSerialNum.size() != 0 )
    {   //如果有空闲的编号，直接回传空闲编号即可。
        int serialNum =  _freeSerialNum.front();
        _freeSerialNum.pop_front();
        return serialNum;
    }
    _array.emplace_back(new Node());//创建新结点
    _numOfVertex++;
    return _numOfVertex - 1;//返回其编号
}
/**
 * @description: 在图中删除一条边
 * @param {int} a: 边的起始结点
 * @param {int} b: 边的结束结点
 * @return {*}
 */
void adjacencyList::eraseEdge(int a, int b)
{
    if (a < 0 || a >= _array.size() || b < 0 || b >= _array.size())
    {
        throw std::invalid_argument("the serial number of vertex should be > 0 and < the max serial Of vertices!");
    }
    Node* preNode = _array[a];//指向该量表的头结点
    Node* targetNode = preNode->next;
    while(targetNode != nullptr && targetNode->vertex2 != b )
    {
        preNode = targetNode;
        targetNode = targetNode->next;
    }
    if(targetNode != nullptr)
    {
        preNode->next = targetNode->next;
        delete targetNode;//释放空间
        _numOfEdge--;
    }
}
/**
 * @description: 在图中删除一个结点
 * @param {int} a:待删除的结点编号
 * @return {*}
 */
void adjacencyList::eraseVertex(int a)
{
    if (a < 0 || a >= _array.size())
    {
        throw std::invalid_argument("he serial number of vertex should be > 0 and < the max serial Of vertices!");       
    }
    //首先要确保这个结点编号不在空闲编号中
    if (std::find(_freeSerialNum.begin(), _freeSerialNum.end(),a) != _freeSerialNum.end())
    {
        return;//直接返回，因为空闲编号代表的结点世纪不存在于图中
    }
    //我们需要扫描整个图中去寻找a的入边并删除，同时还要删除a本身的出边
    //先删除a本身的出边
    Node* preNode = _array[a];
    Node* targetNode = preNode->next;
    while (targetNode != nullptr)
    {
        preNode = targetNode;
        targetNode = targetNode->next;
        delete preNode;
        _numOfEdge--;
    }
    for (size_t i = 0; i < _array.size(); i++)
    {
        preNode = _array[i];
        targetNode = _array[i]->next;
        while(targetNode != nullptr && targetNode->vertex2 != a)
        {
            targetNode = targetNode->next;
        }
        if (targetNode != nullptr)
        {
            preNode->next = targetNode->next;
            delete targetNode;//删除这个结点
            _numOfEdge--;
        }
    }
    _numOfVertex--;
    //将该结点加入空闲编号
    _freeSerialNum.push_back(a);
}
/**
 * @description: 改变图中某条边的权值
 * @param {int} a:边的起始结点
 * @param {int} b:边的某位结点
 * @param {int} value:边的权值
 * @return {*}
 */
void adjacencyList::modifyWeight(int a, int b, int value)
{
    if (getWeight(a,b) == 0)
    {
        std::cout << "you can't use this function when the Edge you want to modify is not exist,maybe you should call insertEdge() " << std::endl;
    }
    Node* targetNode = _array[a]->next;
    //找到这个结点
    while(targetNode != nullptr && targetNode->vertex2 != b)
    {
        targetNode = targetNode->next;
    }
    if (targetNode == nullptr)
    {
        return ;
    }
    targetNode->weight = value;//修改权值。
}
/**
 * @description: 有向图的广度优先搜索
 * @param {int} a: 搜索结点的编号
 * @return {*}
 */
void adjacencyList::BFS(int a)
{
    if (a < 0 || a >= _array.size())
    {
        std::cout << "you can't call this function on vertex which doesn't exist" << std::endl;
    }
    std::vector<int> temp(_array.size(),0);
    std::list<int> queue1;
    queue1.push_back(a);
    while (!queue1.empty()) 
    {
        int vertex = queue1.front();//取出对首元素
        std::cout << vertex << " ";//打印编号
        queue1.pop_front();//弹出这个结点
        temp[a] = 1;//标记为已经访问过了。
        Node* targetNode = _array[a]->next;//访问其相邻结点
        while(targetNode != nullptr)
        {
            if (!temp[targetNode->vertex2])//如果未访问过这个结点
            {
                queue1.push_back(targetNode->vertex2);//相邻结点加入队列
            }
        }
    }
}
/**
 * @description: 有向图的深度优先搜索，生成深度优先森林
 * @param {*}
 * @return {*}
 */
void adjacencyList::DFS()
{
    std::vector<int> stateList(_array.size(), 0);//状态为0表示为白色，1表示为灰色，2表示为黑色
    std::vector<std::pair<int,int>> timeList(_array.size(), {0,0});//记录每个结点的时间戳
    for (auto i = _freeSerialNum.begin(); i != _freeSerialNum.end(); i++)//刚开始我们扫描空闲编号的结点，并将其对应状态置为黑
    {
        stateList[*i] = 2;
    }
    int time = 0;
    //生成森林。
    for (size_t i = 0; i < _array.size(); i++)
    {
        if(stateList[i] == 0)
        {
            DFS(i, time, stateList, timeList);//调用重载函数开始生成不同的深度优先搜索树
        }   
    }
}
/**
 * @description: 深度优先搜索的重载函数，生成深度优先树
 * @param {int} a:待搜索结点的编号
 * @param {int &} time:用于记录的时间戳
 * @param {std::vector<int> & } stateList:用语记录每个结点的颜色。
 * @param {std::vector<std::pair<int,int>> & } timeList:记录每个结点的时间戳
 * @return {*}
 */
void adjacencyList::DFS(int a, int & time, std::vector<int> & stateList, std::vector<std::pair<int,int>> & timeList)
{
    time += 1;//时间加1，为搜索时间
    timeList[a].first = time;//记录v.d
    stateList[a] = 1;//变为灰色，说明被搜索到了(被发现了)
    Node* targetNode = _array[a]->next;
    while(targetNode != nullptr)
    {
        if (stateList[targetNode->vertex2] == 0)//先判断是否是白色，只有是白色我们才可以搜索它
        {
            DFS(targetNode->vertex2, time , stateList, timeList);//递归调用
        }
    }    
    //以下为回溯过程中的代码
    std::cout << a << " ";//打印该结点编号。
    stateList[a] = 2;//置为黑色
    time += 1;//时间戳加1
    timeList[a].second = time;//记录该时间戳为v.f
}
/**
 * @description: 对当前的图进行转置
 * @param {*}
 * @return {adjacencyList}: 返回转置后的图
 */
adjacencyList adjacencyList::transpose()
{
    adjacencyList temp;
    for (size_t i = 0; i < _array.size(); ++i)
    {
        adjacencyList::insertVertex();//插入结点
    }
    for (size_t i = 0; i < _array.size(); ++i)
    {
        Node* targetNode = _array[i]->next;
        while (targetNode != nullptr)
        {
            temp.insertEdge(targetNode->vertex2, i ,targetNode->weight);
            targetNode = targetNode->next;
        }
    }
    return temp;
}