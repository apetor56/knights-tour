#include <iostream>
#include <fstream>
#include <cmath>

int main(int argc, const char *argv[]) {
    /* std::fstream file("output.txt");
    if(!file) {
        std::cerr << "failed to open file\n";
        return -1;
    } */

    int size;
    int *position;

    std::cin >> size;
    position = new int[size * size]();

    for(int i = 0; i < size * size; i++) {
        std::cin >> position[i];
    }

    /* for(int i = 0; i < size * size; i++) {
        if(i % size == 0) {
            std::cout << '\n';
        }
        std::cout << position[i] << ' ';
    }
    std::cout << '\n'; */

    int *inOrder = new int[size * size]();
    for(int i = 0; i < size * size; i++) {
        for(int j = 0; j < size * size; j++) {
            if(position[j] == i) {
                inOrder[i] = j;
            }
        }
    }

    /* for(int i = 0; i < size * size; i++) {
        if(i % size == 0) {
            std::cout << '\n';
        }
        std::cout << inOrder[i] << ' ';
    }
    std::cout << '\n'; */

    const int move_x[] {
        2, 2, 1, 1, -1, -1, -2, -2
    };
    const int move_y[] {
        size, -size, 2 * size, -2 * size, 2 * size, -2 * size, size, -size
    };

    /* std::fstream converted("convert_output.txt");
    if(!converted) {
        std::cerr << "failed to open converted file\n";
        return -2;
    } */

    int diff = 0;
    for(int i = 1; i < size * size; i++) {
        diff = inOrder[i] - inOrder[i - 1];
        for(int j = 0; j < 8; j++) {
            if(move_x[j] + move_y[j] == diff) {
                std::cout << move_x[j] << ' ' << -move_y[j] / size << '\n';
            }
        }
    }

    /* file.close();
    converted.close(); */
    delete[] position;
    delete[] inOrder;
}