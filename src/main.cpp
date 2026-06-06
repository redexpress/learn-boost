#include <iostream>
#include <string>

#include "sort.h"
#include "net.h"

int main(int argc, char* argv[]) {
    std::string cmd = "sort";
    if (argc > 1) {
        cmd = argv[1];
        std::string cmd = argv[1];
        if (cmd == "sort") {
            use_sort();
        } else if (cmd == "net") {

        } else {

        }
        return 0;
    }
    use_network();
    return 0;
}
