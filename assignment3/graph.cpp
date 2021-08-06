#include <queue>
#include <climits>
#include <set>
#include <iostream>
#include <fstream>
#include <map>

#include "graph.h"

/**
 * A graph is made up of vertices and edges
 * A vertex can be connected to other vertices via weighted, directed edge
 */


////////////////////////////////////////////////////////////////////////////////
// This is 80 characters - Keep all lines under 80 characters                 //
////////////////////////////////////////////////////////////////////////////////


/** constructor, empty graph */
Graph::Graph() {
	vertices = new map<std::string, Vertex*>();
}

/** destructor, delete all vertices and edges
    only vertices stored in map
    no pointers to edges created by graph */
Graph::~Graph() {
	delete vertices;
	/*for (std::map<std::string, Vertex*>::iterator it = Map.begin(); it != Map.end(); ++it) {
		delete it;
	}*/
	vertices = nullptr;
}

/** return number of vertices */
int Graph::getNumVertices() const { return vertices.size(); }

/** return number of vertices */
int Graph::getNumEdges() const { return 0; }

/** add a new edge between start and end vertex
    if the vertices do not exist, create them
    calls Vertex::connect
    a vertex cannot connect to itself
    or have multiple edges to another vertex */
bool Graph::add(std::string start, std::string end, int edgeWeight) {
	bool compatible = verticesEdgePairCompatible(start, end);
	if (!compatible) {
		return false;
	}
	/*if (start.compare(end)) {
		cout << "The start and end vertices cannot be the same" << endl;
		return false;
	}
	// Also make sure there isnt already an edge for this pair!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
	if (map.find(start) == map.end()) {
		map.add(start, new Vertex(start));
	}
	if (map.find(end) == map.end()) {
		map.add(end, new Vertex(end));
	}
	map.find(start).connect(map.find(end), edgeWeight);
	return true;
}

/** return weight of the edge between start and end
    returns INT_MAX if not connected or vertices don't exist */
int Graph::getEdgeWeight(std::string start, std::string end) const {
	Vertex* startVert = map.find(start);
	Vertex* endVert = map.find(endVert);
	bool compatible = verticesEdgePairCompatible(start, end);
	if (!compatible) {
		return INT_MAX;
	}
	return startVert.getEdgeWeight(endVert);
}

/** read edges from file
    the first line of the file is an integer, indicating number of edges
    each edge line is in the form of "string string int"
    fromVertex  toVertex    edgeWeight */
void Graph::readFile(std::string filename) {
	delete map;
	map = new Map<std::string, Vertex*>;
	delete vertices;
	vertices = new List<Vertex*>();
	visitedVertList.clear();
	// Before reading file, clear all previous information !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	ofstream graphFile;
	graphFile.open("filename");
	string numOfEdges;
	getline(graphFile, numOfEdges);
	numberOfEdges = (int)numOfEdges;

	for (int i = 0; i < numberOfEdges; i++) {
		stringstream currentLine;
		getline(graphFile, currentLine);

		add(currentLine[0], currentLine[1], (int)currentLine[2]);
	}
	graphFile.close();
}

/** depth-first traversal starting from startLabel
    call the function visit on each vertex label */
void Graph::depthFirstTraversal(std::string startLabel,
                                void visit(const std::string&)) {
	unvisitVertices();
	visitedVertList.clear();
	delete visitedVertList;
	visitedVertList = new list<Vertices>;
	Vertex* startingVertex = findVertex();

	depthFirstTraversalHelper(startingVertex, visit)
	/*
		Mark all nodes as unvisited
		call dfsHelper with startVertex


	*/
	unvisitVertices();
	visitedVertList.clear();
	delete visitedVertList;
}

/** breadth-first traversal starting from startLabel
    call the function visit on each vertex label */
void Graph::breadthFirstTraversal(std::string startLabel,
                                  void visit(const std::string&)) {
	unvisitVertices();
	visitedVertList.clear();
	delete visitedVertList;
	visitedVertList = new list<Vertices>;
	Vertex* startingVertex = findVertex();

	bfsQueue.push(startingVertex);

	visit(startingVertex.getLabel());
	startingVertex.visit();
	visitedVertList.push_back(startingVertex);

	while (!bfsQueue.empty()) {
		Vertex* currentVert = bfsQueue.pop();
		for (Vertex* nextNeighbor = currentVert.getNextNeighbor(); nextNeighbor
			 != currentVert.getLabel(); nextNeighbor = currentVert.getNextNeighbor) {
			if (!nextNeighbor.isVisited()) {
				visit(currentVert.getLabel());
				currentVert.visit();
				visitedVertList.push_back();
				bfsQueue.push(nextNeighbor);
			}
		}
	}

	/*
	Mark all nodes as unvisited
	enqueue startVertex to queue
	mark startVertex as visited
	while queue is not empty
 		w = dequeue
 		for each unvisited neighbor u of w
 			visit u
 			enqueue u
	Starting from O gives the following visiting order:
	O P Q R S T U
	*/

	unvisitVertices();
	visitedVertList.clear();
	delete visitedVertList;
}

/** find the lowest cost from startLabel to all vertices that can be reached
    using Djikstra's shortest-path algorithm
    record costs in the given map weight
    weight["F"] = 10 indicates the cost to get to "F" is 10
    record the shortest path to each vertex using given map previous
    previous["F"] = "C" indicates get to "F" via "C"

    cpplint gives warning to use pointer instead of a non-const map
    which I am ignoring for readability */

/*
Help for this function:
	auto cmp = [&](std::string a, std::string b) { return weight[a] > weight[b]; };
	std::priority_queue<std::string, std::vector<std::string>, decltype(cmp)> pq(cmp);
*/
void Graph::djikstraCostToAllVertices(
    std::string startLabel,
    std::map<std::string, int>& weight,
    std::map<std::string, std::string>& previous) {

}

/** helper for depthFirstTraversal */
void Graph::depthFirstTraversalHelper(Vertex* startVertex,
                                      void visit(const std::string&)) {
	string startVertexLabel = startVertex.getLabel();
	visit(startVertexLabel);
	startVertex.visit();
	visitedVertList.push_back(startVertex);

	for (Vertex* nextNeighbor = startVertex.getNextNeighbor(); nextNeighbor !=
			startVertexLabel; nextNeighbor = startVertex.getNextNeighbor) {
		if (!nextNeighbor.isVisited()) {
			depthFirstTraversalHelper(nextNeighbor, visit)
		}
	}

	/*
	dfsHelper: vertex
		visit vertex
		for each neighbour, getNextNeighbor of vertex as n
			if n is not visited
				call dfsHelper with n
	*/
}

/** helper for breadthFirstTraversal */
void Graph::breadthFirstTraversalHelper(Vertex*startVertex,
                                        void visit(const std::string&)) {}

/** mark all verticies as unvisited */
void Graph::unvisitVertices() {
	for (Vertex* vert : visitedVertList) {
		vert.unvisit();
	}
}

/** find a vertex, if it does not exist return nullptr */
Vertex* Graph::findVertex(const std::string& vertexLabel) const { return nullptr; }

/** find a vertex, if it does not exist create it and return it */
Vertex* Graph::findOrCreateVertex(const std::string& vertexLabel) { return nullptr; }

//Gabe code start
bool Graph::verticesEdgePairCompatible(std::string start, std::string end) const {
	if (start.compare(end)) {
		cout << "The start and end vertices cannot be the same" << endl;
		return false;
	}
	// Also make sure there isnt already an edge for this pair!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	return true;
}
//Gabe code end