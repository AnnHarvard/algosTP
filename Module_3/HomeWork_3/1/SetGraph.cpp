#include "SetGraph.h"

SetGraph::SetGraph( int vertexCount ) {
    assert( vertexCount > 0 );
    adjSets.resize( vertexCount );
    prevAdjSets.resize( vertexCount );
}

SetGraph::SetGraph( const IGraph& graph ) {
    int vertexCount = graph.VerticesCount();
    adjSets.resize( vertexCount );
    prevAdjSets.resize( vertexCount );
    for ( int from = 0; from < vertexCount; ++from ) {
        for ( int to : graph.GetNextVertices( from ) ) {
            adjSets[from].insert( to );
            prevAdjSets[to].insert( from );
        }
    }
}

void SetGraph::AddEdge( int from, int to ) {
    assert( from >= 0 && from < adjSets.size() );
    assert( to >= 0 && to < adjSets.size() );
    adjSets[from].insert( to );
    prevAdjSets[to].insert( from );
}

int SetGraph::VerticesCount() const {
    return adjSets.size();
}

std::vector<int> SetGraph::GetNextVertices( int vertex ) const {
    assert( vertex >= 0 && vertex < adjSets.size() );
    std::vector<int> result;
    for ( int v : adjSets[vertex] ) {
        result.push_back( v );
    }
    return result;
}

std::vector<int> SetGraph::GetPrevVertices( int vertex ) const {
    assert( vertex >= 0 && vertex < prevAdjSets.size() );
    std::vector<int> result;
    for ( int v : prevAdjSets[vertex] ) {
        result.push_back( v );
    }
    return result;
}