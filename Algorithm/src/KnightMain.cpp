#include <iostream>
#include <algorithm>
#include <cstring>
#include "KnightsTour.hpp"

int main(int argc, const char *argv[]) {
    if(argc == 2 && strcmp("-h", argv[1]) == 0) {
        std::cout << "Pass two arguments in command line:\n"
        << '\t' << argv[0] << " [CHESSBOARD SIZE]" << " [START POSITION]\n";
        return 1;
    }
    if(argc != 3) {
        std::cerr << "Pass two arguments in command line:\n"
        << argv[0] << " [CHESSBOARD SIZE]" << " [START POSITION]\n"
        << "for example: if size equals 5, then we have 25 start position, from 0 to 24\n";
        return 2;
    }

    int size = atoi(argv[1]);
    int start_position = atoi(argv[2]);

    if(size < 5 || start_position < 0 || start_position >= size * size) {
        std::cerr << "Passed wrong arguments. See " << argv[0] << " -h";
    }

    KnightTour knight(size);
    bool *visited = new bool[size * size]();
    
    std::cout << size << '\n';
    knight.findSolution(start_position, visited, std::vector<int>());
}