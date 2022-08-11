#ifndef KNIGHTSTOUR_HPP
#define KNIGHTSTOUR_HPP

#include <vector>
#include "Graph.hpp"

class KnightTour {
public:
    KnightTour(int _size);

    ~KnightTour();

    void findSolution(int position, bool *visited, std::vector<int> path);

    void show();
private:
    Graph<int> graph;
    int size;
    bool done = false;

    bool onBoard(int position_X, int position_Y);

    void sortWarndorff(std::list<int>& vertices);

    struct comparator {
        bool operator()(const int& vertex1, const int vertex2);
    };
};

#endif // KNIGHTSTOUR_HPP