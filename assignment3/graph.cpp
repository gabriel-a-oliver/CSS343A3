#include <queue>
#include <climits>
#include <set>
#include <iostream>
#include <fstream>
#include <map>
#include <list>
#include <vector>

#include "graph.h"

using namespace std;
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
int Graph::getNumVertices() const { return vertices->size(); }

/** return number of edges */
int Graph::getNumEdges() const { return numberOfEdges; }

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
	if (vertices->find(start) == vertices->end()) {
		vertices->insert(pair<string, Vertex*>(start, new Vertex(start)));
	}
	if (vertices->find(end)->second == vertices->end()->second) {
		vertices->insert(pair<string, Vertex*>(end, new Vertex(end)));
	}
	vertices->find(start)->second->connect(vertices->find(end)->first, edgeWeight);
	return true;
}

/** return weight of the edge between start and end
    returns INT_MAX if not connected or vertices don't exist */
int Graph::getEdgeWeight(std::string start, std::string end) const {
	Vertex* startVert = vertices->find(start)->second;
	Vertex* endVert = vertices->find(end)->second;
	bool compatible = verticesEdgePairCompatible(start, end);
	if (!compatible) {
		return INT_MAX;
	}
	return startVert->getEdgeWeight(vertices->find(end)->first);
}

/** read edges from file
    the first line of the file is an integer, indicating number of edges
    each edge line is in the form of "string string int"
    fromVertex  toVertex    edgeWeight */
