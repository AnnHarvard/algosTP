#include "ArcGraph.h"

ArcGraph::ArcGraph( int vertexCount ) : vertexCount( vertexCount ) {
    assert( vertexCount > 0 );
}

ArcGraph::ArcGraph( const IGraph& graph ) : vertexCount( graph.VerticesCount() ) {
    for ( int from = 0; from < vertexCount; ++from ) {
        for ( int to : graph.GetNextVertices( from ) ) {
            edges.emplace_back( from, to );
        }
    }
}

void ArcGraph::AddEdge( int from, int to ) {
    assert( from >= 0 && from < vertexCount );
    assert( to >= 0 && to < vertexCount );
    edges.emplace_back( from, to );
}

int ArcGraph::VerticesCount() const {
    return vertexCount;
}

std::vector<int> ArcGraph::GetNextVertices( int vertex ) const {
    assert( vertex >= 0 );
    std::vector<int> nextVertices;
    for ( const auto& edge : edges ) {
        if ( edge.first == vertex ) {
            nextVertices.push_back( edge.second );
        }
    }
    return nextVertices;
}

std::vector<int> ArcGraph::GetPrevVertices( int vertex ) const {
    assert( vertex >= 0 );
    std::vector<int> prevVertices;
    for ( const auto& edge : edges ) {
        if ( edge.second == vertex ) {
            prevVertices.push_back( edge.first );
        }
    }
    return prevVertices;
}
