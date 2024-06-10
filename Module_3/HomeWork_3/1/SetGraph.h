#ifndef SET_GRAPH_H
#define SET_GRAPH_H

#include "IGraph.h"
#include <unordered_set>
#include <vector>
#include <cassert>

class SetGraph : public IGraph {
public:
    SetGraph( int vertexCount );
    SetGraph( const IGraph& graph );

    virtual void AddEdge( int from, int to ) override;
    virtual int VerticesCount() const override;
    virtual std::vector<int> GetNextVertices( int vertex ) const override;
    virtual std::vector<int> GetPrevVertices( int vertex ) const override;

private:
    std::vector<std::unordered_set<int>> adjSets;
    std::vector<std::unordered_set<int>> prevAdjSets;
};

#endif // SET_GRAPH_H