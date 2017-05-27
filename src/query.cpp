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

#include "query.hpp"

namespace mlutil {

    /* Query::Query
     *
     */
    Query::Query() {
    };

    /* Query::~Query
     *
     */
    Query::~Query() {
    };

    /*! Query::options
     *
     * @param n_opts
     * @param opts
     * @return
     */
    CommandLineArgs Query::options(int n_opts, char *opts[]) {
        CommandLineArgs args;

        args.xquery = "";
        args.js = "";

        for (int i = 1; i < n_opts; ++i) {
            if (opts[i][1] == 'f') {
                ++i;
                args.format = opts[i];
            } else if (opts[i][1] == 'q') {
                args.quiet = true;
            } else if (opts[i][1] == 'd') {
                ++i;
                args.database = opts[i];
            } else if (opts[i][1] == 'x') {
                ++i;
                args.xquery = opts[i];
            } else if (opts[i][1] == 'j') {
                ++i;
                args.js = opts[i];
            } else if (opts[i][1] == 'c') {
                ++i;
                args.config = opts[i];
            } else if (opts[i][1] == 'r') {
                args.raw = true;
            } else if (opts[i][1] == 'v') {
                args.verbose = true;
            } else if (opts[i][1] == 'h') {
                usage(opts[0]);
                exit(1);
            } else {
                //usage(opts[0]);
            }
        }
        args.check(opts[0]);
        return args;
    }

    /*! Query::usage
     *
     * @param progname
     * @return
     */
    int Query::usage(const char *progname) {
        const char *name = getprogname(progname);
        cerr << "ml-utils: " << name << " 1.0 | copyright (c)2015 Jim Fuller | see https://github.com/xquery/ml-utils\n"
             << "evaluate query\n"
             << ">" << name << " database [options]\n"
             << "    -c    : config file (ex. /home/jfuller/.ml-utils)\n"
             << "    -f    : format (xml|json)\n"
             << "    -d    : database\n"
             << "    -r    : raw output\n"
             << "    -v    : verbose (show http call)\n"
             << "    -q    : quiet (suppress banner)" << endl;
        return EXIT_SUCCESS;
    }

    /*! setQueryUrl
     *
     * @param port
     * @param root
     * @param path
     * @param view
     */
     void Query::setQueryUrl(string port,
                             string root,
                             string path,
                             string view) {
        checkConfig();
        url = config.protocol + "://" + config.host + ":" + port + root + "/" + path + "?";

        string database = current.database;
        if (!database.empty()) {
            url += "&database=" + database;
        }
    };

    /*! executeQueryPost
     *
     * @param type
     * @param query
     * @return
     */
     int Query::executeQueryPost(string type, string query) {

        int handle_count;
        setCurlOpts();

        if (curl1) {
            //curl_easy_setopt(curl1, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl1, CURLOPT_URL, url.c_str());

            char *data = curl_easy_escape(curl1, query.c_str(), strlen(query.c_str()));
            string xquery = type + "=" + data;
            curl_easy_setopt(curl1, CURLOPT_POST, 1);
            curl_easy_setopt(curl1, CURLOPT_POSTFIELDS, xquery.c_str());
            curl_easy_setopt(curl1, CURLOPT_NOPROGRESS, 1L);

            curl_easy_setopt(curl1, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl1, CURLOPT_WRITEDATA, &readBuffer);

            CURLMcode code;
            while (1) {
                code = curl_multi_perform(curlm, &handle_count);

                if (handle_count == 0) {
                    curl_global_cleanup();

                    break;
                }
            }
            //std::cout << readBuffer << std::endl;
        }

        return EXIT_SUCCESS;

    };

}