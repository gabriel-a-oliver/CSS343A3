
#include <queue>
#include <climits>
#include <iostream>
#include <fstream>
#include <map>
#include <list>
#include <vector>


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
	numberOfEdges = 0;
	numberOfVertices = 0;
}

/** destructor, delete all vertices and edges
    only vertices stored in map
    no pointers to edges created by graph */
Graph::~Graph() {
	delete vertices;
	vertices = nullptr;
	numberOfVertices = 0;
	numberOfEdges = 0;
}

/** return number of vertices */
int Graph::getNumVertices() const { return numberOfVertices; }

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
	vertices->insert(pair<string, Vertex*>(start, new Vertex(start)));
	vertices->insert(pair<string, Vertex*>(end, new Vertex(end)));
	findVertex(start)->connect(end, edgeWeight);
	numberOfVertices = static_cast<int>(vertices->size());
	numberOfEdges++;
	return true;
}

/** return weight of the edge between start and end
    returns INT_MAX if not connected or vertices don't exist */
int Graph::getEdgeWeight(std::string start, std::string end) const {
	Vertex* startVert = findVertex(start);
	Vertex* endVert = findVertex(end);
	if (start == end) {
		cout << start << " is the same as " << end << "." << endl;
		return 0;
	}
	for (Vertex* nextNeighbor = startVert;
	nextNeighbor->getLabel() != start;
	nextNeighbor = findVertex(nextNeighbor->getNextNeighbor())) {
		if (nextNeighbor == endVert) {
			return startVert->getEdgeWeight(end);
		}
	}
	cout << "There is no connection starting from " << start
			<< " and ending at " << end << "." << endl;
	return INT_MAX;
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
		cout << "File " << filename << " cannot be opened." << endl;
		return;
	}

	string numOfEdgesStr;
	getline(graphFile, numOfEdgesStr);
	int tempNumOfEdges = std::stoi(numOfEdgesStr);

	for (int i = 0; i < tempNumOfEdges; i++) {

		std::string connection;
		getline(graphFile, connection);

		std::istringstream str(connection);
		std::string firstVertStr;
		std::string endVertStr;
		std::string weightStr;

		str >> firstVertStr >> endVertStr >> weightStr;

		add(firstVertStr, endVertStr, std::stoi(weightStr));
	}

	graphFile.close();
}

/** depth-first traversal starting from startLabel
    call the function visit on each vertex label */
void Graph::depthFirstTraversal(std::string startLabel,
                                void visit(const std::string&)) {
	if (vertices->empty()) {
		cout << "Graph is empty." << endl;
		return;
	}

	unvisitVertices();
	resetAllNeighbors();

	list <Vertex*> visitedVertList;
	Vertex* startingVertex = findVertex(startLabel);

	depthFirstTraversalHelper(startingVertex, visit, visitedVertList);
	unvisitVertices();
	visitedVertList.clear();
}

/** breadth-first traversal starting from startLabel
    call the function visit on each vertex label */
void Graph::breadthFirstTraversal(std::string startLabel,
                                  void visit(const std::string&)) {
	if (vertices->empty()) {
		cout << "Graph is empty." << endl;
		return;
	}

	unvisitVertices();
	resetAllNeighbors();

	list<Vertex*> visitedVertList;
	queue<Vertex*> bfsQueue;

	Vertex* startingVertex = findVertex(startLabel);
	startingVertex->resetNeighbor();

	bfsQueue.push(startingVertex);
	//visit(startLabel);
	//startingVertex->visit();

	//visitedVertList.push_back(startingVertex);

	while (!bfsQueue.empty()) {
		Vertex* currentVert = bfsQueue.front();
		if (!currentVert->isVisited()) {
			visit(currentVert->getLabel());
			currentVert->visit();
			visitedVertList.push_back(currentVert);
		}
		for (Vertex* nextNeighbor = findVertex(currentVert->getCurrentNeighbor());
			 nextNeighbor != currentVert;
			 nextNeighbor = findVertex(currentVert->getNextNeighbor())) {
			if (!nextNeighbor->isVisited()) {
				visit(nextNeighbor->getLabel());
				nextNeighbor->visit();
				visitedVertList.push_back(nextNeighbor);
				bfsQueue.push(nextNeighbor);
			}
		}
		bfsQueue.pop();
	}
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
	unvisitVertices();
	resetAllNeighbors();

	vector<Vertex*> vertVector;

	initializeDjikstraMaps(startLabel, weight, previous, vertVector);

	Vertex* startVertex = findVertex(startLabel);

	//weight.find(startLabel)->second = 0;
	//previous.find(startLabel)->second = "";
	startVertex->visit();

	djikstraHelper(startLabel, weight, previous, 0);

	vertVector.clear();
}

