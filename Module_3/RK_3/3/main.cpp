#include <iostream>
#include <vector>
#include <utility>
#include <limits>
#include <cassert>

class IGraph {
public:
    virtual void AddEdge(int from, int to, int cost) = 0;
    virtual int VerticesCount() const = 0;
    virtual std::vector<std::pair<int, int>> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
    virtual ~IGraph() {}
};

class ArcGraph : public IGraph {
public:
    ArcGraph(int vertexCount) : vertexCount(vertexCount) {}
    ArcGraph(const IGraph& graph) : vertexCount(graph.VerticesCount()) {
        for (int i = 0; i < graph.VerticesCount(); ++i) {
            for (auto& edge : graph.GetNextVertices(i)) {
                AddEdge(i, edge.first, edge.second);
            }
        }
    }
    
    void AddEdge(int from, int to, int cost) override {
        assert(from >= 0 && from < vertexCount);
        assert(to >= 0 && to < vertexCount);
        edges.push_back({cost, {from, to}});
    }
    
    int VerticesCount() const override {
        return vertexCount;
    }
    
    std::vector<std::pair<int, int>> GetNextVertices(int vertex) const override {
        std::vector<std::pair<int, int>> result;
        for (const auto& edge : edges) {
            if (edge.second.first == vertex) {
                result.push_back({edge.second.second, edge.first});
            }
        }
        return result;
    }
    
    std::vector<int> GetPrevVertices(int vertex) const override {
        std::vector<int> result;
        for (const auto& edge : edges) {
            if (edge.second.second == vertex) {
                result.push_back(edge.second.first);
            }
        }
        return result;
    }

    const std::vector<std::pair<int, std::pair<int, int>>>& GetEdges() const {
        return edges;
    }

private:
    int vertexCount;
    std::vector<std::pair<int, std::pair<int, int>>> edges; // {cost, {from, to}}
};

const int INF = std::numeric_limits<int>::max();

class RickPathFinder {
public:
    RickPathFinder(const ArcGraph& graph, int k, int start, int finish) 
        : graph(graph), K(k), S(start - 1), F(finish - 1), dp(k + 1, std::vector<int>(graph.VerticesCount(), INF)) {
        dp[0][S] = 0;
    }
    int findMinCostPath() {
        for (int i = 0; i < K; ++i) {
            for (const auto& edge : graph.GetEdges()) {
                int u = edge.second.first;
                int v = edge.second.second;
                int cost = edge.first;
                if (dp[i][u] != INF) {
                    dp[i + 1][v] = std::min(dp[i + 1][v], dp[i][u] + cost);
                }
            }
        }
        int result = INF;
        for (int i = 0; i <= K; ++i) {
            result = std::min(result, dp[i][F]);
        }
        return (result == INF) ? -1 : result;
    }
private:
    const ArcGraph& graph;
    int K;
    int S;
    int F;
    std::vector<std::vector<int>> dp;
};

int main() {
    int N, M, K, S, F;
    std::cin >> N >> M >> K >> S >> F;

    ArcGraph graph(N);

    for (int i = 0; i < M; ++i) {
        int Si, Fi, Pi;
        std::cin >> Si >> Fi >> Pi;
        graph.AddEdge(Si - 1, Fi - 1, Pi);
    }

    RickPathFinder pathFinder(graph, K, S, F);
    int result = pathFinder.findMinCostPath();
    std::cout << result << std::endl;

    return 0;
}
