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

#include "log.hpp"

namespace mlutil {

    /* Log::Log
     *
     */
    Log::Log() {
    };

    /* Log::~Log
     *
     */
    Log::~Log() {
    };

    /*! Log::options
     *
     * @param n_opts
     * @param opts
     * @return
     */
    CommandLineArgs Log::options(int n_opts, char *opts[]) {
        CommandLineArgs args;

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
            } else if (opts[i][1] == 'n') {
                ++i;
                args.name = opts[i];
            } else if (opts[i][1] == 't') {
                ++i;
                args.host = opts[i];
            } else if (opts[i][1] == 'r') {
                ++i;
                args.regex = opts[i];
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

    /*! Log::usage
     *
     * @param progname
     * @return
     */
    int Log::usage(const char *progname) {
        const char *name = getprogname(progname);
        cerr << "ml-utils: ml-log 1.0 | copyright (c)2015 Jim Fuller | see https://github.com/xquery/ml-utils\n"
             << "retrieve logs\n"
             << ">" << name << " \n"
             << "    -c    : config file (ex. /home/jfuller/.ml-utils)\n"
             << "    -f    : format(xml | json)\n"
             << "    -t    : host (ex. localhost)\n"
             << "    -n    : filename (ex. ErrorLog.txt)\n"
             << "    -s    : start (ex. 2016-10-27T06:00:00)\n"
             << "    -e    : end (ex. 2016-10-27T06:00:00)\n"
             << "    -r    : regex (ex. Merged)\n"
             << "    -v    : verbose (show http call)\n"
             << "    -q    : quiet (suppress banner)" << endl;
        return EXIT_SUCCESS;
    }


    /*! setLogUrl
     *
     * @param port
     * @param root
     * @param filename
     * @param start
     * @param end
     * @param regex
     * @param host
     * @param format
     */
     void Log::setLogUrl(string port,
                           string root,
                           string filename,
                           string start,
                           string end,
                           string regex,
                           string host,
                           string format) {
        checkConfig();

        url = config.protocol + "://" + config.host + ":" + port + root + "?";
        if (!filename.empty()) {
            url += "filename=" + filename;
        }
        if (!start.empty()) {
            url += "&start=" + start;
        }
        if (!end.empty()) {
            url += "&end=" + end;
        }
        if (!regex.empty()) {
            url += "&regex=" + regex;
        }
        if (!host.empty()) {
            url += "&host=" + host;
        }
        //string format = current.format;
        if (format != "") {
            url += "&format=" + format;
        }
    }
}