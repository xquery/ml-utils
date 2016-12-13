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
        log.setLogUrl(config.port, config.path + "/logs",current.resource,current.start,current.end,current.regex,current.host);
        log.execute();

        string result = log.getReadBuffer();

        if (strcmp(current.format, "json") == 0) {
            cout << result << endl;
        } else {
            cout << result << endl;
        }

    } catch (std::bad_alloc) {
        cout << "Error with ml-log" << endl;
    }
    return EXIT_SUCCESS;
}
