/*
 * @Author: weekie
 * @Date: 2022-03-28 12:19:26
 * @LastEditTime: 2022-04-11 09:53:40
 * @LastEditors: weekie
 * @Description: 邻接矩阵表示的图的接口实现部分
 * @FilePath: /datastructure/basic_datastructre/graph/adjacencyMatrix.cpp
 */
#include <adjacencyMatrix.hpp>
#include <algorithm>
#include <iostream>
/**
 * @description: 图的构造函数
 * @param {*}
 * @return {*}
 */
adjacencyMatrix::adjacencyMatrix():_numOfVertex(0),_numOfEdge(0){}
/**
 * @description: 析构函数
 * @param {*}
 * @return {*}
 */
adjacencyMatrix::~adjacencyMatrix(){}
/**
 * @description: 获取图中结点的数量
 * @param {*}
 * @return {int} 图中结点的数量
 */
int adjacencyMatrix::getVertexNum()
{
    return _numOfVertex;
}
/**
 * @description: 获取图中边的数量
 * @param {*}
 * @return {int}: 图中边的数量
 */
int adjacencyMatrix::getEdgeNum()
{
    return _numOfEdge;
}
/**
 * @description: 获取某条边的权值
 * @param {int} vertex1:该边的起始结点
 * @param {int} vertex2:该边的结束结点
 * @return {int}: 该边的权值
 */
int adjacencyMatrix::getWeight(int vertex1,int vertex2)
{
    if (vertex1 < 0 || vertex1 >= _matrix.size() || vertex2 < 0 || vertex2 >= _matrix.size())
    {
        throw std::invalid_argument("the serial number of vertex should be > 0 and < the total num Of vertices!,you can call getVertexNum to get it!");
    }
    return _matrix[vertex1][vertex2];
}
/**
 * @description: 在图中插入一条边
 * @param {int} a: 边的起始结点
 * @param {int} b: 边的结束结点
 * @param {int} value: 待插入边的权值
 * @return {*}
 */
void adjacencyMatrix::insertEdge(int a, int b, int value)
{
    //有向图的边是双向的，因此我们得同时修改matrix[a][b]和matrix[b][a]
    if (getWeight(a, b) != 0)
    {
        std::cout << "insert failed, the edge already existed. you can use modifyWeight() to modify  weight." << std::endl;
        return;
    }
    _matrix[a][b] = value;
    _matrix[b][a] = value;
    _numOfEdge++;
}
/**
 * @description: 在图中插入一个新结点
 * @param {*}
 * @return {int}: 返回新结点在图中的编号。
 */
int adjacencyMatrix::insertVertex()
{
    if (_freeSerialNum.size() != 0 )
    {   //如果有空闲的编号，直接回传空闲编号即可。
        int serialNum =  _freeSerialNum.front();
        _freeSerialNum.pop_front();
        return serialNum;
    }
    _matrix.emplace_back(std::vector(_numOfVertex + 1, 0));//添加新的一行
    for(int i = 0; i < _matrix.size() - 1; i++)//对原来的矩阵增加一个列
    {
        _matrix[i].emplace_back(0);
    }
    _numOfVertex++;
    return _numOfVertex - 1;
}
/**
 * @description: 在图中删除一条边
 * @param {int} a:待删除边的起始结点
 * @param {int} b:但删除结点的结束结点
 * @return {*}
 */
void adjacencyMatrix::eraseEdge(int a, int b)
{
    if (a < 0 || a >= _matrix.size() || b < 0 || b >= _matrix.size())
    {
        throw std::invalid_argument("the serial number of vertex should be > 0 and < the total num Of vertices!,you can call getVertexNum to get it!");
    }
    if (_matrix[a][b] != 0)
    {
        _matrix[a][b] = 0;//置为0即可
        _matrix[b][a] = 0;//无向图中也要有这部。
        _numOfEdge--;
    }
}
/**
 * @description: 删除图中一个结点
 * @param {int} a:待删除的结点编号
 * @return {*}
 */
void adjacencyMatrix::eraseVertex(int a)
{
    if (a < 0 || a >= _matrix.size())
    {
        throw std::invalid_argument("he serial number of vertex should be > 0 and < the max serial Of vertices!");       
    }
    //现在空闲链表表中寻找是否有该编号
    if (std::find(_freeSerialNum.begin(), _freeSerialNum.end(),a) != _freeSerialNum.end())
    {
        return;
    }
    //先将行a处的所有位置置为0.
    for (auto i = _matrix[a].begin(); i != _matrix[a].end(); i++)
    {
        if (*i != 0)
        {
            *i = 0;
            _numOfEdge--;
        }
    }
    for (size_t i = 0; i < _matrix.size(); i++)
    {
        //在每一行中找到这个点的编号对应的位置，即列a，并将其置为0
        _matrix[i][a] = 0;
    }
    _freeSerialNum.push_back(a);//将删除后的结点编号加入空闲链表。
    _numOfVertex--;
}
/**
 * @description: 修改边的权值
 * @param {int} a: 边的起始结点
 * @param {int} b: 边的结束结点
 * @param {int} value: 边的新的权值
 * @return {*}
 */
void adjacencyMatrix::modifyWeight(int a, int b, int value)
{
    if (getWeight(a,b) == 0)
    {
        std::cout << "you can't use this function when the Edge you want to modify is not exist,maybe you should call insertEdge() " << std::endl;
    }
    //和插入的时候几乎一样，但其只是在已有边的基础上修改。
    _matrix[a][b] = value;
    _matrix[b][a] = value;
}