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


int walkInstallConfig(Admin admin, Config config, string path){

    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir (path.c_str())) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            string name = ent->d_name;
            string fullpath = path + "/" + name;
            if (name != "." && name != ".." && name != ".DS_Store") {
                if (name.find(".json") != std::string::npos) {

                    if (fullpath.find("forests") != std::string::npos) {
                        cout << "installing forest" << endl;
                        admin.setResourceUrl(config.port, config.path, "forests");
                        admin.executeInstallPost(fullpath, "json");
                    } else if (fullpath.find("databases") != std::string::npos) {
                        cout << "installing database" << endl;
                        admin.setResourceUrl(config.port, config.path, "databases");
                        admin.executeInstallPost(fullpath, "json");
                    } else if (fullpath.find("servers") != std::string::npos) {
                        cout << "installing server" << endl;
                        admin.setResourceUrl(config.port, config.path, "servers");
                        admin.executeInstallPost(fullpath, "json");
                    }else if (fullpath.find("amps") != std::string::npos) {
                        cout << "installing amps" << endl;
                        admin.setResourceUrl(config.port, config.path, "amps");
                        admin.executeInstallPost(fullpath, "json");
                    }else if (fullpath.find("external-securities") != std::string::npos) {
                        cout << "installing external-securities" << endl;
                        admin.setResourceUrl(config.port, config.path, "external-securities");
                        admin.executeInstallPost(fullpath, "json");
                    }else if (fullpath.find("privileges") != std::string::npos) {
                        cout << "installing privileges" << endl;
                        admin.setResourceUrl(config.port, config.path, "privileges");
                        admin.executeInstallPost(fullpath, "json");
                    }else if (fullpath.find("security/roles") != std::string::npos) {
                        cout << "installing roles" << endl;
                        admin.setResourceUrl(config.port, config.path, "roles");
                        admin.executeInstallPost(fullpath, "json");
                    }else if (fullpath.find("users") != std::string::npos) {
                        cout << "installing users" << endl;
                        admin.setResourceUrl(config.port, config.path, "users");
                        admin.executeInstallPost(fullpath, "json");
                    }else if (fullpath.find("protected-paths") != std::string::npos) {
                        cout << "installing protected-paths" << endl;
                        admin.setResourceUrl(config.port, config.path, "protected-paths");
                        admin.executeInstallPost(fullpath, "json");
                    }

                }else if (name.find(".xml") != std::string::npos) {
                    cout << "ignoring xml for now" << endl;
                }else{
                    walkInstallConfig(admin, config, fullpath);
                }
            }
        }
        closedir (dir);
    } else {
        /* could not open directory */
        perror ("");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

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
                //admin.executeResourcePut(body, resource);
            }else if(command == "install"){

                //get top level ml-config directory
                string path = config.config;
                walkInstallConfig(admin,config,path);

            }
            cout << admin.getReadBuffer() << endl;
        }else{
            cout << "test2" << endl;

        }

    } catch (std::bad_alloc) {
        cerr << "Error with ml-config" << endl;
    }
    return EXIT_SUCCESS;
}


