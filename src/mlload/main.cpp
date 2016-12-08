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
            cout << "----------------" << endl;
            cout << "~/.ml-utils" << endl;
            cout << "----------------" << endl;
            cout << "user: " << config.user << endl;
            cout << "pass: " << config.pass << endl;
            cout << "host: " << config.host << endl;
            cout << "----------------" << endl;
            cout << "options" << endl;
            cout << "----------------" << endl;
            cout << "format: " << current.format << endl;
            cout << "resource: " << current.resource << endl;
        }

        load.setLoadUrl("8000", "/v1/documents", current.uri);
        std::string line, body;
        if(!current.filename){
        while (std::getline(std::cin, line)) {
            body.append(line);
        }}
        load.executeLoadPost(current.filename,body);

    } catch (std::bad_alloc) {
        cout << "Error with ml-load" << endl;
    }
    return EXIT_SUCCESS;
};
