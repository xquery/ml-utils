#include <cstring>
#include <string>
#include <vector>
#include <istream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <map>
#include <unistd.h>

#include "../admin.cpp"

using namespace std;

int main(int argc, char *argv[]) {

    try {
        Admin admin;
        admin.setCurrentArgs(admin.options(argc, argv));
        CommandLineArgs current = admin.getCurrentArgs();
        Config config = admin.getConfig();

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

        admin.setUrl("8000", "/v1/eval", "", "");

        if (strcmp(current.xquery, "") == 0) {
            std::string line, qry;
            while (std::getline(std::cin, line)) {
                qry.append(line);
            }
            admin.executeQueryPost("xquery", qry);
        } else {
            admin.executeQueryPost("xquery", current.xquery);
        }
        string result = admin.getReadBuffer();

        if (strcmp(current.format, "json") == 0) {
            cout << result << endl;
        } else {
            cout << result << endl;
        }

    } catch (std::bad_alloc) {
        cout << "Error with mlquery" << endl;
    }
    return EXIT_SUCCESS;
}
