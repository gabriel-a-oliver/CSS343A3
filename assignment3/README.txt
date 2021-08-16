Assignment 3 was made by Pankhuri Singhal and Gabriel Oliver

08/16/21

To run the program, you must:
1. Open a linux shell and get to the program location
2. Perform the command: g++ -std=c++14 -g -Wall -Wextra *.cpp -o assignment3 > output.txt
3. Perform the command: valgrind ./assignment3 > output.txt
4. You can open output.txt to view the result of the tests found in assignment3.cpp

Main methods and descriptions:

VERTEX CLASS:
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
  

EDGE CLASS:  



GRAPH CLASS:

