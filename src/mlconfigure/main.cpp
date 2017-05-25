/*
 * Copyright 2017 James Fuller
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * entry point for ml-config - utility for setting up resources on MarkLogic server.
 */

#include <cstring>
#include <string>
#include <vector>
#include <istream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include "../admin.cpp"

using namespace std;
using namespace mlutil;

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
            } else if (command == "get") {
                admin.setUrl(config.port, config.path, current.resource, "default");
                admin.execute();
            } else if (command == "get-properties") {
                admin.setUrl(config.port, config.path, resource + "/properties", "default");
                admin.execute();
            } else if (command == "create") {
                string line, body;
                if (name.empty()) {
                    while (getline(std::cin, line)) {
                        body.append(line);
                    }
                }
                admin.setResourceUrl(config.port, config.path, resource);
                if (body.empty()) {
                    string payload;
                    if (resource == "forests") {
                        payload = "{\"forest-name\":\"" + name + "\"}";
                    } else if (resource == "databases") {
                        payload = "{\"database-name\":\"" + name + "\"}";
                    } else if (resource == "servers") {
                        payload = "{\"server-name\":\"" + name + "\"}";
                    } else if (resource == "groups") {
                        payload = "{\"group-name\":\"" + name + "\"}";
                    }
                    admin.executeResourcePost(payload, resource);
                } else {
                    admin.executeResourcePost(body, resource);
                }
            } else if (command == "update") {
                string line, body;
                while (getline(std::cin, line)) {
                    body.append(line);
                }
                admin.setResourceUrl(config.port, config.path, resource + "/properties");
                admin.executeResourcePut(body, resource);
            } else if (command == "install" || command == "reinstall") {
                string path = config.mlconfig;
                cout << path << endl;
                admin.walkInstallConfig(admin, config, path);
            } else {
                LOG_S(ERROR) << "invalid command.";
                return EXIT_FAILURE;
            }
            cout << admin.getReadBuffer() << endl;
        } else {
            LOG_S(ERROR) << "no command supplied.";
            return EXIT_FAILURE;
        }
    } catch (std::bad_alloc) {
        LOG_S(ERROR) << "problem with ml-config.";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
