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
            cout << "resource: " << current.resource << endl;
        }

        admin.setResourceUrl("8002", "/manage/v2", string(current.resource));

        std::string line, body;
        while (std::getline(std::cin, line)) {
                body.append(line);
        }
        if(string(current.resource).find("properties") == 0){
            admin.executeResourcePut(body, string(current.resource));

        }else{
            admin.executeResourcePost(body, string(current.resource));
        }

    } catch (std::bad_alloc) {
        cout << "Error with ml-config" << endl;
    }
    return EXIT_SUCCESS;
}
