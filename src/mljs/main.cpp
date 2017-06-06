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
 * entry point for ml-js - utility for evaluating javascript against a MarkLogic server.
 */

#include <cstring>
#include <string>
#include <vector>
#include <istream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include "../query.cpp"

using namespace std;
using namespace mlutil;

int main(int argc, char *argv[]) {

    try {
        Query *pQuery = new Query();
        pQuery->setCurrentArgs(pQuery->options(argc, argv));
        CommandLineArgs current = pQuery->getCurrentArgs();
        Config config = pQuery->getConfig();

        if (current.verbose) {
            pQuery->displayargs();
            pQuery->displayconfig();
        }

        pQuery->setQueryUrl(config.port, "/v1/eval", "", "");
        if (strcmp(current.js, "") == 0) {
            std::string line, qry;
            while (std::getline(std::cin, line)) {
                qry.append(line);
            }
            pQuery->executeQueryPost("javascript", qry);
        } else {
            pQuery->executeQueryPost("javascript", current.js);
        }

        if (!current.quiet) {
            string result = pQuery->getReadBuffer();
            // count number of lines
            std::stringstream cc(result);
            std::string un;
            int countline = 0;
            while (std::getline(cc, un, '\n')) {
                countline++;
            }

            if (strcmp(current.format, "json") == 0) {
                LOG_S(INFO) << "output json";
                std::stringstream ss(result);
                std::string to;
                if (!result.empty()) {
                    int count = 0;
                    while (std::getline(ss, to, '\n')) {
                        if (count > 4 && count != countline - 1) {
                            cout << to << endl;
                        }
                        count++;
                    }
                }
            } else {
                LOG_S(INFO) << "output multipart";
                std::stringstream ss(result);
                std::string to;
                if (!result.empty()) {
                    int count = 0;
                    while (std::getline(ss, to, '\n')) {
                        if (count > 4 && count != countline - 1) {
                            cout << to << endl;
                        }
                        count++;
                    }
                }
            }
        }
        delete pQuery;
    } catch (std::bad_alloc) {
        LOG_S(ERROR) << "Error with ml-js.";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
