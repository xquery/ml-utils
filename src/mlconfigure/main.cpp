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
#include <dirent.h>
#include <sys/stat.h>

#include "../admin.cpp"

using namespace std;

int main(int argc, char *argv[]) {

    try {
        Admin admin;
        admin.setCurrentArgs(admin.options(argc, argv));
        CommandLineArgs current = admin.getCurrentArgs();
        Config config = admin.getConfig();

        if (current.verbose) {
            admin.displayargs();
            admin.displayconfig();
        }

        string command = current.command;
        string resource = current.resource;
        string name = current.name;

        if (!command.empty()) {
            if (command == "restart") {
                admin.setResourceUrl(config.port, config.path, "");
                admin.executeResourcePost("{\"operation\":\"restart-local-cluster\"}", "");
            }else if(command == "get"){
                admin.setUrl(config.port, config.path, current.resource, "default");
                admin.execute();
            }else if(command == "get-properties") {
                admin.setUrl(config.port, config.path, resource + "/properties", "default");
                admin.execute();
            }else if(command == "create"){

                string line, body;
                if(name.empty()){
                    while (getline(std::cin, line)) {
                        body.append(line);
                    }
                }
                admin.setResourceUrl(config.port, config.path, resource);
                if(body.empty()){
                    string payload;
                    if(resource == "forests"){
                        payload="{\"forest-name\":\""+name+"\"}";
                    }else if(resource=="databases"){
                        payload="{\"database-name\":\""+name+"\"}";
                    }else if(resource=="servers"){
                        payload="{\"server-name\":\""+name+"\"}";
                    }else if(resource=="groups"){
                        payload="{\"group-name\":\""+name+"\"}";
                    }

                    admin.executeResourcePost(payload,resource);

                }else{
                    admin.executeResourcePost(body,resource);
                }
            }else if(command == "update"){
                string line, body;
                while (getline(std::cin, line)) {
                    body.append(line);
                }
                admin.setResourceUrl(config.port, config.path, resource + "/properties");
                cout << body << endl;
                admin.executeResourcePut(body, resource);
            }else if(command == "install" || command == "reinstall"){
                string path = config.mlconfig;
                admin.walkInstallConfig(admin,config,path);
            }
            cout << admin.getReadBuffer() << endl;
        }else{
            cerr << "no command" << endl;
            return EXIT_FAILURE;
        }

    } catch (std::bad_alloc) {
        cerr << "Error with ml-config" << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


