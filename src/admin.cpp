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
#include <dirent.h>

#include "admin.hpp"

namespace mlutil {

    /*!
     *
     */
    Admin::Admin() {
    };

    /*!
     *
     */
    Admin::~Admin() {
    };

    /*! Admin::options
     *
     * @param n_opts
     * @param opts
     * @return
     */
    CommandLineArgs Admin::options(int n_opts, char *opts[]) {
        CommandLineArgs args;

        for (int i = 1; i < n_opts; ++i) {
            string optstr = opts[i];
            if (optstr == "--host") {
                ++i;
                args.chost = opts[i];
            } else if (optstr == "--port") {
                ++i;
                args.format = opts[i];
            } else if (optstr == "--user") {
                ++i;
                args.cuser = opts[i];
            } else if (optstr == "--pass") {
                ++i;
                args.cpass = opts[i];
            } else if (optstr == "--path") {
                ++i;
                args.cpath = opts[i];
            } else if (optstr == "--protocol") {
                ++i;
                args.cprotocol = opts[i];
            } else if (optstr == "--ml-config") {
                ++i;
                args.cmlconfig = opts[i];
            } else if (optstr == "-f") {
                ++i;
                args.format = opts[i];
            } else if (optstr == "-n") {
                ++i;
                args.name = opts[i];
            } else if (optstr == "-q") {
                args.quiet = true;
            } else if (optstr == "-r") {
                ++i;
                args.resource = opts[i];
            } else if (optstr == "-c") {
                ++i;
                args.config = opts[i];
            } else if (optstr == "-p") {
                ++i;
                args.payload = opts[i];
            } else if (optstr == "-v") {
                args.verbose = true;
            } else if (optstr == "-h") {
                usage(opts[0]);
                exit(1);
            } else {
                args.command = opts[i];
            }
        }
        args.check(opts[0]);
        return args;
    }

    /*! Admin::usage
     *
     * @param progname
     * @return
     */
    int Admin::usage(const char *progname) {
        const char *name = getprogname(progname);
        cerr << "ml-utils: ml-config 1.0 | copyright (c)2015 Jim Fuller | see https://github.com/xquery/ml-utils\n"
             << "configure MarkLogic\n"
             << ">" << name << " command [options]\n"
             << "   [command] : restart | get | get-properties | create | update | delete | install\n"
             << "    --ml-config : ml-config directory (ex. etc/ml-config)\n"
             << "    -c          : config file (ex. /home/jfuller/.ml-utils\n"
             << "    -p          : payload path \n"
             << "    -n          : name \n"
             << "    -r          : {resources}/{resource-name} directory\n"
             << "    -v          : verbose (show http call)\n"
             << "    -q          : quiet (suppress banner)" << endl;
        return EXIT_SUCCESS;
    }

