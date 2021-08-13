#include <climits>

#include "vertex.h"


#include <functional>
#include <map>
#include <iterator>
#include <string>
#include <set>

#include "edge.h"
using namespace std;



////////////////////////////////////////////////////////////////////////////////
// This is 80 characters - Keep all lines under 80 characters                 //
////////////////////////////////////////////////////////////////////////////////


/** Creates an unvisited vertex, gives it a label, and clears its
    adjacency list.
    NOTE: A vertex must have a unique label that cannot be changed. */
Vertex::Vertex(std::string label) {
    this->vertexLabel = label;
    this->visited = false;
    adjacencyList.clear();

}

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
    adjacencyList.emplace(this->vertexLabel, e);
    //iterate w/iterator, check if this vertexlbl.second
    //maybe put this up top?
    map<std::string, Edge>::iterator it;
    for (it = adjacencyList.begin(); it != adjacencyList.end(); it++) {
        if (it->second.getEndVertex() == this->vertexLabel) {
            //connection to self - like A->A
            return false;
        }
    }
    return true; //default initialization
    //how to handle the connections to itself? connections to same vertex?
}

/** Removes the edge between this vertex and the given one.
@return  True if the removal is successful. */
bool Vertex::disconnect(const std::string& endVertex) { 
    //first check endvert != this vertex
    //run thru , iterate, grab edge, delete
    if (this->vertexLabel != endVertex) { //edge case: same edge to edge
        map<std::string, Edge>::iterator it;
        for (it = adjacencyList.begin(); it != adjacencyList.end(); it++) {
            string labelTemp = it->first;
            Edge eTemp = it->second;
            if (labelTemp == endVertex) {
                adjacencyList.erase(labelTemp);
                return true;
            }
        }
        return false; //can't find vertex
    }
    else {
        return false;
    }
    //remove the element
}

/** Gets the weight of the edge between this vertex and the given vertex.
 @return  The edge weight. This value is zero for an unweighted graph and
    is negative if the .edge does not exist */
int Vertex::getEdgeWeight(const std::string& endVertex)  { 
    //say this = D, end = F, get weight of 10
    //itreate!
    /*if (endVertex == this->vertexLabel) {
        return -1;
    }*/

    map<std::string, Edge>::iterator it;
    for (it = adjacencyList.begin(); it != adjacencyList.end(); it++) {
        if (it->second.getEndVertex() == endVertex) {
            return it->second.getWeight();
        }
    }
    return -1; //edge cannot be found
}

/** Calculates how many neighbors this vertex has.
 @return  The number of the vertex's neighbors. */
int Vertex::getNumberOfNeighbors() { 
    //iterate thru list, create vector of neighbors
    //everytime you see a element with this->vertexLabel as its starting vertex,
    //pushback it's second.endVert into the vector
    //then count the unique/distinct vertices
    //OR, USE A SET!!!! DOESN'T ALLOW DUPES ANYWAY
    set<string> distinctVerts;
    map<std::string, Edge>::iterator it;
    //maybe have a check to see if adjlist is empty or not here?
    for (it = adjacencyList.begin(); it != adjacencyList.end(); it++) {
        if (it->first.compare(this->getLabel()) == 0) {
            distinctVerts.insert(it->second.getEndVertex()); //gathers up distinct neighbors
        }
    }

    return distinctVerts.size();
}

/** Sets current neighbor to first in adjacency list. */
void Vertex::resetNeighbor() {
    //currNeighbor = adjlist.begin
    currentNeighbor = adjacencyList.begin();
}

/** Gets this vertex's next neighbor in the adjacency list.
    Neighbors are automatically sorted alphabetically via map
    Returns the vertex label if there are no more neighbors
 @return  The label of the vertex's next neighbor. */
std::string Vertex::getNextNeighbor() { 
    resetNeighbor();
    if (currentNeighbor != adjacencyList.end()) {
        currentNeighbor++;
        string currLbl = currentNeighbor->first;
        return currLbl;
    }
    else {
        return this->getLabel();
    }
}

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

