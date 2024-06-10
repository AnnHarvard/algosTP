#include <iostream>
#include <vector>
#include <queue>

class Graph {
public:
    Graph(int vertices) : adjList(vertices), vertices(vertices), edges(0) {}

    void AddEdge(int from, int to) {
        adjList[from].push_back(to);
        adjList[to].push_back(from);
        edges++;
    }

    bool IsTree() {
        if (edges != vertices - 1) {
            return false;
        }

        std::vector<bool> visited(vertices, false);
        if (HasCycleOrDisconnected(0, -1, visited)) {
            return false;
        }

        for (bool v : visited) {
            if (!v) {
                return false;
            }
        }

        return true;
    }

private:
    std::vector<std::vector<int>> adjList;
    int vertices;
    int edges;

    bool HasCycleOrDisconnected(int vertex, int parent, std::vector<bool>& visited) {
        visited[vertex] = true;
        for (int neighbor : adjList[vertex]) {
            if (!visited[neighbor]) {
                if (HasCycleOrDisconnected(neighbor, vertex, visited)) {
                    return true;
                }
            } else if (neighbor != parent) {
                return true;
            }
        }
        return false;
    }
};

int main() {
    int N, M;
    std::cin >> N >> M;

    Graph graph(N);
    for (int i = 0; i < M; ++i) {
        int from, to;
        std::cin >> from >> to;
        graph.AddEdge(from, to);
    }

    std::cout << (graph.IsTree() ? 1 : 0) << std::endl;

    return 0;
}
