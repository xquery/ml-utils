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

#include <cstring>
#include <string>
#include <vector>
#include <istream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <map>

#include "status.hpp"

namespace mlutil {

    Status::Status() {
        //std::cout << "status constructor called\n";
    };

    Status::~Status() {
        //std::cout << "status destructor called\n";
    };

    /*!
     *
     * @param n_opts
     * @param opts
     * @return
     */
    CommandLineArgs Status::options(int n_opts, char *opts[]) {
        CommandLineArgs args;

        args.group = "Default";

        for (int i = 1; i < n_opts; ++i) {
            if (opts[i][1] == 'f') {
                ++i;
                args.format = opts[i];
            } else if (opts[i][1] == 'q') {
                args.quiet = true;
            } else if (opts[i][1] == 'v') {
                args.verbose = true;
            } else if (opts[i][1] == 'r') {
                ++i;
                args.resource = opts[i];
            } else if (opts[i][1] == 'g') {
                ++i;
                args.group = opts[i];
            } else if (opts[i][1] == 'h') {
                usage(opts[0]);
                exit(1);
            } else if (opts[i][1] == 'c') {
                ++i;
                args.config = opts[i];
            } else {
                //usage(opts[0]);
            }
        }
        args.check(opts[0]);
        return args;
    }

    /*!
     *
     * @param progname
     * @return
     */
    int Status::usage(const char *progname) {
        const char *name = getprogname(progname);
        cerr << "ml-utils: ml-status 1.0 | copyright (c)2015 Jim Fuller | see https://github.com/xquery/ml-utils\n"
             << "retrieve MarkLogic resource status\n"
             << ">" << name << " Resources/resource [options]\n"
             << "    -c    : config file (ex. /home/jfuller/.ml-utils)\n"
             << "    -f    : format (xml|json)\n"
             << "    -r    : resource (databases|forests|servers|hosts) / resource-name\n"
             << "    -v    : verbose (show http call)\n"
             << "    -q    : quiet (suppress banner)\n";
        return EXIT_SUCCESS;
    }
}