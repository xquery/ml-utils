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

#pragma once

#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <wchar.h>
#include <vector>
#include <map>
#include <cassert>
#include <curl/curl.h>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/pointer.h>

#include "config.hpp"

#include <gnuplot_i.hpp>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
#include <conio.h>   //for getch(), needed in wait_for_key()
#include <windows.h> //for Sleep()
void sleep(int i) { Sleep(i*1000); }
#endif

#define LOGURU_IMPLEMENTATION 1
#define LOGURU_WITH_STREAMS 1
#define LOGURU_FILENAME_WIDTH 16
#define LOGURU_REDEFINE_ASSERT 1

#include <loguru.hpp>

using namespace rapidjson;
using namespace std;

namespace mlutil {

    struct CommandLineArgs {
        CommandLineArgs()
                : command(""),
                  config(""),
                  format(""),
                  period(""),
                  start(""),
                  end(""),
                  regex(""),
                  metric(""),
                  filename(""),
                  name(""),
                  resource(""),
                  uri(""),
                  database(""),
                  group(""),
                  host(""),
                  xquery(""),
                  js(""),
                  payload(""),
                  output(""),
                  gnuplot(""),
                  chost("localhost"),
                  cport("8002"),
                  cuser("admin"),
                  cpass("admin"),
                  cpath("/manage/v2"),
                  cprotocol("http"),
                  cmlconfig(""),
                  cmllog("/var/log/ml-util.log"),
                  quiet(false),
                  verbose(false),
                  raw(false) {
        }

        void check(const char *progname) {}

        // config overrides
        const char *chost, *cport, *cpath, *cprotocol, *cuser, *cpass, *cmlconfig, *cmllog;

        //comand
        const char *command, *payload, *name;

        //options
        const char *config, *format, *period, *filename, *uri, *start, *end, *regex, *metric, *resource, *database, *group, *host, *xquery, *js, *output, *gnuplot;

        //flags
        bool quiet, verbose, raw;
    };

    // interface for commands
    class Command {
    private:

    protected:
        CommandLineArgs current;
        Config config;
        struct curl_slist *headers = NULL;
        string readBuffer;
        CURLM *curlm;
        CURL *curl1 = NULL;
        CURL *curl2 = NULL;
        string url;
        CURLM *cm;
        CURLMsg *msg;

    public:

        /*! Command
         *
         */
        Command() {
            LOG_S(INFO) << "command constructor called";

            // set defaults
            loadConfig(config, current.config);
            if (config.user.empty()) {
                config.user = current.cuser;
            }
            if (config.pass.empty()) {
                config.pass = current.cpass;
            }
            if (config.host.empty()) {
                config.host = current.chost;
            }
            if (config.protocol.empty()) {
                config.protocol = current.cprotocol;
            }
            if (config.path.empty()) {
                config.path = current.cpath;
            }
            if (config.port.empty()) {
                config.port = current.cport;
            }
            if (config.mlconfig.empty()) {
                config.mlconfig = current.cmlconfig;
            }
            if (config.mllog.empty()) {
                config.mllog = current.cmllog;
            }

            initCurl();
        };

        /*! ~Command
         *
         */
        virtual ~Command() {
            LOG_S(INFO) << "command destructor called";
            curl_easy_cleanup(curl1);
            curl_easy_cleanup(curl2);
            curl_multi_cleanup(curlm);
            curl_multi_cleanup(cm);
            curl_global_cleanup();
        };

        virtual int usage(const char *progname) =0;

