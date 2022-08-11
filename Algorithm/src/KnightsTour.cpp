#include "KnightsTour.hpp"
#include <iostream>

const static int x[] = {-1,  1,  2, 2, 1, -1, -2, -2};
const static int y[] = {-2, -2, -1, 1, 2,  2,  1, -1};
static int counter = 0;
static int *warnsdorff;

/* create graph of valid knight's movements
and warnsdorf tab */
KnightTour::KnightTour(int _size) : size(_size) {
    warnsdorff = new int[size * size]();
    for(int i = 0; i < size * size; i++) {
        graph.addVertex(i);
    }

    // x - axis
    for(int i = 0; i < size; i++) {
        // y - axis
        for(int j = 0; j < size; j++) {
            // go through all possible knight's movements
            // and check if knight is still on the board
            for(int k = 0; k < 8; k++) {
                if(onBoard(i + x[k], j + y[k])) {
                    graph.addEdge(i * size + j, (i + x[k]) * size + (j + y[k]));
                    warnsdorff[i * size + j]++;
                }
            }
        }
    }
}

KnightTour::~KnightTour() {
    delete[] warnsdorff;
}

void KnightTour::findSolution(int position, bool *visited, std::vector<int> path) {
    counter++;
    if(done == true) {
        return;
    }

    visited[position] = true;

    if(path.size() == size_t(size * size - 1)) {
        int *result = new int[size * size]();

        for(int i = 0; i < size * size - 1; i++) {
            result[path.at(i)] = i + 1;
        }

        for(int i = 0; i < size; i++) {
            for(int j = 0; j < size; j++) {
                std::cout << result[i * size + j] << ' ';
            }
            std::cout << '\n';
        }
        done = true;
        delete[] result;
        return;
    }

    std::list<int> vertices = graph.neighbours(position);
    sortWarndorff(vertices);

    for(int& vertex : vertices) {
        if(visited[vertex] == false) {
            std::vector<int> newPath;
            newPath = path;
            newPath.push_back(vertex);

            bool *visited_copy = new bool[size * size]();
            std::copy(visited, visited + size * size, visited_copy);

            findSolution(vertex, visited_copy, newPath);
            delete[] visited_copy;
        }
    }
    return;
}

void KnightTour::show() {
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            std::cout << warnsdorff[i * size + j] << ' ';
        }
        std::cout << '\n';
    }

    std::cout << '\n';

    for(int i = 0; i < size * size; i++) {
        auto neighbours = graph.neighbours(i);
        std::cout << "[" << i << "]: ";
        
        for(int& x : neighbours) {
            std::cout << x << ", ";
        }
        std::cout << '\n';
    }
}

bool KnightTour::onBoard(int position_X, int position_Y) {
    return (position_X >= 0 && position_X < size)
        && (position_Y >= 0 && position_Y < size);
}

void KnightTour::sortWarndorff(std::list<int>& vertices) {
    vertices.sort(comparator());
}

bool KnightTour::comparator::operator()(const int& vertex1, const int vertex2) {
    return (warnsdorff[vertex1] < warnsdorff[vertex2]);
}
