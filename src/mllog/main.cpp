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

        log.setUrl("8002", "/manage/v2/logs", "", "");

        log.execute();

        string result = log.getReadBuffer();

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
