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
#include <stdio.h>
#include <pthread.h>
#include<dirent.h>
#include "load.hpp"

#define MAX 20

namespace mlutil {

    /* Load::Load
     *
     */
    Load::Load() {//std::cout << "load constructor called\n";
        LOG_S(INFO) << "load constructor called";
    };

    /* Load::~Load
     *
     */
    Load::~Load() {//std::cout << "load destructor called\n";
        LOG_S(INFO) << "load destructor called";
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
      int Load::executeLoadPost(string filename,string uri) {

        FILE *fd;
        fd = fopen(filename.c_str(), "rb");
        string url = "http://node1:8000/v1/documents?uri="+uri;

        int handle_count;

        if (curl1) {
            curl_easy_setopt(curl1, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl1, CURLOPT_UPLOAD, 1L);
            curl_easy_setopt(curl1, CURLOPT_READDATA, fd);
            curl_easy_setopt(curl1, CURLOPT_NOPROGRESS, 1L);

            CURLMcode code;
            while (1) {
                code = curl_multi_perform(curlm, &handle_count);
                if (handle_count == 0) {
                    curl_global_cleanup();
                    break;
                }
            }

        }
        return EXIT_SUCCESS;
    };

    void* Load::postOneFile(CURLM *cm, int i, vector<string> *v)
    {
        CURL *eh = curl_easy_init(); //curl_easy_duphandle(curl1);
        FILE *fd;
        fd = fopen(v->at(i).c_str(), "rb");
        string url = "http://node1:8000/v1/documents?uri="+v->at(i);

        curl_easy_setopt(eh, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
        curl_easy_setopt(eh, CURLOPT_USERNAME, "admin");
        curl_easy_setopt(eh, CURLOPT_PASSWORD, "admin");

        curl_easy_setopt(eh, CURLOPT_URL, url.c_str());
        curl_easy_setopt(eh, CURLOPT_UPLOAD, 1L);
        curl_easy_setopt(eh, CURLOPT_READDATA, fd);
        curl_easy_setopt(eh, CURLOPT_NOPROGRESS, 1L);
        curl_multi_add_handle(cm, eh);
    }

    int Load::executeLoad(string dirname) {
        struct dirent *ent;
        vector<string> v;

        DIR *dir = getDir(dirname.c_str());

        while ((ent = readdir (dir)) != NULL) {
            string filename = ent->d_name;
            v.push_back(dirname +"/"+ filename);
        }
        v.erase(v.begin());
        v.erase(v.begin() + 1);
        closedir (dir);

        cout << "uploading #" << v.size() << "documents: " << endl;

        long L;
        unsigned int C=0;
        int M, Q, U = -1;
        fd_set R, W, E;
        struct timeval T;

        //curl_multi_setopt(cm, CURLMOPT_MAXCONNECTS, (long)MAX);
        //curl_multi_setopt(cm, CURLMOPT_MAX_TOTAL_CONNECTIONS, 10L);

        // setup
        for(C = 0; C < v.size(); ++C) {
            postOneFile(cm, C, &v);
        }

        while(U) {
            curl_multi_perform(cm, &U);
            if(U) {
                FD_ZERO(&R);
                FD_ZERO(&W);
                FD_ZERO(&E);

                if(curl_multi_fdset(cm, &R, &W, &E, &M)) {
                    fprintf(stderr, "E: curl_multi_fdset\n");
                    return EXIT_FAILURE;
                }

                if(curl_multi_timeout(cm, &L)) {
                    fprintf(stderr, "E: curl_multi_timeout\n");
                    return EXIT_FAILURE;
                }
                if(L == -1)
                    L = 100;

                if(M == -1) {
                    sleep((unsigned int)L / 1000);
                }
                else {
                    T.tv_sec = L/1000;
                    T.tv_usec = (L%1000)*1000;

                    if(0 > select(M+1, &R, &W, &E, &T)) {
                        fprintf(stderr, "E: select(%i,,,,%li): %i: %s\n",
                                M+1, L, errno, strerror(errno));
                        return EXIT_FAILURE;
                    }
                }
            }

            while((msg = curl_multi_info_read(cm, &Q))) {
                if(msg->msg == CURLMSG_DONE) {
                    char *url;
                    CURL *e = msg->easy_handle;
                    curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &url);
                    fprintf(stderr, "R: %d - %s <%s>\n",
                            msg->data.result, curl_easy_strerror(msg->data.result), url);
                    curl_multi_remove_handle(cm, e);
                    curl_easy_cleanup(e);
                }
                else {
                    fprintf(stderr, "E: CURLMsg (%d)\n", msg->msg);
                }
            }
        }

        return EXIT_SUCCESS;
    };
}