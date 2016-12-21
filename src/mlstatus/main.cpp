#include <cstring>
#include <string>
#include <vector>
#include <istream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/pointer.h>

#include "../status.cpp"

using namespace rapidjson;

int main(int argc, char *argv[]) {

    try {
        Status status;
        status.setCurrentArgs(status.options(argc, argv));
        CommandLineArgs current = status.getCurrentArgs();
        Config config = status.getConfig();

        if (current.verbose) {
            status.displayargs();
            status.displayconfig();
        }

        status.setUrl(config.port, config.path, current.resource, "status");
        status.execute();

        if(!current.quiet) {
            string result = status.getReadBuffer();
            if (strcmp(current.format, "json") == 0) {
                cout << result << endl;
            } else {
                cout << result << endl;
            }
        }
    } catch (std::bad_alloc) {
        LOG_S(ERROR) << "Error with ml-status.";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
