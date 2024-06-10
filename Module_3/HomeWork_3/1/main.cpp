// Задача 1. «Представление графа».(5 баллов)

// Дан базовый интерфейс для представления ориентированного графа:
// struct IGraph {
// virtual ~IGraph() {}
	
// 	// Добавление ребра от from к to.
// virtual void AddEdge(int from, int to) = 0;

// 	virtual int VerticesCount() const  = 0;

// virtual std::vector<int> GetNextVertices(int vertex) const = 0;
// virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
// };

// Необходимо написать несколько реализаций интерфейса:
// ListGraph, хранящий граф в виде массива списков смежности,
// MatrixGraph, хранящий граф в виде матрицы смежности,
// SetGraph, хранящий граф в виде массива хэш-таблиц/сбалансированных деревьев поиска,
// ArcGraph, хранящий граф в виде одного массива пар {from, to}.
// Также необходимо реализовать конструктор, принимающий const IGraph&. 
// Такой конструктор должен скопировать переданный граф в создаваемый объект.
// Для каждого класса создавайте отдельные h и cpp файлы.
// Число вершин графа задается в конструкторе каждой реализации.

#include<vector>
#include<iostream>
#include<cassert>
#include<queue>
#include "ListGraph.cpp"
#include "MatrixGraph.cpp"
#include "SetGraph.cpp"
#include "ArcGraph.cpp"

void BFS( const IGraph& graph, int vertex, void( *visit )(int) )
{
    std::vector<bool> visited( graph.VerticesCount(), false );
    std::queue<int> bfsQ;
    bfsQ.push( vertex );
    visited[vertex] = true;
    while( bfsQ.size() > 0 ) {
        int current = bfsQ.front();
        bfsQ.pop();
        visit( current );
        std::vector<int> adjList = graph.GetNextVertices( current );
        for( int i = 0; i < adjList.size(); ++i ) {
            if( !visited[adjList[i]] ) {
                bfsQ.push( adjList[i] );
                visited[adjList[i]] = true;
            }
        }
    }

}

int main() {
    int vertices;
    std::cin >> vertices;
    ListGraph listGraph( vertices );

    int edges;
    std::cin >> edges;

    for ( int i = 0; i < edges; ++i ) {
        int from, to;
        std::cin >> from >> to;
        listGraph.AddEdge( from, to );
    }

    MatrixGraph matrixGraph( listGraph );
    SetGraph setGraph( listGraph );
    ArcGraph arcGraph( listGraph );

    std::cout << "\nBFS traversal of ListGraph:\n";
    BFS( listGraph, 0, []( int vertex ) { std::cout << vertex << " "; } );

    std::cout << "\n\nBFS traversal of MatrixGraph:\n";
    BFS( matrixGraph, 0, []( int vertex ) { std::cout << vertex << " "; } );

    std::cout << "\n\nBFS traversal of SetGraph:\n";
    BFS( setGraph, 0, []( int vertex ) { std::cout << vertex << " "; } );

    std::cout << "\n\nBFS traversal of ArcGraph:\n";
    BFS( arcGraph, 0, []( int vertex ) { std::cout << vertex << " "; } );

    return 0;
}


