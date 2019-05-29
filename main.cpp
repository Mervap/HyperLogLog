//
// Created by dumpling on 28.05.19.
//

#include <iostream>
#include <fstream>
#include "hyper_log_log.h"

void print_help() {
    std::cout << "-file <filename> — read arrays from file" << std::endl
              << "-help — help" << std::endl << std::endl
              << "The expected performance of the arrays:" << std::endl
              << "n" << std::endl
              << "a_1 a_2 ... a_n" << std::endl;
}

int main(int argc, char **argv) {

    if (argc > 3) {
        std::cerr << "Wrong count of arguments, use help" << std::endl;
    }


    if (argc == 2) {
        if (std::string(argv[1]) == "-help") {
            print_help();
            return EXIT_SUCCESS;
        } else {
            std::cerr << "Wrong argument, use help" << std::endl;
            return EXIT_FAILURE;
        }
    }

    size_t n;
    Hyper_log_log set;
    if (argc == 3) {
        if (std::string(argv[1]) == "-file") {
            std::ifstream in(argv[2]);
            std::ifstream::sync_with_stdio(false);
            in.tie(nullptr);

            in >> n;
            int a;
            for (int i = 0; i < n; ++i) {
                in >> a;
                set.add(a);
            }
        } else {
            std::cerr << "Wrong argument, use help" << std::endl;
            return EXIT_FAILURE;
        }
    } else {
        std::ios_base::sync_with_stdio(false);
        std::cin.tie(nullptr);

        std::cin >> n;
        int a;
        for (int i = 0; i < n; ++i) {
            std::cin >> a;
            set.add(a);
        }
    }
    std::cout.tie(nullptr);

    uint32_t ans = set.get_uniq_num();
    std::cout << "Approximate number of distinct elements: " << ans << std::endl;
    return 0;
}