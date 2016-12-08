#pragma once

#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <curl/curl.h>
#include <sstream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <wchar.h>
#include <vector>
#include <map>
#include <cassert>

#include "config.hpp"

struct CommandLineArgs {
    CommandLineArgs()
            : config(""),
              format(""),
              period(""),
              start(""),
              end(""),
              regex(""),
              metric(""),
              filename(""),
              resource(""),
              uri(""),
              database(""),
              group(""),
              host(""),
              xquery(""),
              js(""),
              output(""),
              gnuplot(""),
              quiet(true),
              verbose(false),
              raw(false) {
    }

    void check(const char *progname) {}

    const char *config, *format, *period, *filename, *uri, *start, *end, *regex, *metric, *resource, *database, *group, *host, *xquery, *js, *output, *gnuplot;
    bool quiet, verbose, raw;
};

// interface for commands
class Command {
private:
    Config config;
    CommandLineArgs current;
    string readBuffer;

public:
    string url;

    Command() {
        loadConfig(config, current.config);
    };

    virtual ~Command() {
        //std::cout << "Default destructor called\n";
    };

    virtual int usage(const char *progname) =0;

    virtual CommandLineArgs options(int n_opts, char *opts[]) = 0;

    virtual string getReadBuffer() {
        return this->readBuffer;
    }

    virtual CommandLineArgs getCurrentArgs() {
        return this->current;
        loadConfig(config, this->current.config);
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

    virtual void setLoadUrl(string port,
                           string root,
                           string uri)
    {
        checkConfig();

        url = config.protocol + "://" + config.host + ":" + port + root+"?" ;
        if (!uri.empty()) {
            url += "uri=" + uri;
        }
        string database = current.database;
        if (!database.empty()) {
            url += "&database=" + database;
        }
    }

    virtual void setLogUrl(string port,
                        string root,
                        string filename,
                        string start,
                        string end,
                        string regex,
                        string host)
    {
        checkConfig();

        url = config.protocol + "://" + config.host + ":" + port + root+"?" ;
        if (!filename.empty() ) {
            url += "filename=" + filename;
        }
        if (!start.empty() ) {
            url += "&start=" + start;
        }
        if (!end.empty() ) {
            url += "&end=" + end;
        }
        if (!regex.empty() ) {
            url += "&regex=" + regex;
        }
        if (!host.empty() ) {
            url += "&host=" + host;
        }
        string format = current.format;
        if (format != "") {
            url += "&format=" + format;
        }
    }

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

    virtual int execute() {

        CURLM *curlm;
        int handle_count;
        curlm = curl_multi_init();

        CURL *curl1 = NULL;
        curl1 = curl_easy_init();

        curl_multi_setopt(curlm, CURLMOPT_PIPELINING, 0L);

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Accept: text/html");
        headers = curl_slist_append(headers, "Accept: text/plain");
        headers = curl_slist_append(headers, "Accept: application/xml");
        headers = curl_slist_append(headers, "Accept: application/js");
        headers = curl_slist_append(headers, "Accept: application/x-www-form-urlencoded");

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

            curl_easy_setopt(curl1, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
            curl_easy_setopt(curl1, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl1, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl1, CURLOPT_HTTPGET, 1L);
            curl_easy_setopt(curl1, CURLOPT_NOPROGRESS, 1L);
            curl_easy_setopt(curl1, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl1, CURLOPT_WRITEDATA, &readBuffer);

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

    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
        ((std::string *) userp)->append((char *) contents, size * nmemb);
        return size * nmemb;
    }


    virtual int executeQueryPost(string type, string query) {

        CURLM *curlm;
        int handle_count;
        curlm = curl_multi_init();

        CURL *curl1 = NULL;
        curl1 = curl_easy_init();

        curl_multi_setopt(curlm, CURLMOPT_PIPELINING, 0L);


        struct curl_slist *headers = NULL; // init to NULL is important
        headers = curl_slist_append(headers, "Accept: text/plain");
        headers = curl_slist_append(headers, "Accept: text/html");
        headers = curl_slist_append(headers, "Accept: application/xml");
        headers = curl_slist_append(headers, "Accept: application/js");
        headers = curl_slist_append(headers, "Accept: application/x-www-form-urlencoded");

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
            curl_easy_setopt(curl1, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl1, CURLOPT_URL, url.c_str());

            char *data = curl_easy_escape(curl1, query.c_str(), strlen(query.c_str()));
            string xquery = type + "=" + data;
            curl_easy_setopt(curl1, CURLOPT_POST, 1);
            curl_easy_setopt(curl1, CURLOPT_POSTFIELDS, xquery.c_str());
            curl_easy_setopt(curl1, CURLOPT_NOPROGRESS, 1L);

            curl_easy_setopt(curl1, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl1, CURLOPT_WRITEDATA, &readBuffer);

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


    virtual int executeLoadPost(const char* file, string body) {

        FILE *fd;

        fd = fopen(file, "rb"); /* open file to upload */

        CURLM *curlm;
        int handle_count;
        curlm = curl_multi_init();

        CURL *curl1 = NULL;
        curl1 = curl_easy_init();

        curl_multi_setopt(curlm, CURLMOPT_PIPELINING, 0L);

        struct curl_slist *headers = NULL; // init to NULL is important
        headers = curl_slist_append(headers, "Accept: text/plain");
        headers = curl_slist_append(headers, "Accept: text/html");
        headers = curl_slist_append(headers, "Accept: application/xml");
        headers = curl_slist_append(headers, "Accept: application/js");
        headers = curl_slist_append(headers, "Accept: application/x-www-form-urlencoded");

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
            //std::cout << readBuffer << std::endl;
        }

        curl_global_cleanup();

        return EXIT_SUCCESS;

    };

};
