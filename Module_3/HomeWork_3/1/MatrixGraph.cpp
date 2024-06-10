#include "MatrixGraph.h"

MatrixGraph::MatrixGraph( int vertexCount ) {
    assert( vertexCount > 0 );
    adjMatrix.resize( vertexCount, std::vector<bool>( vertexCount, false ) );
}

    MatrixGraph::MatrixGraph( const IGraph& graph ) {
        int vertexCount = graph.VerticesCount();
        adjMatrix.resize( vertexCount, std::vector<bool>( vertexCount, false ) );
        for ( int from = 0; from < vertexCount; ++from ) {
            for ( int to : graph.GetNextVertices( from ) ) {
                adjMatrix[from][to] = true;
            }
        }
    }

void MatrixGraph::AddEdge( int from, int to ) {
    assert( from >= 0 && from < adjMatrix.size() );
    assert( to >= 0 && to < adjMatrix.size() );
    adjMatrix[from][to] = true;
}

int MatrixGraph::VerticesCount() const {
    return adjMatrix.size();
}

std::vector<int> MatrixGraph::GetNextVertices( int vertex ) const {
    assert( vertex >= 0 && vertex < adjMatrix.size() );
    std::vector<int> nextVertices;
    for ( int i = 0; i < adjMatrix.size(); ++i ) {
        if ( adjMatrix[vertex][i] ) {
            nextVertices.push_back( i );
        }
    }
    return nextVertices;
}

std::vector<int> MatrixGraph::GetPrevVertices( int vertex ) const {
    assert( vertex >= 0 && vertex < adjMatrix.size() );
    std::vector<int> prevVertices;
    for ( int i = 0; i < adjMatrix.size(); ++i ) {
        if ( adjMatrix[i][vertex] ) {
            prevVertices.push_back( i );
        }
    }
    return prevVertices;
}