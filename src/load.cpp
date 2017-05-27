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

#include "load.hpp"

namespace mlutil {

    /* Load::Load
     *
     */
    Load::Load() {//std::cout << "load constructor called\n";
    };

    /* Load::~Load
     *
     */
    Load::~Load() {//std::cout << "load destructor called\n";
    };

    /*! Load::options
     *
     * @param n_opts
     * @param opts
     * @return
     */
    CommandLineArgs Load::options(int n_opts, char *opts[]) {
        CommandLineArgs args;

        for (int i = 1; i < n_opts; ++i) {
            if (opts[i][1] == 'd') {
                ++i;
                args.database = opts[i];
            } else if (opts[i][1] == 'u') {
                ++i;
                args.uri = opts[i];
            } else if (opts[i][1] == 'f') {
                ++i;
                args.filename = opts[i];
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

    /*! Load::usage
     *
     * @param progname
     * @return
     */
    int Load::usage(const char *progname) {
        const char *name = getprogname(progname);
        cerr << "ml-utils: ml-load 1.0 | copyright (c)2015 Jim Fuller | see https://github.com/xquery/ml-utils\n"
             << "load data\n"
             << ">" << name << " [options]\n"
             << "    -c    : config file (ex. /home/jfuller/.ml-utils)\n"
             << "    -d    : database\n"
             << "    -u    : uri\n"
             << "    -f    : filename\n"
             << "    -v    : verbose (show http call)\n"
             << "    -q    : quiet (suppress banner)" << endl;
        return EXIT_SUCCESS;
    }

    /*! setLoadUrl
     *
     * @param port
     * @param root
     * @param uri
     */
     void Load::setLoadUrl(string port,
                            string root,
                            string uri) {
        checkConfig();

        url = config.protocol + "://" + config.host + ":" + port + root + "?";
        if (!uri.empty()) {
            url += "uri=" + uri;
        }
        string database = current.database;
        if (!database.empty()) {
            url += "&database=" + database; }
    }

    /*! executeLoadPost
     *
     * @param file
     * @param body
     * @return
     */
     int Load::executeLoadPost(const char *file, string body) {

        FILE *fd;

        fd = fopen(file, "rb"); /* open file to upload */

        CURLM *curlm;
        int handle_count;
        curlm = curl_multi_init();

        CURL *curl1 = NULL;
        curl1 = curl_easy_init();

        curl_multi_setopt(curlm, CURLMOPT_PIPELINING, 0L);

        if (curl1) {
            curl_easy_setopt(curl1, CURLOPT_HTTPHEADER, headers);

            curl_easy_setopt(curl1, CURLOPT_USERNAME, config.user.c_str());
            curl_easy_setopt(curl1, CURLOPT_PASSWORD, config.pass.c_str());

            if (current.verbose) {
                curl_easy_setopt(curl1, CURLOPT_VERBOSE, 1L);
            } else {
                curl_easy_setopt(curl1, CURLOPT_VERBOSE, 0L);
            }

            curl_easy_setopt(curl1, CURLOPT_USERAGENT, "ml-utils via curl");

            curl_easy_setopt(curl1, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
            curl_easy_setopt(curl1, CURLOPT_FAILONERROR, 1);
            curl_easy_setopt(curl1, CURLOPT_DEBUGFUNCTION, log_trace);
            curl_easy_setopt(curl1, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl1, CURLOPT_URL, url.c_str());

            curl_easy_setopt(curl1, CURLOPT_UPLOAD, 1L);
            curl_easy_setopt(curl1, CURLOPT_READDATA, fd);

            curl_easy_setopt(curl1, CURLOPT_NOPROGRESS, 1L);

            curl_multi_add_handle(curlm, curl1);
            CURLMcode code;
            while (1) {
                code = curl_multi_perform(curlm, &handle_count);
                if (handle_count == 0) {
                    curl_global_cleanup();
                    break;
                }
            }
        }
        curl_global_cleanup();
        return EXIT_SUCCESS;
    };
}