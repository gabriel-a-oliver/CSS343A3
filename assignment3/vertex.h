#ifndef VERTEX_H
#define VERTEX_H

#include <climits>
#include "vertex.cpp"
#include <functional>
#include <map>
#include <string>

////////////////////////////////////////////////////////////////////////////////
// This is 80 characters - Keep all lines under 80 characters                 //
////////////////////////////////////////////////////////////////////////////////


/** Creates an unvisited vertex, gives it a label, and clears its
    adjacency list.
    NOTE: A vertex must have a unique label that cannot be changed. */
Vertex::Vertex(std::string label) {}

/** @return  The label of this vertex. */
string Vertex::getLabel() const { 
    return this->vertexLabel; 
}

/** Marks this vertex as visited. */
void Vertex::visit() {
    this->visited = true;
}

/** Marks this vertex as not visited. */
void Vertex::unvisit() {
    this->visited = false;
}

/** Returns the visited status of this vertex.
 @return  True if the vertex has been visited, otherwise
    returns false/ */
bool Vertex::isVisited() const { 
    return (this->visited);
}

/** Adds an edge between this vertex and the given vertex.
    Cannot have multiple connections to the same endVertex
    Cannot connect back to itself
 @return  True if the connection is successful. */
bool Vertex::connect(const string& endVertex, const int edgeWeight) { 
    Edge e(endVertex, edgeWeight);
    return false; //default initialization
    //how to handle the connections to itself? connections to same vertex?
}

/** Removes the edge between this vertex and the given one.
@return  True if the removal is successful. */
bool Vertex::disconnect(const std::string& endVertex) { return true; }

/** Gets the weight of the edge between this vertex and the given vertex.
 @return  The edge weight. This value is zero for an unweighted graph and
    is negative if the .edge does not exist */
int Vertex::getEdgeWeight(const std::string& endVertex) const { 
    
}

/** Calculates how many neighbors this vertex has.
 @return  The number of the vertex's neighbors. */
int Vertex::getNumberOfNeighbors() const { return 0; }

/** Sets current neighbor to first in adjacency list. */
void Vertex::resetNeighbor() {}

/** Gets this vertex's next neighbor in the adjacency list.
    Neighbors are automatically sorted alphabetically via map
    Returns the vertex label if there are no more neighbors
 @return  The label of the vertex's next neighbor. */
std::string Vertex::getNextNeighbor() { return "XXX"; }

/** Sees whether this vertex is equal to another one.
    Two vertices are equal if they have the same label. */
bool Vertex::operator==(const Vertex& rightHandItem) const { 
    std::string lhsLabel = this->getLabel();
    std::string rhsLabel = rightHandItem.getLabel();
    return lhsLabel.compare(rhsLabel) == 0;
}

/** Sees whether this vertex is < another one.
    Compares vertexLabel. */
bool Vertex::operator<(const Vertex& rightHandItem) const { 
    std::string lhsLabel = this->getLabel();
    std::string rhsLabel = rightHandItem.getLabel();
    return lhsLabel.compare(rhsLabel) < 0;
}

#endif