void Graph::readFile(std::string filename) {
	delete vertices;
	vertices = new map<std::string, Vertex*>;

	std::ifstream graphFile;
	graphFile.open(filename);
	if (!(graphFile.is_open())) {
		cout << "File " << filename << " does not exist. It opened: " << (graphFile.is_open()) << endl;
		return;
	}

	string numOfEdgesStr;
	getline(graphFile, numOfEdgesStr);
	numOfEdges = std::stoi(numOfEdgesStr);

	//help from: https://stackoverflow.com/questions/32898558/c-read-in-input-one-word-at-a-time////////////////////////////////////////////////////////////////////
	for (int i = 0; i < numOfEdges; i++) {
		std::vector<std::string> elements;

		std::string connection;
		getline(graphFile, connection);

		std::istringstream str(connection);
		std::string element;
		while (str >> element) {
			elements.push_back(element);
		}

		add(elements[0], elements[1], std::stoi(elements[2]));
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


	/*for (int i = 0; i < numOfEdges; i++) {
		stringstream currentLine;
		getline(graphFile, currentLine);

		add(currentLine[0], currentLine[1], (int)currentLine[2]);
	}*/
	graphFile.close();
}

/** depth-first traversal starting from startLabel
    call the function visit on each vertex label */
void Graph::depthFirstTraversal(std::string startLabel,
                                void visit(const std::string&)) {
	unvisitVertices();
	list <Vertex*> visitedVertList;
	Vertex* startingVertex = findVertex(startLabel);

	depthFirstTraversalHelper(startingVertex, visit, visitedVertList);
	/*
		Mark all nodes as unvisited
		call dfsHelper with startVertex
	*/
	unvisitVertices();
	visitedVertList.clear();
}

/** breadth-first traversal starting from startLabel
    call the function visit on each vertex label */
void Graph::breadthFirstTraversal(std::string startLabel,
                                  void visit(const std::string&)) {
	unvisitVertices();
	list<Vertex*> visitedVertList;
	queue<Vertex*> bfsQueue;

	Vertex* startingVertex = findVertex(startLabel);
	bfsQueue.push(startingVertex);

	visit(startingVertex->getLabel());
	startingVertex->visit();
	visitedVertList.push_back(startingVertex);

	while (!bfsQueue.empty()) {
		Vertex* currentVert = bfsQueue.front();
		for (Vertex* nextNeighbor = findVertex(currentVert->getNextNeighbor());
			 nextNeighbor != currentVert;
			 nextNeighbor = findVertex(currentVert->getNextNeighbor())) {
			if (!nextNeighbor->isVisited()) {
				visit(currentVert->getLabel());
				currentVert->visit();
				visitedVertList.push_back(currentVert);
				bfsQueue.push(nextNeighbor);
			}
		}
		bfsQueue.pop(); // this may call the destructor ??!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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

	if (vertices->empty()){
		cout << "Graph is Empty" << endl;
		return;
	}

	weight.clear();
	previous.clear();

	vector<Vertex*> vertVector;
	/*auto comparison = [&](std::string firstLabel, std::string secondLabel) {
		return weight[firstLabel] > weight[secondLabel];
	};
	std::priority_queue<string, std::vector<string>, decltype(comparison)>
			orderedVerticesPQueue(comparison);*/

	for (pair<string, Vertex*> currPair : *vertices) {
		vertVector.push_back(currPair.second);
		weight.insert(pair<string, int>(currPair.first, INT_MAX));
		previous.insert(pair<string, string>(currPair.first, nullptr));
		//orderedVerticesPQueue.push();
	}

	unvisitVertices();
	//list<Vertex*> vertList;
	Vertex* startVertex = findVertex(startLabel);
	//vertList.push_back(startVertex);
	//djikstraListBuilderHelper(startVertex, vertList);
	//unvisitVertices();

	weight.find(startLabel)->second = 0;
	previous.find(startLabel)->second = nullptr;
	startVertex->visit();
	//vertList.push_back(startVertex);

	//string currLabel = startLabel;
	djikstraHelper(startLabel, weight, previous, 0);

	/* Gabe Psuedocode
	pick vertex
	for each neighbor
		if unvisited
	 		add to visitedList with cost + cost of current vert
		else
	 		compare costs
	 		if new cost is cheaper
	 			update cost
	 			recurse with curr neighbor
	*/


	vertVector.clear();
	//vertList.clear();
	/*while (!orderedVerticesPQueue.empty()) {
		orderedVerticesPQueue.pop();
	}*/
}

void Graph::djikstraListBuilderHelper(Vertex* currVert, std::list<Vertex*> vertList) {
	for (Vertex* nextNeighbor = currVert; currVert->getNextNeighbor() != currVert->getLabel(); nextNeighbor = findVertex(currVert->getNextNeighbor())) {
		if (!nextNeighbor->isVisited()) {
			nextNeighbor->visit();
			vertList.push_back(nextNeighbor);
			djikstraListBuilderHelper(nextNeighbor, vertList);
		}
	}
}

void Graph::djikstraHelper(std::string currLabel,
						   std::map<std::string, int>& weight,
						   std::map<std::string, std::string>& previous,
						   int currWeight) {
	/*if (vertList.empty()) {
		return;
	}*/

	Vertex* currVert = findVertex(currLabel);
	/*if (!currVert->isVisited()) {
		currVert->visit();
		//vertList.remove(currVert);
	}*/

	for (Vertex* nextNeighbor = findVertex(currVert->getNextNeighbor());
		 nextNeighbor->getNextNeighbor() != currLabel;
		 nextNeighbor = findVertex(currVert->getNextNeighbor())) {
		if (!nextNeighbor->isVisited()) {
			//vertList.remove(nextNeighbor);
			nextNeighbor->visit();
			weight.find(nextNeighbor->getLabel())->second =
					currVert->getEdgeWeight(nextNeighbor->getLabel())
					+ currWeight;
			previous.find(nextNeighbor->getLabel())->second = currLabel;
		}
		else {
			if (currWeight + currVert->getEdgeWeight(nextNeighbor->getLabel())
					<= weight.find(nextNeighbor->getLabel())->second) {
				weight.find(nextNeighbor->getLabel())->second = currWeight +
							currVert->getEdgeWeight(nextNeighbor->getLabel());
				previous.find(nextNeighbor->getLabel())->second = currLabel;
			}
			else {
				if (nextNeighbor->getNextNeighbor() == currLabel) {
					return;
				}
			}
		}
		djikstraHelper(nextNeighbor->getLabel(), weight, previous,
					   currWeight + currVert->getEdgeWeight(
						nextNeighbor->getLabel()));
	}
}

/** helper for depthFirstTraversal */
void Graph::depthFirstTraversalHelper(Vertex* startVertex,
									  void visit(const std::string&),
									  std::list<Vertex*> visitedVertList) {
	string startVertexLabel = startVertex->getLabel();
	visit(startVertexLabel);
	startVertex->visit();
	visitedVertList.push_back(startVertex);

	for (Vertex* nextNeighbor = findVertex(startVertex->getNextNeighbor());
		 nextNeighbor->getLabel() != startVertexLabel;
		 nextNeighbor = findVertex(startVertex->getNextNeighbor())) {
		if (!nextNeighbor->isVisited()) {
			depthFirstTraversalHelper(nextNeighbor, visit, visitedVertList);
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
/*void Graph::breadthFirstTraversalHelper(Vertex*startVertex,
                                        void visit(const std::string&)) {}*/

/** mark all verticies as unvisited */
void Graph::unvisitVertices() {
	for (pair<string, Vertex*> pair : *vertices) {
		pair.second->unvisit();
	}
}

/** find a vertex, if it does not exist return nullptr */
Vertex* Graph::findVertex(const std::string& vertexLabel) const {
	return vertices->find(vertexLabel)->second;
}

/** find a vertex, if it does not exist create it and return it */
Vertex* Graph::findOrCreateVertex(const std::string& vertexLabel) {
	Vertex* result = vertices->find(vertexLabel)->second;
	if (result == nullptr) {
		result = new Vertex(vertexLabel);
		vertices->insert(pair<string, Vertex*>(vertexLabel, result));
	}
	return result;
}

//Gabe code start
bool Graph::verticesEdgePairCompatible(std::string start, std::string end) const {
	if (start.compare(end)) {
		cout << "The start and end vertices cannot be the same" << endl;
		return false;
	}
	Vertex* startVert = findVertex(start);
	Vertex* endVert = findVertex(end);
	for (Vertex* nextNeighbor = startVert;
		 nextNeighbor->getLabel() != start;
		 nextNeighbor = findVertex(nextNeighbor->getNextNeighbor())) {
		if (nextNeighbor == endVert) {
			return false;
		}
	}
	// Also make sure there isnt already an edge for this pair!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	return true;
}

