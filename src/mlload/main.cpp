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

#include "../load.cpp"

using namespace std;
using namespace mlutil;

int main(int argc, char *argv[]) {

    try {
        Load load;
        load.setCurrentArgs(load.options(argc, argv));
        CommandLineArgs current = load.getCurrentArgs();
        Config config = load.getConfig();
        if (current.verbose) {
            load.displayargs();
            load.displayconfig();
        }
        load.setLoadUrl(config.port, "/v1/documents", current.uri);
        std::string line, body;
        if (!current.filename) {
            while (std::getline(std::cin, line)) {
                body.append(line);
            }
        }
        load.executeLoadPost(current.filename, body);
    } catch (std::bad_alloc) {
        LOG_S(ERROR) << "Error with ml-load.";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
};
