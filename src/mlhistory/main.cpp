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
 * entry point for ml-hist - utility for retrieving historical metrics from MarkLogic server.
 */

#include <cstring>
#include <string>
#include <vector>
#include <istream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include <rapidjson/document.h>

#include "../history.cpp"

using namespace rapidjson;
using namespace std;
using namespace mlutil;


int main(int argc, char *argv[]) {

    try {
        History *pHistory = new History();
        pHistory->setCurrentArgs(pHistory->options(argc, argv));
        CommandLineArgs current = pHistory->getCurrentArgs();
        Config config = pHistory->getConfig();

        if (current.verbose) {
            pHistory->displayargs();
            pHistory->displayconfig();
        }
        pHistory->setUrl(config.port, config.path, current.resource, "metrics");
        pHistory->execute();

        string result = pHistory->getReadBuffer();

        string graph_prefix = "graph";
        string output = current.output;
        string plot = string(current.gnuplot);
        string resource = string(current.resource);

        if (result.empty()) {
            LOG_S(ERROR) << "no output.";
        }

        if (!plot.empty() || !output.empty()) {
            Document doc;
            doc.Parse(result.c_str());
            string res, units, name;
            if (resource.find(string("databases")) != std::string::npos) {
                res = "database-metrics-list";
                units = doc[res.c_str()]["metrics-relations"]["database-metrics-list"]["metrics"][0]["master"][0][current.metric]["units"].GetString();
                name = doc[res.c_str()]["metrics-relations"]["database-metrics-list"]["metrics"][0]["master"][0][current.metric]["name"].GetString();
                const Value &entries = doc[res.c_str()]["metrics-relations"]["database-metrics-list"]["metrics"][0]["master"][0][current.metric]["summary"]["data"]["entry"];
                pHistory->doPlot(output, current.gnuplot, units, name, string(current.resource), entries);
            } else if (resource.find(string("servers")) != std::string::npos) {
                res = "server-metrics-list";
                units = doc[res.c_str()]["metrics-relations"][res.c_str()]["metrics"][0][current.metric]["units"].GetString();
                name = doc[res.c_str()]["metrics-relations"][res.c_str()]["metrics"][0][current.metric]["name"].GetString();
                const Value &entries = doc[res.c_str()]["metrics-relations"]["server-metrics-list"]["metrics"][0][current.metric]["summary"]["data"]["entry"];
                pHistory->doPlot(output, current.gnuplot, units, name, string(current.resource), entries);
            } else if (resource.find(string("forests")) != std::string::npos) {
                res = "forest-metrics-list";
                units = doc[res.c_str()]["metrics-relations"][res.c_str()]["metrics"][0][current.metric]["units"].GetString();
                name = doc[res.c_str()]["metrics-relations"][res.c_str()]["metrics"][0][current.metric]["name"].GetString();
                const Value &entries = doc[res.c_str()]["metrics-relations"]["forest-metrics-list"]["metrics"][0][current.metric]["summary"]["data"]["entry"];
                pHistory->doPlot(output, current.gnuplot, units, name, string(current.resource), entries);
            } else if (resource.find(string("hosts")) != std::string::npos) {
                res = "host-metrics-list";
                units = doc[res.c_str()]["metrics-relations"][res.c_str()]["metrics"][0][current.metric]["units"].GetString();
                name = doc[res.c_str()]["metrics-relations"][res.c_str()]["metrics"][0][current.metric]["name"].GetString();
                const Value &entries = doc[res.c_str()]["metrics-relations"]["host-metrics-list"]["metrics"][0][current.metric]["summary"]["data"]["entry"];
                pHistory->doPlot(output, current.gnuplot, units, name, string(current.resource), entries);
            }
        }

        if (!current.quiet) {
            if (strcmp(current.output, "csv") == 0) {
                LOG_S(INFO) << "output csv ";

                Document doc;
                doc.Parse(result.c_str());
                string metricslist = "-metrics-list";
                string res = current.resource + metricslist;

                string units = doc["database-metrics-list"]["metrics-relations"]["database-metrics-list"]["metrics"][0]["master"][0][current.metric]["units"].GetString();

                string name = doc["database-metrics-list"]["metrics-relations"]["database-metrics-list"]["metrics"][0]["master"][0][current.metric]["name"].GetString();

                cout << "#mlhistory  " + name << endl;

                const Value &entries = doc["database-metrics-list"]["metrics-relations"]["database-metrics-list"]["metrics"][0]["master"][0][current.metric]["summary"]["data"]["entry"];
                vector<string> x;
                vector<double> y;
                for (SizeType i = 0; i < entries.Size(); i++) {
                    const Value &entry = entries[i];
                    cout << entry["dt"].GetString() + string(",");
                    cout << entry["value"].GetDouble() << endl;
                }
            } else if (strcmp(current.format, "json") == 0) {
                LOG_S(INFO) << "output json";
                cout << result << endl;
            } else {
                LOG_S(INFO) << "output xml";
                cout << result << endl;
            }
        }
    delete pHistory;
    } catch (std::bad_alloc) {
        LOG_S(ERROR) << "Error with ml-hist.";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

