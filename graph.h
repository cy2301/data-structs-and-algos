#ifndef GRAPH_H
#define GRAPH_H

#include <cassert> 
#include <cmath> 
#include <cstddef> 
#include <iostream> 
#include <list> 
#include <vector>
#include <unordered_map>
#include <sstream>
#include <stack>

using std::unordered_map, std::vector;

class Graph {
    private:
        struct Edge {
            size_t id;
            double _weight;
        };
        struct Vertex {
            vector<Edge> adjList;
            double distance = INFINITY;
            bool known;
            size_t nextNode = INT32_MAX;
        };
        unordered_map <size_t,Vertex> vertexList;
        size_t numEdges; // total number of elements in the list
    public:
        Graph() : vertexList(), numEdges(0) {}
        Graph(const Graph& other) : vertexList(other.vertexList), numEdges(other.numEdges) {}
        Graph& operator=(const Graph& other)
        {
            if (this == &other)
            {
                return *this;
            }
            vertexList = other.vertexList;
            numEdges = other.numEdges;
            return *this;
        }
        ~Graph() 
        {
            vertexList.clear();
        }
        size_t vertex_count() const {
            return vertexList.size();
        }
        size_t edge_count() const {
            return numEdges;
        }
        bool contains_vertex(size_t id) const { // based on insert
            if (vertexList.find(id) != vertexList.end())
                return true;
            return false;
        }
        bool contains_edge(size_t src, size_t dest) const
        {
            unordered_map <size_t,Vertex>::const_iterator itr = vertexList.find(src);
            if (itr != vertexList.end())
            {
                for (Edge edge : itr->second.adjList)
                {
                    if (edge.id == dest)
                    {
                        return true;
                    }
                }
            }
            return false;
        }
        double cost(size_t src, size_t dest) const 
        {
            if (!contains_edge(src,dest)) // edge does not exist
            {
                return INFINITY;
            }
            unordered_map <size_t,Vertex>::const_iterator itr = vertexList.find(src); // returns an iterator to that element in the table
            for (Edge edge : itr->second.adjList) // access the adjacency vertices, if it's empty, returns infinity
            {
                if (edge.id == dest) // if there exists an edge from src to dest, return its weight
                {
                    return edge._weight;
                } 
            }
            return INFINITY;
        }
        bool add_vertex(size_t id) {
            if (contains_vertex(id))
            {
                return false;
            }
            Vertex newVertex;
            vertexList[id] = newVertex;
            return true;
        }
        bool add_edge(size_t src, size_t dest, double weight=1)
        {
            if (!contains_vertex(src)) // if the source vertex doesn't exist
            {
                return false;
            }
            if (!contains_vertex(dest)) // if the destination vertex doesn't exist
            {
                return false;
            }
            if (contains_edge(src,dest))
            {
                return false;
            }
            Edge newEdge;
            newEdge.id = dest;
            newEdge._weight = weight;
            vertexList[src].adjList.push_back(newEdge);
            numEdges++;
            return true;
        }
        bool remove_vertex(size_t id) {
            if (!contains_vertex(id))
            {
                return false;
            }
            unordered_map <size_t,Vertex>::const_iterator itr = vertexList.find(id);
            numEdges -= itr->second.adjList.size();
            vertexList.erase(id); // account for the fact that vertex might have edges of its own
            for (auto& index : vertexList) // for each <size_t, vector> pair in the list
            {
                for (vector<Edge>::iterator itr = index.second.adjList.begin(); itr != index.second.adjList.end(); itr++)
                {
                    if (itr->id == id)
                    {
                        index.second.adjList.erase(itr);
                        itr--;
                        numEdges--;
                    }
                }
            }
            return true;
        }
        bool remove_edge(size_t src, size_t dest)
        {
            if (!contains_vertex(src))
            {
                return false;
            }
            if (!contains_vertex(dest))
            {
                return false;
            }
            if (!contains_edge(src,dest))
            {
                return false;
            }
            unordered_map <size_t,Vertex>::iterator itr = vertexList.find(src);
            for (vector<Edge>::iterator vectorItr = itr->second.adjList.begin(); vectorItr != itr->second.adjList.end(); vectorItr++)
            {
                if (vectorItr->id == dest)
                {
                    itr->second.adjList.erase(vectorItr);
                    vectorItr--;
                    numEdges--;
                }
            }
            return true;
            
        }
        void print_graph(std::ostream& os =std::cout) const
        {
            if (vertexList.size() == 0)
            {
                os << "<empty>\n";
                return;
            }
            for (auto index : vertexList)
            {
                os << "Vertex: " << index.first << "; Destinations: ";
                for (Edge edge : index.second.adjList)
                {
                    os << edge.id << " with weight " << edge._weight << " | ";
                }
                os << "\n";
            }
        }
        
