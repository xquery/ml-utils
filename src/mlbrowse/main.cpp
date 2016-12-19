#include <cstring>
#include <string>
#include <vector>
#include <istream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include "../browse.cpp"

int main(int argc, char *argv[]) {

    try {
        //loguru::init(argc, argv);
        Browse browse;
        browse.setCurrentArgs(browse.options(argc, argv));
        CommandLineArgs current = browse.getCurrentArgs();
        Config config = browse.getConfig();

        if (current.verbose) {
            browse.displayargs();
            browse.displayconfig();
        }

        cout << "not implemented yet" << endl;

    } catch (std::bad_alloc) {
        cout << "Error with ml-browse" << endl;
    }
    return EXIT_SUCCESS;
};