    /*! Admin::walkInstallConfig
     *
     * @param admin
     * @param config
     * @param path
     * @return
     */
    int Admin::walkInstallConfig(Admin admin, Config config, string path) {
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir(path.c_str())) != NULL) {
            while ((ent = readdir(dir)) != NULL) {
                string name = ent->d_name;
                string fullpath = path + "/" + name;
                if (name != "." && name != ".." && name != ".DS_Store") {
                    if (name.find(".json") != std::string::npos) {
                        if (fullpath.find("forests") != std::string::npos) {
                            cout << "installing forest" << endl;
                            admin.setResourceUrl(config.port, config.path, "forests");
                            admin.executeInstallPost(fullpath, "json");
                        } else if (fullpath.find("databases") != std::string::npos) {
                            cout << "installing database" << endl;
                            admin.setResourceUrl(config.port, config.path, "databases");
                            admin.executeInstallPost(fullpath, "json");
                        } else if (fullpath.find("servers") != std::string::npos) {
                            cout << "installing server" << endl;
                            admin.setResourceUrl(config.port, config.path, "servers");
                            admin.executeInstallPost(fullpath, "json");
                        } else if (fullpath.find("amps") != std::string::npos) {
                            cout << "installing amps" << endl;
                            admin.setResourceUrl(config.port, config.path, "amps");
                            admin.executeInstallPost(fullpath, "json");
                        } else if (fullpath.find("external-securities") != std::string::npos) {
                            cout << "installing external-securities" << endl;
                            admin.setResourceUrl(config.port, config.path, "external-security");
                            admin.executeInstallPost(fullpath, "json");
                        } else if (fullpath.find("privileges") != std::string::npos) {
                            cout << "installing privileges" << endl;
                            admin.setResourceUrl(config.port, config.path, "privileges");
                            admin.executeInstallPost(fullpath, "json");
                        } else if (fullpath.find("security/roles") != std::string::npos) {
                            cout << "installing roles" << endl;
                            admin.setResourceUrl(config.port, config.path, "roles");
                            admin.executeInstallPost(fullpath, "json");
                        } else if (fullpath.find("users") != std::string::npos) {
                            cout << "installing users" << endl;
                            admin.setResourceUrl(config.port, config.path, "users");
                            admin.executeInstallPost(fullpath, "json");
                        } else if (fullpath.find("protected-paths") != std::string::npos) {
                            cout << "installing protected-paths" << endl;
                            admin.setResourceUrl(config.port, config.path, "protected-paths");
                            admin.executeInstallPost(fullpath, "json");
                        }
                    } else if (name.find(".xml") != std::string::npos) {
                        cout << "ignoring xml properties" << endl;
                    } else {
                        walkInstallConfig(admin, config, fullpath);
                    }
                }
            }
            closedir(dir);
        } else {
            /* could not open directory */
            LOG_S(ERROR) << "problem installing";
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

    /*! executeResourcePut
     *
     * @param body
     * @param format
     * @return
     */
     int Admin::executeResourcePut(string body, string format) {

        int handle_count;
        curlm = curl_multi_init();

        curl1 = curl_easy_init();

        curl_multi_setopt(curlm, CURLMOPT_PIPELINING, 0L);

        if (format == "xml") {
            headers = curl_slist_append(headers, "Content-Type: application/xml");
        } else {
            headers = curl_slist_append(headers, "Content-Type: application/json");
        }

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
            curl_easy_setopt(curl1, CURLOPT_PUT, 1L);
            curl_easy_setopt(curl1, CURLOPT_READDATA, body.c_str());

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
            //std::cout << readBuffer << std::endl;
        }

        curl_global_cleanup();

        return EXIT_SUCCESS;

    };

    /*! executeResourcePost
     *
     * @param body
     * @param format
     * @return
     */
     int Admin::executeResourcePost(string body, string format) {

        long http_code = 0;

        int handle_count;

        if (format == "xml") {
            curl_slist_append(headers, "Content-Type: application/xml");
        } else {
            curl_slist_append(headers, "Content-Type: application/json");
        }
        setCurlOpts();

        if (curl1) {

            curl_easy_setopt(curl1, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl1, CURLOPT_URL, url.c_str());

            curl_easy_setopt(curl1, CURLOPT_POSTFIELDS, body.c_str());
            curl_easy_setopt(curl1, CURLOPT_POSTFIELDSIZE, body.length());
            curl_easy_setopt(curl1, CURLOPT_POST, 1);

            curl_easy_getinfo (curl1, CURLINFO_RESPONSE_CODE, &http_code);

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

    /*! executeInstallPost
     *
     * @param filename
     * @param format
     * @return
     */
     int Admin::executeInstallPost(string filename, string format) {
        std::ifstream ifs(filename);
        string body = string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

        int handle_count;

        struct curl_httppost *post = NULL;
        struct curl_httppost *last = NULL;

        if (current.verbose) {
            curl_easy_setopt(curl1, CURLOPT_VERBOSE, 1L);
        } else {
            curl_easy_setopt(curl1, CURLOPT_VERBOSE, 0L);
        }
        if (format == "xml") {
            headers = curl_slist_append(headers, "Content-Type: application/xml");
        } else {
            headers = curl_slist_append(headers, "Content-Type: application/json");
        }

        setCurlOpts();

        if (curl1) {
            curl_easy_setopt(curl1, CURLOPT_POSTFIELDS, body.c_str());
            curl_easy_setopt(curl1, CURLOPT_POST, 1L);

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
            //std::cout << readBuffer << std::endl;
        }
        return EXIT_SUCCESS;
    };
}