        size_t minDistance()
        {
            double min = INFINITY;
            size_t minID = 0;
            for (unordered_map <size_t,Vertex>::iterator itr = vertexList.begin(); itr != vertexList.end(); itr++)
            {
                if (itr->second.known == false && itr->second.distance <= min)
                {
                    minID = itr->first;
                    min = itr->second.distance;
                }
            }
            return minID;
        }
        
        void dijkstra(size_t source_id) {
            int numUnknown = 0;
            for (auto& pair : vertexList) // rethink how distance works...?
            {
                pair.second.distance = INFINITY;
                pair.second.known = false;
                pair.second.nextNode = INT32_MAX;
                numUnknown++;
            }
            if (!contains_vertex(source_id))
            {
                return;
            }
            vertexList.find(source_id)->second.distance = 0;
            while (numUnknown > 0) // use a counter for num unknown
            {
                size_t smallestUnknownID = minDistance();
                unordered_map <size_t,Vertex>::iterator itr = vertexList.find(smallestUnknownID);
                itr->second.known = true;
                numUnknown--;
                for (vector<Edge>::iterator vectorItr = itr->second.adjList.begin(); vectorItr != itr->second.adjList.end(); vectorItr++)
                {
                    if (!vertexList.find(vectorItr->id)->second.known) // the vertex w is unknown
                    {
                        double cvw = cost(smallestUnknownID,vectorItr->id);
                        double vDist = vertexList.find(smallestUnknownID)->second.distance;
                        double wDist = vertexList.find(vectorItr->id)->second.distance;
                        if (vDist + cvw < wDist) // v.dist + cvw < w.dist
                        {
                            vertexList.find(vectorItr->id)->second.distance = vDist + cvw;
                            vertexList.find(vectorItr->id)->second.nextNode = smallestUnknownID;
                        }
                    }
                }
            }
        }

        double distance(size_t id) const {
            if (vertexList.find(id) == vertexList.end()) // vertex doesn't exist
            {
                return INFINITY;
            }
            return vertexList.find(id)->second.distance; // path doesn't exist then it'll return infinity
        }
        
        void print_shortest_path(size_t dest_id, std::ostream& os=std::cout) const {
            unordered_map <size_t,Vertex>::const_iterator itr = vertexList.find(dest_id);
            std::stack<size_t> theStack;
            
            if (!contains_vertex(dest_id)) // vertex doesn't exist
            {
                os << "<no path>\n";
                return;
            }
            if (distance(dest_id) == INFINITY) // path doesn't exist
            {
                os << "<no path>\n";
                return;
            }
            
            size_t nextID = dest_id;
            while (nextID != INT32_MAX)
            {
                theStack.push(nextID);
                nextID = itr->second.nextNode;
                itr = vertexList.find(nextID);
            }
            os << theStack.top();
            theStack.pop();
            while (!theStack.empty())
            {
                os << " --> " << theStack.top();
                theStack.pop();
            }
            os << " distance: " << distance(dest_id) << "\n";
        }
        
};

#endif  // GRAPH_H
