#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>

int main(int argc, char* argv[])
{
    std::vector<int> data;
    std::ifstream input_file("input.txt");

    if (!input_file.is_open()) {
        std::cerr << "Не удалось открыть файл." << std::endl;
        return 1;
    }

    int value;
    while (input_file >> value) {
        data.push_back(value);
        std::cout << value;
    }
}
