#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <list>
#include <map>
#include <climits>
#include <fstream>
#include <iostream>
// #include "GraphException.hpp"

template<typename T> 
class Graph {
public:
    Graph() {}

    Graph(const Graph& other) = delete;

    Graph& operator=(const Graph& other) = delete;

    bool adjacent(unsigned int index1, unsigned int index2) {
        auto& currentList = data.at(index1).adjacency;
        for(auto it = currentList.begin(); it != currentList.end(); it++) {
            if((*it).toVertex == index2) {
                return true;
            }
        }
        return false;
    }

    std::list<T> neighbours(unsigned int index) {
        if(data.count(index) == 0) {
            // throw GraphException("setVertex(): vertex with given index does not exist", index);
        }

        std::list<T> tmp;
        auto currentList = data.at(index).adjacency;
        for(auto it = currentList.begin(); it != currentList.end(); it++) {
            tmp.push_back((*it).toVertex);
        }

        return tmp;
    }

    void addVertex(unsigned int index, T value = 0) {
        data.insert(std::make_pair(index, Vertex(value)));
    }

    void setVertex(unsigned int index, T value) {
        if(data.count(index) == 0) {
            // throw GraphException("setVertex(): vertex with given index does not exist", index);
        }

        data.at(index).value = value;
    }

    T getVertex(unsigned int index) {
        if(data.count(index) == 0) {
            // throw GraphException("setVertex(): vertex with given index does not exist", index);
        }
        return data.at(index).value;
    }

    void removeVertex(unsigned int index) {
        if(data.count(index) == 0) {
            // throw GraphException("removeVertex(): vertex with given index does not exist", index);
        }

        for(auto& pair : data) {
            auto& currentList = pair.second.adjacency;
            for(auto it = currentList.begin(); it != currentList.end(); it++) {
                if((*it).toVertex == index) {
                    currentList.erase(it);
                    break;
                }
            }
        }
        data.erase(index);
    }

    void addEdge(unsigned int index1, unsigned int index2, int value = -INT_MAX) {
        checkVertices(index1, index2);
        auto& currentList = data.at(index1).adjacency;
        for(auto it = currentList.begin(); it != currentList.end(); it++) {
            if((*it).toVertex == index2) {
                return;
            }
        }

        data.at(index1).adjacency.push_back(Edge(index2, value));
    }

    void setEdge(unsigned int index1, unsigned int index2, int value) {
        checkVertices(index1, index2);

        auto& currentList = data.at(index1).adjacency;
        for(auto it = currentList.begin(); it != currentList.end(); it++) {
            if((*it).toVertex == index2) {
                (*it).value = value;
                return;
            }
        }

        // throw GraphException("setEdge(): edge does not exist beetwen given vertices", index1, index2);
    }

    int getEdge(unsigned int index1, unsigned int index2) {
        checkVertices(index1, index2);

        auto& currentList = data.at(index1).adjacency;
        for(auto it = currentList.begin(); it != currentList.end(); it++) {
            if((*it).toVertex == index2) {
                return (*it).value;
            }
        }

        // throw GraphException("getEdge(): edge does not exist beetwen given vertices", index1, index2);
    }

    bool findEdge(unsigned int index1, unsigned int index2) {
        auto& currentList = data.at(index1).adjacency;
        for(auto it = currentList.begin(); it != currentList.end(); it++) {
            if((*it).toVertex == index2) {
                return true;
            }
        }
        return false;
    }

    void removeEdge(unsigned int index1, unsigned int index2, int value = INT_MAX) {
        checkVertices(index1, index2);

        auto& currentList = data.at(index1).adjacency;
        for(auto it = currentList.begin(); it != currentList.end(); it++) {
            if((*it).toVertex == index2) {
                if(value == INT_MAX) {
                    currentList.erase(it);
                    return;
                }
                else if((*it).value == value) {
                    currentList.erase(it);
                    return;
                }
            }
        }

        // throw GraphException("getEdge(): edge does not exist beetwen given vertices", index1, index2);
    }

    void saveToImage(const std::string& name) const {
        std::fstream file("../graph/" + name + ".dot", std::fstream::out | std::fstream::trunc);

        if(!file) {
            std::cerr << "failed to open file\n";
            return;
        }

        file << "digraph A {\n";
        for(auto& pair : data) {
            auto& currentList = pair.second.adjacency;
            file << '\t' << pair.first << " -> {";
            for(auto it = currentList.begin(); it != currentList.end(); it++) {
                file << (*it).toVertex; 
                if((*it) != currentList.back()) {
                    file << ", ";
                }
            }
            file << "}\n";
        }
        file << "}";

        file.close();
    }

    unsigned int size() const noexcept {
        return data.size();
    }

private:
    struct Edge {
        unsigned int toVertex;
        int value;

        Edge(unsigned int _toVertex, int _value) :
            toVertex(_toVertex),
            value(_value) {}

        bool operator==(const Edge& other) const {
            return (this == &other);
        }

        bool operator!=(const Edge& other) const {
            return !(operator==(other));
        }

    };

    struct Vertex {
        T value;
        std::list<Edge> adjacency;

        Vertex(T _value) :
            value(_value) {}
    };

    void checkVertices(unsigned int index1, unsigned int index2) {
        int c1 = data.count(index1);
        if(c1 == 0) {
            // throw GraphException("vertex with given index does not exist", index1);
        }

        int c2 = data.count(index2);
        if(c2 == 0) {
            // throw GraphException("vertex with given index does not exist", index2);
        }
    }

    std::map<unsigned int, Vertex> data;
};

#endif // GRAPH_HPP