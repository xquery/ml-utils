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
 * entry point for ml-log - utility for retrieving MarkLogic logs.
 */

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
using namespace mlutil;

int main(int argc, char *argv[]) {

    try {
        Log log;
        log.setCurrentArgs(log.options(argc, argv));
        CommandLineArgs current = log.getCurrentArgs();
        Config config = log.getConfig();

        if (current.verbose) {
            log.displayargs();
            log.displayconfig();
        }

        log.setUrl(config.port, config.path + "/logs", "", "");
        log.setLogUrl(config.port, config.path + "/logs", current.name, current.start, current.end, current.regex,
                      current.host, "text");
        log.execute();

        if (!current.quiet) {
            cout << log.getReadBuffer() << endl;
        }
    } catch (std::bad_alloc) {
        LOG_S(ERROR) << "Error with ml-log.";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