        virtual void initCurl(){

            curl_global_init(CURL_GLOBAL_ALL);

            setheaders();

            cm = curl_multi_init();

            curlm = curl_multi_init();
            curl1 = curl_easy_init();
            curl2 = curl_easy_init();
            curl_multi_add_handle(curlm, curl1);

            curl_multi_setopt(cm, CURLMOPT_PIPELINING, CURLPIPE_MULTIPLEX);

            curl_easy_setopt(curl1, CURLOPT_USERNAME, config.user.c_str());
            curl_easy_setopt(curl1, CURLOPT_PASSWORD, config.pass.c_str());
            curl_easy_setopt(curl2, CURLOPT_USERNAME, config.user.c_str());
            curl_easy_setopt(curl2, CURLOPT_PASSWORD, config.pass.c_str());

            if (current.verbose) {
                curl_easy_setopt(curl1, CURLOPT_VERBOSE, 1L);
                curl_easy_setopt(curl2, CURLOPT_VERBOSE, 1L);
            } else {
                curl_easy_setopt(curl1, CURLOPT_VERBOSE, 0L);
                curl_easy_setopt(curl2, CURLOPT_VERBOSE, 0L);
            }

            curl_easy_setopt(curl1, CURLOPT_USERAGENT, "ml-utils via curl");
            curl_easy_setopt(curl1, CURLOPT_FAILONERROR, 1);
            curl_easy_setopt(curl2, CURLOPT_USERAGENT, "ml-utils via curl");
            curl_easy_setopt(curl2, CURLOPT_FAILONERROR, 1);

            curl_easy_setopt(curl1, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
            curl_easy_setopt(curl1, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl2, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
            curl_easy_setopt(curl2, CURLOPT_FOLLOWLOCATION, 1L);

            curl_easy_setopt(curl1, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl1, CURLOPT_DEBUGFUNCTION, log_trace);
            curl_easy_setopt(curl2, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl2, CURLOPT_DEBUGFUNCTION, log_trace);

        };

        virtual CommandLineArgs options(int n_opts, char *opts[]) = 0;

        virtual string getReadBuffer() {
            return this->readBuffer;
        }

        virtual CommandLineArgs getCurrentArgs() {
            return this->current;
        }

        virtual void setCurrentArgs(CommandLineArgs args) {
            this->current = args;
            loadConfig(config, this->current.config);
        }

        virtual Config getConfig() {
            return this->config;
        }

        virtual void setConfig(Config config) {
            this->config = config;
        }

        /*! checkConfig
         *
         * @return
         */
        virtual bool checkConfig() {
            if (config.host.empty()) {
                cerr << "undefined MarkLogic host." << endl;
            }
            if (config.pass.empty()) {
                cerr << "undefined MarkLogic password." << endl;
            }
            if (config.protocol.empty()) {
                cerr << "undefined MarkLogic protocol." << endl;
            }
            if (config.user.empty()) {
                cerr << "undefined MarkLogic user." << endl;
            }
        }

        /*! setResourceUrl
         *
         * @param port
         * @param root
         * @param resource
         */
        virtual void setResourceUrl(string port,
                                    string root,
                                    string resource) {
            checkConfig();
            url = config.protocol + "://" + config.host + ":" + port + root + "/" + resource + "";
        }

        /*! setUrl
         *
         * @param port
         * @param root
         * @param path
         * @param view
         */
        virtual void setUrl(string port,
                            string root,
                            string path,
                            string view) {
            checkConfig();
            if (view == "metrics") {
                if (path.find("/") != string::npos) {
                    string res1 = path.substr(0, path.find("/"));
                    url = config.protocol + "://" + config.host + ":" + port + root + "/" + res1 + "?";
                } else {
                    url = config.protocol + "://" + config.host + ":" + port + root + "/" + path + "?";
                }
            } else if (view == "status") {
                url = config.protocol + "://" + config.host + ":" + port + root + "/" + path + "?";
            } else {
                // identical to status for now
                url = config.protocol + "://" + config.host + ":" + port + root + "/" + path + "?";
            }

            if (!view.empty()) {

                url += "view=" + view;
            }

            string group = current.group;
            if (!group.empty() && view != "status") {
                url += "&group-id=" + group;
            }
            string format = current.format;
            if (format != "") {
                url += "&format=" + format;
            }
            string start = current.start;
            if (start != "") {
                url += "&start=" + start;
            }
            string end = current.end;
            if (end != "") {
                url += "&end=" + end;
            }
            string period = current.period;
            if (period != "") {
                url += "&period=" + period;
            }

            string metric = current.metric;
            if (!metric.empty() && view == "metrics") {
                string resource = current.resource;
                string delimiter = "/";
                string res1 = resource.substr(0, resource.find("/"));
                string res2 = path.substr(path.find("/") + 1, path.size());
                if (res1 == "databases") {
                    url += "&database-metrics=" + metric;
                    if (resource.find("/") != string::npos) {
                        url += "&database=" + res2;
                    }
                } else if (res1 == "servers") {
                    url += "&server-metrics=" + metric;
                    if (resource.find("/") != string::npos) {
                        url += "&group-id=Default&server=" + res2;
                    }
                } else if (res1 == "hosts") {
                    url += "&host-metrics=" + metric;
                    if (resource.find("/") != string::npos) {
                        url += "&host=" + res2;
                    }
                } else if (res1 == "forests") {
                    url += "&forest-metrics=" + metric;
                    if (resource.find("/") != string::npos) {
                        url += "&forest=" + res2;
                    }
                }
            }
        }

        /*! setheaders
         *
         */
        void setheaders() {
            headers = curl_slist_append(headers, "Accept: text/plain");
            headers = curl_slist_append(headers, "Accept: text/html");
            headers = curl_slist_append(headers, "Accept: application/xml");
            headers = curl_slist_append(headers, "Accept: application/js");
            headers = curl_slist_append(headers, "Accept: application/x-www-form-urlencoded");
        };

        /*! log_trace
         *
         * @param handle
         * @param type
         * @param data
         * @param size
         * @param userp
         * @return
         */
        static
        int log_trace(CURL *handle, curl_infotype type,
                      char *data, size_t size,
                      void *userp) {
            const char *text;
            (void) handle; /* prevent compiler warning */
            (void) userp;

            switch (type) {
                case CURLINFO_TEXT:
                    LOG_S(INFO) << data;
                default: /* in case a new one is introduced to shock us */
                    return 0;

                case CURLINFO_HEADER_OUT:
                    LOG_S(INFO) << "=> Send ";
                    LOG_S(INFO) << data;
                    break;
                case CURLINFO_DATA_OUT:
                    LOG_S(INFO) << "=> Send data";
                    LOG_S(INFO) << data;
                    break;
                case CURLINFO_SSL_DATA_OUT:
                    LOG_S(INFO) << "=> Send SSL data";
                    LOG_S(INFO) << data;
                    break;
                case CURLINFO_HEADER_IN:
                    LOG_S(INFO) << "<= Recv header";
                    LOG_S(INFO) << data;
                    break;
                case CURLINFO_DATA_IN:
                    LOG_S(INFO) << "<= Recv data";
                    LOG_S(INFO) << data;
                    break;
                case CURLINFO_SSL_DATA_IN:
                    LOG_S(INFO) << "<= Recv SSL data";
                    LOG_S(INFO) << data;
                    break;
            }
        }

        /*! execute
         *
         * @return
         */
        virtual int execute() {

            int handle_count;
            curlm = curl_multi_init();
            curl_multi_setopt(curlm, CURLMOPT_PIPELINING, 0L);

            CURL *curl1 = NULL;
            curl1 = curl_easy_init();

            if (curl1) {
                curl_easy_setopt(curl1, CURLOPT_HTTPHEADER, headers);

                curl_easy_setopt(curl1, CURLOPT_USERNAME, config.user.c_str());
                curl_easy_setopt(curl1, CURLOPT_PASSWORD, config.pass.c_str());

                if (current.verbose) {
                    curl_easy_setopt(curl1, CURLOPT_VERBOSE, 1L);
                } else {
                    curl_easy_setopt(curl1, CURLOPT_HEADER, 0);
                }

                curl_easy_setopt(curl1, CURLOPT_USERAGENT, "ml-utils via curl/7.19.6");
                curl_easy_setopt(curl1, CURLOPT_FAILONERROR, 1);
                curl_easy_setopt(curl1, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
                curl_easy_setopt(curl1, CURLOPT_FOLLOWLOCATION, 1L);
                curl_easy_setopt(curl1, CURLOPT_URL, url.c_str());
                curl_easy_setopt(curl1, CURLOPT_HTTPGET, 1L);
                curl_easy_setopt(curl1, CURLOPT_NOPROGRESS, 1L);
                curl_easy_setopt(curl1, CURLOPT_WRITEFUNCTION, WriteCallback);
                curl_easy_setopt(curl1, CURLOPT_WRITEDATA, &readBuffer);
                curl_easy_setopt(curl1, CURLOPT_DEBUGFUNCTION, log_trace);

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

        /*! WriteCallback
         *
         * @param contents
         * @param size
         * @param nmemb
         * @param userp
         * @return
         */
        static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
            ((std::string *) userp)->append((char *) contents, size * nmemb);
            return size * nmemb;
        }

        /*! displayargs
         *
         * @return
         */
        int displayargs() {
            LOG_S(INFO) << "----------------";
            LOG_S(INFO) << "options" << endl;
            LOG_S(INFO) << "----------------";

            LOG_S(INFO) << "format: " << current.format;
            LOG_S(INFO) << "database: " << current.database;
            LOG_S(INFO) << "xquery: " << current.xquery;
            LOG_S(INFO) << "format: " << current.format;
            LOG_S(INFO) << "period: " << current.period;
            LOG_S(INFO) << "start: " << current.start;
            LOG_S(INFO) << "end: " << current.end;
            LOG_S(INFO) << "metric: " << current.metric;
            LOG_S(INFO) << "resource: " << current.resource;
            LOG_S(INFO) << "name: " << current.name;
            LOG_S(INFO) << "ml-config: " << current.config;
            LOG_S(INFO) << "output: " << current.output;
            LOG_S(INFO) << "gnuplot: " << current.gnuplot;
            return EXIT_SUCCESS;
        };

        int displayconfig() {
            LOG_S(INFO) << "----------------";
            LOG_S(INFO) << "config";
            LOG_S(INFO) << "----------------";
            LOG_S(INFO) << "user: " << config.user;
            LOG_S(INFO) << "pass: " << config.pass;
            LOG_S(INFO) << "host: " << config.host;
            return EXIT_SUCCESS;
        };

        /*! getprogname
         *
         * @param progname
         * @return
         */
        const char *getprogname(const char *progname) {
            const char *name = progname;
            while (*progname != 0) {
                if (*progname == '/' || *progname == '\\') {
                    ++progname;
                    name = progname;
                } else {
                    ++progname;
                }
            }
            return name;
        };

        /*! doPlot
         *
         * @param output
         * @param plot
         * @param units
         * @param name
         * @param resource
         * @param entries
         */
        void doPlot(string output, string plot, string units, string name, string resource, const Value &entries) {

            //Gnuplot::set_terminal_std("dumb");
            Gnuplot g1("linespoints");
            g1.remove_tmpfiles();
            g1.set_ylabel(units);
            g1.set_xlabel("time");
            g1.cmd("set xdata time");
            g1.cmd("set timefmt '%Y-%m-%dT%H:%M:%S'");
            g1.cmd("set timefmt '%Y-%m-%dT%H:%M:%S'");

            if (plot.empty()) {
                if (output.find(string("jpg")) != std::string::npos) {
                    g1.cmd("set terminal 'jpeg' size 900,500 nocrop enhanced font 'Verdana,10' size 900,500 background '#FFFCF5'");
                } else if (output.find(string("jpeg")) != std::string::npos) {
                    g1.cmd("set terminal 'jpeg' size 900,500 nocrop enhanced font 'Verdana,10' size 900,500 background '#FFFCF5'");
                } else if (output.find(string("png")) != std::string::npos) {
                    g1.cmd("set terminal 'pngcairo' nocrop enhanced font 'Verdana,10' size 900,500 background '#FFFCF5'");
                } else {
                    g1.cmd("set terminal 'dumb'");
                }
                // default gnuplot settings
                g1.cmd(
                        "unset key\n"
                                "set xlabel \"Time\" textcolor'#000000'\n"
                                "set ylabel \"" + units + "\" textcolor '#000000'\n"
                                "set label textcolor '#000000'\n"
                                "set border 10 lw 1 lc '#038DDD'\n"
                                "set grid lc '#038DDD'\n"
                                "set border 0 back ls 11\n"
                                "set grid\n"
                                "set autoscale y\n"
                                "set autoscale x\n"
                                "set xtics rotate by -45 offset -0.1,-0.1\n"
                                "set mxtics\n"
                                "set mytics\n");
                g1.set_style("linespoints linetype 1 lw 1 lc '#025072' pt 7 ps .5");
            } else {
                // load custom gnuplot
                g1.set_GNUPlotPath(plot);
            }

            vector<string> x;
            vector<double> y;
            string firstDT, lastDT;
            for (SizeType i = 0; i < entries.Size(); i++) {
                const Value &entry = entries[i];
                if (i == 0) {
                    firstDT = entry["dt"].GetString();
                }
                if (i == entries.Size() - 1) {
                    lastDT = entry["dt"].GetString();
                }
                x.push_back(entry["dt"].GetString());
                y.push_back(entry["value"].GetDouble());
            }
            g1.cmd("set xrange ['" + firstDT + "':'" + lastDT + "']");

//    auto maxY = std::max_element(std::begin(y), std::end(y));
//    auto minY = std::max_element(std::begin(y), std::end(y));
//    stringstream minV,maxV;
//    minV << *minY;
//    maxV << *maxY;
//    g1.cmd("set yrange ['" + minV.str() + "':'" + maxV.str() + "']");
            g1.cmd("set title '{/Verdana " + resource + ":" + name + "}'  ");

            if (!output.empty()) {
                g1.cmd("set output '" + output + "'");
            }

            g1.plot_xy(x, y);
        }
    };
}

