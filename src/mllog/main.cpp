#include <cstring>
#include <string>
#include <vector>
#include <istream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include "../log.cpp"

using namespace std;
using namespace mlutil;

int main(int argc, char *argv[]) {

    try {
        Log log;
        log.setCurrentArgs(log.options(argc, argv));
        CommandLineArgs current = log.getCurrentArgs();
        Config config = log.getConfig();

        if (current.verbose) {
            log.displayargs();
            log.displayconfig();
        }

        log.setUrl(config.port, config.path + "/logs", "", "");
        log.setLogUrl(config.port, config.path + "/logs",current.name,current.start,current.end,current.regex,current.host,"text");
        log.execute();

        if(!current.quiet) {
            cout << log.getReadBuffer() << endl;
        }
    } catch (std::bad_alloc) {
        LOG_S(ERROR) << "Error with ml-log.";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
