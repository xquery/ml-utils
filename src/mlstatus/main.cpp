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
 * entry point for ml-status - utility for retrieving MarkLogic resource statuses.
 */

#include <cstring>
#include <string>
#include <vector>
#include <istream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include <rapidjson/document.h>

#include "../status.cpp"

using namespace rapidjson;
using namespace std;
using namespace mlutil;

int main(int argc, char *argv[]) {

    try {
        Status status;
        status.setCurrentArgs(status.options(argc, argv));
        CommandLineArgs current = status.getCurrentArgs();
        Config config = status.getConfig();

        if (current.verbose) {
            status.displayargs();
            status.displayconfig();
        }

        status.setUrl(config.port, config.path, current.resource, "status");
        status.execute();

        if (!current.quiet) {
            string result = status.getReadBuffer();
            if (strcmp(current.format, "json") == 0) {
                cout << result << endl;
            } else {
                cout << result << endl;
            }
        }
    } catch (std::bad_alloc) {
        LOG_S(ERROR) << "Error with ml-status.";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
