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

    if(path.size() == size_t(size * size)) {
        result.reserve(size * size);

        for(int i = 0; i < size * size; i++) {
            result.push_back(path.at(i));
        }

        done = true;
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

std::vector<int> KnightTour::getResult() const {
    return result;
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

std::vector<int> KnightTour::convertOutput() {
    std::vector<int>& inOrder = result;

    const int move_x[] {
        2, 2, 1, 1, -1, -1, -2, -2
    };
    const int move_y[] {
        size, -size, 2 * size, -2 * size, 2 * size, -2 * size, size, -size
    };

    int diff = 0;
    std::vector<int> moves((size * size) * 2);

    // first two zeros means that we don't want to move knight from start
    // position "in first frame" by translation matrix 
    auto it = moves.begin() + 2;

    for(int i = 1; i < size * size; i++) {
        diff = inOrder[i] - inOrder[i - 1];
        for(int j = 0; j < 8; j++) {
            if(move_x[j] + move_y[j] == diff) {
                (*it++) = move_x[j];
                (*it++) = -move_y[j] / size;
            }
        }
    }

    return moves;
}
