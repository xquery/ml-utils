#include <cstring>
#include <string>
#include <vector>
#include <istream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include "../browse.cpp"

using namespace std;
using namespace mlutil;

int main(int argc, char *argv[]) {
    try {
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
        LOG_S(ERROR) << "Error with ml-browse.";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
};
