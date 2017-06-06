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
 * entry point for ml-load - utility for loading files into MarkLogic.
 */

#include <cstring>
#include <string>
#include <vector>
#include <istream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <dirent.h>

#include "../load.cpp"

using namespace std;
using namespace mlutil;

int main(int argc, char *argv[]) {

    try {
        Load *pLoad = new Load();
        pLoad->setCurrentArgs(pLoad->options(argc, argv));
        CommandLineArgs current = pLoad->getCurrentArgs();
        Config config = pLoad->getConfig();
        if (current.verbose) {
            pLoad->displayargs();
            pLoad->displayconfig();
        }
        pLoad->setLoadUrl(config.port, "/v1/documents", current.uri);
        if (!current.filename) {
            LOG_S(INFO) << "load from stdin single file: "  << current.filename ;
            std::string line, body;
            while (std::getline(std::cin, line)) {
                body.append(line);
            }
        }else{
            struct stat buffer;
            string name = current.filename;
            string uri = current.uri;
            if (stat (current.filename, &buffer) == 0){
                pLoad->initCurl();
                LOG_S(INFO) << "load file: " << current.filename;
                pLoad->executeLoadPost(name,uri);
            }else{
                LOG_S(INFO) << "load directory: " << current.filename;
                pLoad->executeLoad(name);
            }
        }
        delete pLoad;

    } catch (std::bad_alloc) {
        LOG_S(ERROR) << "Error with ml-load.";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
};
