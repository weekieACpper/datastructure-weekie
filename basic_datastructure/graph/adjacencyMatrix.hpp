/*
 * @Author: weekie
 * @Date: 2022-03-28 12:18:59
 * @LastEditTime: 2022-04-11 09:32:58
 * @LastEditors: weekie
 * @Description: 邻接矩阵表示法的图的接口部分
 * @FilePath: /datastructure/basic_datastructre/graph/adjacencyMatrix.hpp
 */
#ifndef __ADJACENCYMATRIX_HPP__
#define __ADJACENCYMATRIX_HPP__
#include <vector>
#include <queue>
#include <list>
/**
 * @struct:邻接矩阵表示的图的ADT描述
 */
class adjacencyMatrix
{
public:
    adjacencyMatrix();//构造函数
    ~adjacencyMatrix();//析构函数
public:
    int getVertexNum();//获取图中的结点数
    int getEdgeNum();//获取图中的边数
    int getWeight(int a, int b);//获得改边的权值
    void insertEdge(int a, int b, int value);//插入一个条边。
    int insertVertex();//在图中增加新的点并返回该点点编号。
    void eraseEdge(int a, int b);//删除对应的边
    void eraseVertex(int a);//删除该结点，并将和该结点相连的边删除
    void modifyWeight(int a, int b, int value);//修改边的权值
private:
    std::vector< std::vector<int> > _matrix;//邻接矩阵
    std::list<int> _freeSerialNum;//记录未使用的编号的队列，采用链表实现。
    int _numOfVertex;//记录结点数
    int _numOfEdge;//记录边的数目。
};
#endif