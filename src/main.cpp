#include <iostream>
#include <string>

#include "json.h"
#include "sort.h"
#include "net.h"
#include "devcore.h"

int main(int argc, char* argv[]) {
    std::string cmd = "sort";
    if (argc > 1) {
        cmd = argv[1];
        std::string cmd = argv[1];
        if (cmd == "sort") {
            use_sort();
        } else if (cmd == "net") {
            use_network();
        } else if (cmd == "json") {
            use_json();
        } else if (cmd == "base64") {
            use_base64();
        } else if (cmd == "uuid") {
            use_uuid();
        } else if (cmd == "aes") {
            use_aes();
        } else if (cmd == "hash") {
            use_hash();
        } else if (cmd == "ini") {
            use_ini();
        }
        return 0;
    }
    use_ini();
    return 0;
}
