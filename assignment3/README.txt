Assignment 3 was made by Pankhuri Singhal and Gabriel Oliver

08/16/21

To run the program, you must:
1. Open a linux shell and get to the program location
2. Perform the command: g++ -std=c++14 -g -Wall -Wextra *.cpp -o assignment3 > output.txt
3. Perform the command: valgrind ./assignment3 > output.txt
4. You can open output.txt to view the result of the tests found in assignment3.cpp

Main methods and descriptions:


*************VERTEX CLASS:***************************
The class that symbolizes a single vertex of the overall graph and its corresponding adjacency list. Has methods to (un)visit the vertex for search algorithms, connect and disconnect an edge, get the next neighbor, and compare vertices.

  explicit Vertex(std::string label); -- creates a vertex, marks it unvisited, and clears its respective adjacency list.
  
  std::string getLabel() const; -- returns string label of vertex.
  
  void visit(); -- marks vertex as visited
  
  void unvisit(); -- marks vertex as unvisited
  
  bool isVisited() const; -- checks to see if the vertex is visited/not
  
  bool connect(const std::string& endVertex, const int edgeWeight = 0); -- creates an edge between invoking and argument vertex, and places it in adjacency list to "connect" it to this vertex. Checks if there is no connection to itself, aka cycling.
  
  bool disconnect(const std::string& endVertex); -- removes edge between invoking and argument vertex by erasing it from the adjList. Returns true if removal is successful, returns false otherwise or if the edgeconnects to itself.
  
  int getEdgeWeight(const std::string& endVertex); -- returns the weight of the edge between invoking and argument vertices. Returns -1 if the edge is not found in the list.
   
  int getNumberOfNeighbors(); -- returns how many neighbors the vertex has
  
  void resetNeighbor(); -- resets the neighbor to the beginning of the adjList, good for iterating through the graph for the search algorithms.
  
  std::string getNextNeighbor(); -- returns the vertex's next neighbor label in the adjacency list by incrementing the iterator to the next entry in the list. Otherwise if it reaches the end, returns the invoking vertex label.
  
  bool operator==(const Vertex& rightHandItem) const; -- compares two vertex labels to see if they are the same vertex.

  bool operator<(const Vertex& rightHandItem) const; -- compares two vertex labels  to see if this < other vertex.
  
  std::string getCurrentNeighbor(); -- gets the current neighbor of the vertex, otherwise it returns the invoking vertex label if the iterator is at the end of the list.
  

****************EDGE CLASS:*******************************
Represents a single edge of the graph, connecting between two vertices. Has methods to represent weight and the end vertex label.

  Edge(); -- empty edge constructor, sets weight to 0 and end vertex as empty string

  Edge(const std::string& end, int weight); -- constructor to create an edge with a given weight and end vertex label

  std::string getEndVertex() const; -- returns the end vertex that he edge connects to

  int getWeight() const; -- return the weight/cost of travlleing via this edge
  

**********************GRAPH CLASS:********************************
Represents a graph read from a file, with methods to conduct DFS, BFS, and Dijkstra's algorithm to find an optimal path between two vertices. Also checks if vertices and edges are compatible, finds vertices and gets the weight of the edge between the start and end. Also has a destructor to free up memory.

  Graph(); -- creates an empty graph, with the number of edges and vertices set to 0. Initializes a new map for vertices.

  ~Graph(); -- frees up all memory, clears out all maps

  int getNumVertices() const; -- returns number of vertices in graph

  int getNumEdges() const; -- returns number of edges in graph

  bool add(const std::string& start, const std::string& end, int edgeWeight = 0); -- adds new edge between vertices in graph, used to construct graph to use search alg.s on 

  int getEdgeWeight(const std::string& start, const std::string& end) const; --returns weight of edge between two vertices, returns maximum int if the edge does not exist

  void readFile(const std::string& filename); - reads the graph from a text file, taking in number of connections, the vertices and the costs

  void depthFirstTraversal(const std::string& startLabel,
                           void visit(const std::string&)); -- performs recursive DFS from start label, and visits each vertex

  void breadthFirstTraversal(const std::string& startLabel,
                             void visit(const std::string&)); -- performs BFS on the graph, starting from start label, and visits each vertex. uses a queue

  void djikstraCostToAllVertices(
      const std::string& startLabel,
      std::map<std::string, int>& weight,
      std::map<std::string, std::string>& previous); -- calculates cheapest cost from start label to all other vertices of the graph, using Dijkstra's shortest path
      
  void depthFirstTraversalHelper(Vertex* startVertex,
                                void visit(const std::string&)); -- helper for DFS
  
  
 void initializeDjikstraMaps(const std::string& startLabel,
			     std::map<std::string, int>& weight,
			     std::map<std::string, std::string>& previous); -- initilizes default values for maps used in Dijkstra's
                
  
 void djikstraHelper(const std::string& currLabel,
			std::map<std::string, int>& weight,
			std::map<std::string, std::string>& previous,
			int currWeight); -- helper for Dijkstra's, finds shortest path to each vertex 
            
 void unvisitVertices(); -- marks all vertices unvisited, used after each traversal to prep for next traversal
 
 Vertex* findVertex(const std::string& vertexLabel) const; -- finds a vertex in the vertices map, and returns pointer to it for easy access
 
 Vertex* findOrCreateVertex(const std::string& vertexLabel); -- if the vertex from findVertex is not found (if the item returned is nullptr) then create the vertex
 
 bool verticesEdgePairCompatible(const std::string& start, const std::string& end) const; -- determines whether 2 vertices can be used in an operation, like connection or disconnect. Returns true if the two vertices are distinct or if the connection is complete. False otherwise returned.
 
 void resetAllNeighbors(); -- resets all vertices to the beginning of their adjacency list, helps to restart a traversal from the beginning
 
void Graph::clearEverything() -- clears out all memory, including pointers to Vertex in the vertices map, and resets all class data fields to default values.
