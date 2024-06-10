#ifndef ARC_GRAPH_H
#define ARC_GRAPH_H

#include "IGraph.h"
#include <vector>
#include <utility>
#include <cassert>

class ArcGraph : public IGraph {
public:
    ArcGraph( int vertexCount );
    ArcGraph( const IGraph& graph );
    
    virtual void AddEdge( int from, int to ) override;
    virtual int VerticesCount() const override;
    virtual std::vector<int> GetNextVertices( int vertex ) const override;
    virtual std::vector<int> GetPrevVertices( int vertex ) const override;

private:
    int vertexCount;
    std::vector<std::pair<int, int>> edges;
};

#endif // ARC_GRAPH_H