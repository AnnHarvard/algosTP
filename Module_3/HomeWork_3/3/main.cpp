// Задача 3. «Города» (4 балла)

// Требуется отыскать самый выгодный маршрут между городами. 
// Требования: время работы O((N+M)logN), где N-количество городов, M-известных дорог между ними.
// Формат входных данных.
// Первая строка содержит число N – количество городов.
// Вторая строка содержит число M - количество дорог.
// Каждая следующая строка содержит описание дороги (откуда, куда, время в пути).
// Последняя строка содержит маршрут (откуда и куда нужно доехать).
// Формат выходных данных.
// Вывести длину самого выгодного маршрута.

#include <iostream>
#include <vector>
#include <set>
#include <limits>

const int INF = std::numeric_limits<int>::max();

class ListGraph {
public:
    explicit ListGraph( int vertexCount );
    void AddEdge( int from, int to, int weight );
    int VerticesCount() const;
    std::vector<std::pair<int, int>> GetNextVertices( int vertex ) const;

private:
    std::vector<std::vector<std::pair<int, int>>> adjLists;
};

ListGraph::ListGraph( int vertexCount ) {
    adjLists.resize( vertexCount );
}

void ListGraph::AddEdge( int from, int to, int weight ) {
    adjLists[from].emplace_back( to, weight );
}

int ListGraph::VerticesCount() const {
    return adjLists.size();
}

std::vector<std::pair<int, int>> ListGraph::GetNextVertices( int vertex ) const {
    return adjLists[vertex];
}

std::vector<int> Dijkstra( const ListGraph& graph, int start ) {
    int vertexCount = graph.VerticesCount();
    std::vector<int> distances( vertexCount, INF );
    std::set<std::pair<int, int>> activeVertices;

    distances[start] = 0;
    activeVertices.insert( {0, start} );

    while ( !activeVertices.empty() ) {
        int u = activeVertices.begin()->second;
        activeVertices.erase( activeVertices.begin() );

        for ( const auto& edge : graph.GetNextVertices( u ) ) {
            int v = edge.first;
            int weight = edge.second;
            if ( distances[u] + weight < distances[v] ) {
                activeVertices.erase( {distances[v], v} );
                distances[v] = distances[u] + weight;
                activeVertices.insert( {distances[v], v} );
            }
        }
    }

    return distances;
}

int main() {
    int N, M;
    std::cin >> N >> M;

    ListGraph graph( N );

    for ( int i = 0; i < M; ++i ) {
        int from, to, weight;
        std::cin >> from >> to >> weight;
        graph.AddEdge( from, to, weight );
        graph.AddEdge( to, from, weight );
    }

    int start, end;
    std::cin >> start >> end;

    std::vector<int> distances = Dijkstra( graph, start );

    if ( distances[end] == INF ) {
        std::cout << "No path from " << start << " to " << end << std::endl;
    } else {
        std::cout << distances[end] << std::endl;
    }

    return 0;
}
