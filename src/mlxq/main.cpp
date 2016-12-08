#include <cstring>
#include <string>
#include <vector>
#include <istream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <unistd.h>

#include "../query.cpp"

using namespace std;

int main(int argc, char *argv[]) {

    try {
        Query query;
        query.setCurrentArgs(query.options(argc, argv));
        CommandLineArgs current = query.getCurrentArgs();
        Config config = query.getConfig();

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
            cout << "database: " << current.database << endl;
            cout << "xquery: " << current.xquery << endl;
        }

        query.setUrl("8000", "/v1/eval", "", "");

        if (strcmp(current.xquery, "") == 0) {
            std::string line, qry;
            while (std::getline(std::cin, line)) {
                qry.append(line);
            }
            query.executeQueryPost("xquery", qry);
        } else {
            query.executeQueryPost("xquery", current.xquery);
        }
        string result = query.getReadBuffer();

        // count number of lines
        std::stringstream cc(result);
        std::string un;
        int countline = 0;
        while (std::getline(cc, un, '\n')) {
            countline++; }

        if (strcmp(current.format, "json") == 0) {
            if(current.raw){
                cout<< result << endl;
            }else {
                std::stringstream ss(result);
                std::string to;
                if (!result.empty()) {
                    int count = 0;
                    while (std::getline(ss, to, '\n')) {
                        if (count > 4 && count != countline - 1) {
                            cout << to << endl;
                        }
                        count++; }}}
        } else {
            if(current.raw){
                cout<< result << endl;
            }else {
                std::stringstream ss(result);
                std::string to;
                if (!result.empty()) {
                    int count = 0;
                    while (std::getline(ss, to, '\n')) {
                        if (count > 4 && count != countline - 1) {
                            cout << to << endl;
                        }
                        count++; }}}}

    } catch (std::bad_alloc) {
        cout << "Error with ml-xq" << endl;
    }
    return EXIT_SUCCESS;
}
