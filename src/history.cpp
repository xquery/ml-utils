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
#include <ctime>

#include "history.hpp"

using namespace std;

namespace mlutil {

    History::History() {
        //std::cout << "history constructor called\n";
    };

    History::~History() {
        //std::cout << "history destructor called\n";
    };

    /*!
     *
     * @param n_opts
     * @param opts
     * @return
     */
    CommandLineArgs History::options(int n_opts, char *opts[]) {
        CommandLineArgs args;

        args.period = "hour";
        args.start = "";
        args.end = "";

        for (int i = 1; i < n_opts; ++i) {
            if (opts[i][1] == 'f') {
                ++i;
                args.format = opts[i];
            } else if (opts[i][1] == 'p') {
                ++i;
                args.period = opts[i];
            } else if (opts[i][1] == 's') {
                ++i;
                args.start = opts[i];
            } else if (opts[i][1] == 'e') {
                ++i;
                args.end = opts[i];
            } else if (opts[i][1] == 'm') {
                ++i;
                args.metric = opts[i];
            } else if (opts[i][1] == 'r') {
                ++i;
                args.resource = opts[i];
            } else if (opts[i][1] == 'o') {
                ++i;
                args.output = opts[i];
            } else if (opts[i][1] == 'g') {
                ++i;
                args.gnuplot = opts[i];
            } else if (opts[i][1] == 'q') {
                args.quiet = true;
            } else if (opts[i][1] == 'v') {
                args.verbose = true;
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
    int History::usage(const char *progname) {
        const char *name = getprogname(progname);
        cerr << "ml-utils: ml-hist 1.0 | copyright (c)2015 Jim Fuller | see https://github.com/xquery/ml-utils\n"
             << "retrieve MarkLogic history metrics\n"
             << ">" << name << " resources/{resource-name} [options]\n"
             << "    -c    : config file (ex. /home/jfuller/.ml-utils)\n"
             << "    -f    : format (xml|json)\n"
             << "    -p    : period (raw|hour|day)\n"
             << "    -s    : start date (ex. 2015-03-21T17:38:00)\n"
             << "    -e    : end date (ex. 2015-03-22T17:58:00)\n"
             << "    -m    : meter (resource specific)\n"
             << "    -r    : resource (databases|forests|servers|hosts) / resource-name\n"
             << "    -o    : graph output\n"
             << "    -g    : custom gnuplot script\n"
             << "    -v    : verbose (show http call)\n"
             << "    -q    : quiet (suppress banner)" << endl;
        return EXIT_SUCCESS;
    }
}