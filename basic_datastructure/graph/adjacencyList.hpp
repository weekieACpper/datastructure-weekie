/*
 * @Author: weekie
 * @Date: 2022-03-28 12:17:49
 * @LastEditTime: 2022-04-11 09:06:31
 * @LastEditors: Please set LastEditors
 * @Description: 邻接链表表示图的声明部分
 * @FilePath: /datastructure/basic_datastructre/graph/adjacencyList.hpp
 */
#ifndef __ADJACENCYLIST_HPP__
#define __ADJACENCYLIST_HPP__
#include <vector>
#include <iostream>
#include <list>
/**
 * @struct:邻接链表表示法中链表的结点 
 */
struct Node
{
    Node():next(nullptr){}//构造函数
    Node(int a, int b, int c):vertex1(a),vertex2(b),weight(c),next(nullptr){}
    Node(int a, int b, int c, Node* node):vertex1(a),vertex2(b),weight(c),next(node){}
    Node(const Node & other):vertex1(other.vertex1),vertex2(other.vertex2),weight(other.weight),next(nullptr){}//拷贝构造函数
    int vertex1;//结点编号
    int vertex2;//结点编号
    int weight;//权值
    Node* next;//下一个结点
};
/**
 * @class: 邻接链表表表示图的接口类
 */
class adjacencyList//有向加权图的邻接链表表示
{
public:
    adjacencyList();//构造函数
    adjacencyList(const adjacencyList & other);//复制构造函数
    adjacencyList & operator=(const adjacencyList & other);//复制构造运算符
    ~adjacencyList();//析构函数
public:
    int getVertexNum();//获取图中结点数
    int getEdgeNum();//获取图中的边数
    int getWeight(int a, int b);//获得该边的权值
    void insertEdge(int a, int b, int value);//插入一个条边。
    int insertVertex();//在图中增加新的点并返回该点点编号。
    void eraseEdge(int a, int b);//删除对应的边
    void eraseVertex(int a);//删除该结点，并将和该结点相连的边删除
    void modifyWeight(int a, int b, int value);//修改边的权值
    void BFS(int a);//广度优先搜索
    void DFS();//深度优先搜索
    adjacencyList transpose();//返回转置的图
private:
    void DFS(int a, int & time, std::vector<int> & stateList, std::vector<std::pair<int,int>> & timeList);
private:
    std::vector<Node* > _array;//存储邻接链表的数组
    std::list<int> _freeSerialNum;//记录未使用的编号。
    int _numOfVertex;//记录结点数
    int _numOfEdge;//记录边的数目。
};
#endif