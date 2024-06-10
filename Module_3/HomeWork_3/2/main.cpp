// Задача 2. Количество различных путей (3 балла)

// Дан невзвешенный неориентированный граф.
// В графе может быть несколько кратчайших путей между какими-то вершинами. 
// Найдите количество различных кратчайших путей между заданными вершинами. 
// Требования: сложность O(V+E). 
// Формат ввода.
// v: кол-во вершин (макс. 50000),
// n: кол-во ребер (макс. 200000),
// n пар реберных вершин,
// пара вершин u, w для запроса.
// Формат вывода.
// Количество кратчайших путей от u к w.

#include<vector> 
#include<iostream> 
#include<cassert> 
#include<queue> 

using namespace std;
 
struct IGraph { 
    virtual ~IGraph() {} 
         
        // Добавление ребра от from к to. 
    virtual void AddEdge( int from, int to ) = 0; 
 
    virtual int VerticesCount() const  = 0; 
 
    virtual std::vector<int> GetNextVertices( int vertex ) const = 0; 
}; 
 
class ListGraph : public IGraph { 
public:     
    ListGraph( int vertexCount ); 
    ListGraph( const IGraph& graph ); 
 
    //IGraph 
    virtual void AddEdge( int from, int to ) override; 
    virtual int VerticesCount() const override; 
    virtual std::vector<int> GetNextVertices( int vertex ) const override; 
 
private: 
    std::vector<std::vector<int>> adjLists; 
}; 
 
ListGraph::ListGraph( int vertexCount ) 
{ 
    assert( vertexCount > 0 ); 
    adjLists.resize( vertexCount ); 
} 
 
ListGraph::ListGraph( const IGraph& graph )  
{ 
    adjLists.resize( graph.VerticesCount() ); 
    for( int i = 0; i < VerticesCount(); ++i ) { 
        adjLists[i] = graph.GetNextVertices( i ); 
    }    
} 
 
void ListGraph::AddEdge( int from, int to ) 
{ 
    assert( from >= 0 && from < adjLists.size() ); 
    assert( to >= 0 && to < adjLists.size() ); 
    adjLists[from].push_back( to ); 
    adjLists[to].push_back( from );    
} 
 
int ListGraph::VerticesCount() const 
{ 
    return adjLists.size(); 
} 
 
std::vector<int> ListGraph::GetNextVertices( int vertex ) const 
{ 
    assert( vertex >= 0 && vertex < adjLists.size() ); 
    return adjLists[vertex]; 
} 

int countShortestPaths( const IGraph& graph, int u, int w ) {
    int n = graph.VerticesCount();
    vector<int> shortestPaths( n, 0 );
    vector<int> distances( n, -1 );
    vector<bool> visited( n, false );

    queue<int> q;
    q.push( u );
    distances[u] = 0;
    shortestPaths[u] = 1;

    while ( !q.empty() ) {
        int current = q.front();
        q.pop();
        visited[current] = true;

        vector<int> neighbors = graph.GetNextVertices( current );
        for ( int neighbor : neighbors ) {
            if ( !visited[neighbor] ) {
                if ( distances[neighbor] == -1 ) {
                    distances[neighbor] = distances[current] + 1;
                    shortestPaths[neighbor] = shortestPaths[current];
                    q.push( neighbor );
                } else if ( distances[neighbor] == distances[current] + 1 ) {
                    shortestPaths[neighbor] += shortestPaths[current];
                }
            }
        }
    }

    return shortestPaths[w];
}

int main() {
    int v, n;
    cin >> v >> n;
    
    ListGraph graph( v );

    for ( int i = 0; i < n; ++i ) {
        int u, w;
        cin >> u >> w;
        graph.AddEdge( u, w );
    }

    int u, w;
    cin >> u >> w;

    int result = countShortestPaths( graph, u, w );
    cout << result << endl;

    return 0;
}
