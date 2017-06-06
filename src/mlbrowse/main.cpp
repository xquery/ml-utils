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
 * entry point for ml-browse - utility for browsing documents in an MarkLogic database.
 */

#include <cstring>
#include <string>
#include <vector>
#include <istream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include "../browse.cpp"

using namespace std;
using namespace mlutil;

int main(int argc, char *argv[]) {
    try {
        Browse *pBrowse= new Browse();
        pBrowse->setCurrentArgs(pBrowse->options(argc, argv));
        CommandLineArgs current = pBrowse->getCurrentArgs();
        Config config = pBrowse->getConfig();
        if (current.verbose) {
            pBrowse->displayargs();
            pBrowse->displayconfig();
        }
        cout << "not implemented yet" << endl;
        delete pBrowse;
    } catch (std::bad_alloc) {
        LOG_S(ERROR) << "Error with ml-browse.";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
};
