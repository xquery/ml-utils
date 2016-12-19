#include <cstring>
#include <string>
#include <vector>
#include <istream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include "../query.cpp"

int main(int argc, char *argv[]) {

    try {
        //loguru::init(argc, argv);
        Query query;
        query.setCurrentArgs(query.options(argc, argv));
        CommandLineArgs current = query.getCurrentArgs();
        Config config = query.getConfig();

        if (current.verbose) {
            query.displayargs();
            query.displayconfig();
        }

        query.setUrl(config.port, "/v1/eval", "", "");
        if (strcmp(current.js, "") == 0) {
            std::string line, qry;
            while (std::getline(std::cin, line)) {
                qry.append(line);
            }
            query.executeQueryPost("javascript", qry);
        } else {
            query.executeQueryPost("javascript", current.js);
        }

        if(!current.quiet) {

            string result = query.getReadBuffer();

            // count number of lines
            std::stringstream cc(result);
            std::string un;
            int countline = 0;
            while (std::getline(cc, un, '\n')) {
                countline++;
            }

            if (strcmp(current.format, "json") == 0) {
                std::stringstream ss(result);
                std::string to;

                if (!result.empty()) {
                    int count = 0;
                    while (std::getline(ss, to, '\n')) {
                        if (count > 4 && count != countline - 1) {
                            cout << to << endl;
                        }
                        count++; }}
            } else {
                std::stringstream ss(result);
                std::string to;

                if (!result.empty()) {
                    int count = 0;
                    while (std::getline(ss, to, '\n')) {
                        if (count > 4 && count != countline - 1) {
                            cout << to << endl;
                        }
                        count++; }}
            }
        }
    } catch (std::bad_alloc) {
        cerr << "Error with ml-js" << endl;
    }
    return EXIT_SUCCESS;
}
