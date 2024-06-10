#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

class Graph {
public:
    Graph(int vertices) : adjList(vertices), vertices(vertices) {}

    void AddEdge(int from, int to) {
        adjList[from].push_back(to);
        adjList[to].push_back(from);
    }

    bool IsEulerian() {
        if (!IsConnected()) {
            return false;
        }

        for (int i = 0; i < vertices; ++i) {
            if (adjList[i].size() % 2 != 0) {
                return false;
            }
        }

        return true;
    }

private:
    std::vector<std::vector<int>> adjList;
    int vertices;

    bool IsConnected() {
        std::vector<bool> visited(vertices, false);
        std::queue<int> q;

        int startVertex = -1;
        for (int i = 0; i < vertices; ++i) {
            if (!adjList[i].empty()) {
                startVertex = i;
                break;
            }
        }

        if (startVertex == -1) {
            return true;
        }

        q.push(startVertex);
        visited[startVertex] = true;
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (int neighbor : adjList[v]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }

        for (int i = 0; i < vertices; ++i) {
            if (!adjList[i].empty() && !visited[i]) {
                return false;
            }
        }

        return true;
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

    std::cout << (graph.IsEulerian() ? 1 : 0) << std::endl;

    return 0;
}
