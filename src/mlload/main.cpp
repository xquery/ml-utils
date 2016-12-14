#include <cstring>
#include <string>
#include <vector>
#include <istream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include "../load.cpp"

int main(int argc, char *argv[]) {

    try {
        Load load;
        load.setCurrentArgs(load.options(argc, argv));
        CommandLineArgs current = load.getCurrentArgs();
        Config config = load.getConfig();

        if (current.verbose) {
            load.displayargs();
            load.displayconfig();
        }

        load.setLoadUrl(config.port, "/v1/documents", current.uri);
        std::string line, body;
        if(!current.filename){
        while (std::getline(std::cin, line)) {
            body.append(line);
        }}
        load.executeLoadPost(current.filename,body);

    } catch (std::bad_alloc) {
        cerr << "Error with ml-load" << endl;
    }
    return EXIT_SUCCESS;
};