void Graph::djikstraHelper(std::string currLabel,
						   std::map<std::string, int>& weight,
						   std::map<std::string, std::string>& previous,
						   int currWeight) {
	Vertex* currVert = findVertex(currLabel);

	for (Vertex* nextNeighbor = findVertex(currVert->getNextNeighbor());
		 nextNeighbor->getCurrentNeighbor() != currLabel;
		 nextNeighbor = findVertex(currVert->getNextNeighbor())) {
		if (!nextNeighbor->isVisited()) {
			nextNeighbor->visit();
			weight.find(nextNeighbor->getLabel())->second =
					currVert->getEdgeWeight(nextNeighbor->getLabel())
					+ currWeight;
			previous.find(nextNeighbor->getLabel())->second = currLabel;
		}
		else {
			if (currWeight + currVert->getEdgeWeight(nextNeighbor->getLabel())
					< weight.find(nextNeighbor->getLabel())->second) {
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

void Graph::initializeDjikstraMaps(std::string startLabel,
								   std::map<std::string, int>& weight,
								   std::map<std::string, std::string>& previous,
								   std::vector<Vertex*> vertVector) {
	for (pair<string, Vertex*> currPair : *vertices) {
		if (currPair.first != startLabel) {
			vertVector.push_back(currPair.second);
			weight.insert(pair<string, int>(currPair.first, INT_MAX));
			previous.insert(pair<string, string>(currPair.first, ""));
		}
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
	startVertex->resetNeighbor();
	// need to use a neighbor iterator /////////////////////////////////////////////////////////////////////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	for (Vertex* nextNeighbor = findVertex(startVertex->getCurrentNeighbor());
		 nextNeighbor->getLabel() != startVertexLabel;
		 nextNeighbor = findVertex(startVertex->getNextNeighbor())) /*this is causing issues*/{ ////////////////////////////////////////////////////!!!!!!!
		if (!nextNeighbor->isVisited()) {
			depthFirstTraversalHelper(nextNeighbor, visit, visitedVertList);
		}
	}
}

/** mark all verticies as unvisited */
void Graph::unvisitVertices() {
	for (pair<string, Vertex*> pair : *vertices) {
		pair.second->unvisit();
	}
}

/** find a vertex, if it does not exist return nullptr */
Vertex* Graph::findVertex(const std::string& vertexLabel) const {
	_Rb_tree_iterator<pair<const string, Vertex *>> result =
			vertices->find(vertexLabel);
	if (result == vertices->end()) {
		//cout << "Vertex " << vertexLabel << " does not exist." << endl;
		return nullptr;
	}
	return result->second;
}

/** find a vertex, if it does not exist create it and return it */
Vertex* Graph::findOrCreateVertex(const std::string& vertexLabel) {
	Vertex* result = findVertex(vertexLabel);
	if (result == nullptr) {
		result = new Vertex(vertexLabel);
		vertices->insert(pair<string, Vertex*>(vertexLabel, result));
	}
	return result;
}

bool Graph::verticesEdgePairCompatible(std::string start,
									   std::string end) const {
	if (start == end) {
		cout << "The start and end vertices cannot be the same." << endl;
		return false;
	}
	if (vertices->empty()) {
		return true;
	}
	Vertex* startVert = findVertex(start);
	Vertex* endVert = findVertex(end);
	if (startVert == nullptr || endVert == nullptr) {
		return true;
	}
	for (Vertex* nextNeighbor = startVert;
		 nextNeighbor->getLabel() != start;
		 nextNeighbor = findVertex(nextNeighbor->getNextNeighbor())) {
		if (nextNeighbor == endVert) {
			cout << "An edge from " << start << " to " << end
					<< " already exists." << endl;
			return false;
		}
	}
	return true;
}

void Graph::resetAllNeighbors() {
	for (pair<string, Vertex*> currPair : *vertices) {
		currPair.second->resetNeighbor();
	}
}