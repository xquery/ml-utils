#include <cstring>
#include <string>
#include <vector>
#include <istream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <map>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/pointer.h>

#include "../history.cpp"

#include <gnuplot_i.hpp>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
#include <conio.h>   //for getch(), needed in wait_for_key()
#include <windows.h> //for Sleep()
void sleep(int i) { Sleep(i*1000); }
#endif

#define SLEEP_LGTH 2  // sleep time in seconds
#define NPOINTS    50 // length of array

using namespace rapidjson;
using namespace std;

void doPlot(string output, string plot, string units, string name, string resource, const Value &entries) {

    //Gnuplot::set_terminal_std("dumb");
    Gnuplot g1("linespoints");
    g1.set_ylabel(units);
    g1.set_xlabel("time");
    g1.cmd("set xdata time");
    g1.cmd("set timefmt '%Y-%m-%dT%H:%M:%S'");
    g1.cmd("set timefmt '%Y-%m-%dT%H:%M:%S'");
    g1.cmd("set macro\n"
                   "red_000 = \"#F9B7B0\"\n"
                   "red_025 = \"#F97A6D\"\n"
                   "red_050 = \"#E62B17\"\n"
                   "red_075 = \"#8F463F\"\n"
                   "red_100 = \"#6D0D03\"\n"
                   "\n"
                   "blue_000 = \"#D2D225\"\n"
                   "blue_025 = \"#D2D225\"\n"
                   "blue_050 = \"#D2D225\"\n"
                   "blue_075 = \"#D2D225\"\n"
                   "blue_100 = \"#D2D225\" \n"
                   "\n"
                   "green_000 = \"#A6EBB5\"\n"
                   "green_025 = \"#67EB84\"\n"
                   "green_050 = \"#11AD34\"\n"
                   "green_075 = \"#2F6C3D\"\n"
                   "green_100 = \"#025214\"\n"
                   "\n"
                   "brown_000 = \"#F9E0B0\"\n"
                   "brown_025 = \"#F9C96D\"\n"
                   "brown_050 = \"#E69F17\"\n"
                   "brown_075 = \"#8F743F\"\n"
                   "brown_100 = \"#6D4903\"\n"
                   "\n"
                   "grid_color = \"#d5e0c9\"\n"
                   "text_color = \"#6a6a6a\"");

    g1.cmd("my_line_width = \"2\"\n"
                   "my_axis_width = \"1.5\"\n"
                   "my_ps = \"1.2\"\n"
                   "set pointsize my_ps\n"
                   "\n"
                   "set style line 1 linecolor rgbcolor blue_025 linewidth @my_line_width pt 7\n"
                   "set style line 2 linecolor rgbcolor green_025 linewidth @my_line_width pt 5\n"
                   "set style line 3 linecolor rgbcolor red_025 linewidth @my_line_width pt 9\n"
                   "set style line 4 linecolor rgbcolor brown_025 linewidth @my_line_width pt 13\n"
                   "set style line 5 linecolor rgbcolor blue_050 linewidth @my_line_width pt 11\n"
                   "set style line 6 linecolor rgbcolor green_050 linewidth @my_line_width pt 7\n"
                   "set style line 7 linecolor rgbcolor red_050 linewidth @my_line_width pt 5\n"
                   "set style line 8 linecolor rgbcolor brown_050 linewidth @my_line_width pt 9\n"
                   "set style line 9 linecolor rgbcolor blue_075 linewidth @my_line_width pt 13\n"
                   "set style line 10 linecolor rgbcolor green_075 linewidth @my_line_width pt 11\n"
                   "set style line 11 linecolor rgbcolor red_075 linewidth @my_line_width pt 7\n"
                   "set style line 12 linecolor rgbcolor brown_075 linewidth @my_line_width pt 5\n"
                   "set style line 13 linecolor rgbcolor blue_100 linewidth @my_line_width pt 9\n"
                   "set style line 14 linecolor rgbcolor green_100 linewidth @my_line_width pt 13\n"
                   "set style line 15 linecolor rgbcolor red_100 linewidth @my_line_width pt 11\n"
                   "set style line 16 linecolor rgbcolor brown_100 linewidth @my_line_width pt 7\n"
                   "set style line 17 linecolor rgbcolor \"#224499\" linewidth @my_line_width pt 5");
    g1.cmd("set style increment user");
    g1.cmd("set xtics textcolor rgb text_color\n"
                   "set ytics textcolor rgb text_color\n"
                   "set ztics textcolor rgb text_color");
    g1.cmd("set xlabel \"Time\" textcolor rgb text_color\n"
               "set ylabel \""+units+"\" textcolor rgb text_color\n"
               "set label textcolor rgb text_color\n"
               "set border 31 lw @my_axis_width lc rgb text_color\n"
               "set key outside box width 2 height 2 enhanced spacing 2\n"
               "set grid lc rgb grid_color");

    g1.cmd("set border 0 back ls 11\n"
                   "unset key\n"
                   "set mxtics\n"
                   "set mytics\n"
                   "set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 ps 1.5   # --- blue\n"
                  );

    if (plot.empty()) {
        // default gnuplot settings
        if (output.find(string("jpg")) != std::string::npos) {
            g1.cmd("set terminal 'jpeg' size 900,500");
        } else if (output.find(string("jpeg")) != std::string::npos) {
            g1.cmd("set terminal 'jpeg' size 900,500");
        } else if (output.find(string("png")) != std::string::npos) {
            g1.cmd("set terminal 'pngcairo' nocrop enhanced font 'verdana,8' size 900,500 background '#C7C7B6'");
        } else {
            g1.cmd("set terminal 'dumb'");
        }
        g1.cmd("set object 1 rectangle from screen 0,0 to screen 1,1 fillcolor '#FBFBF7' behind");

    } else {
        // load custom gnuplot
        cout << "loading " << plot << endl;
        //g1.cmd("load '" + plot + "'");
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
    g1.cmd("set title '{/Verdana:Bold " + resource + ":" + name + "}'  ");

    if(!output.empty()){
        g1.cmd("set output '" + output + "'");
    }

    g1.plot_xy(x, y);
}

int main(int argc, char *argv[]) {

    try {
        History history;
        history.setCurrentArgs(history.options(argc, argv));
        CommandLineArgs current = history.getCurrentArgs();
        Config config = history.getConfig();

        if (current.verbose) {
            cout << "----------------" << endl;
            cout << "~/.ml-utils" << endl;
            cout << "----------------" << endl;
            cout << "config: " << current.config << endl;
            cout << "----------------" << endl;
            cout << "user: " << config.user << endl;
            cout << "pass: " << config.pass << endl;
            cout << "host: " << config.host << endl;
            cout << "----------------" << endl;
            cout << "options" << endl;
            cout << "----------------" << endl;
            cout << "format: " << current.format << endl;
            cout << "period: " << current.period << endl;
            cout << "start: " << current.start << endl;
            cout << "end: " << current.end << endl;
            cout << "metric: " << current.metric << endl;
            cout << "resource: " << current.resource << endl;
            cout << "output: " << current.output << endl;
            cout << "gnuplot: " << current.gnuplot << endl;
        }

        history.setUrl("8002", "/manage/v2", current.resource, "metrics");

        history.execute();

        string result = history.getReadBuffer();

        string graph_prefix = "graph";
        string output = current.output;
        string plot = string(current.gnuplot);
        string resource = string(current.resource);

        if (!plot.empty() || !output.empty()) {
            Document doc;
            doc.Parse(result.c_str());
            string res, units, name;

            if (resource.find(string("databases")) != std::string::npos) {
                res = "database-metrics-list";
                units = doc[res.c_str()]["metrics-relations"]["database-metrics-list"]["metrics"][0]["master"][0][current.metric]["units"].GetString();
                name = doc[res.c_str()]["metrics-relations"]["database-metrics-list"]["metrics"][0]["master"][0][current.metric]["name"].GetString();
                const Value &entries = doc[res.c_str()]["metrics-relations"]["database-metrics-list"]["metrics"][0]["master"][0][current.metric]["summary"]["data"]["entry"];
                doPlot(output, current.gnuplot, units, name, string(current.resource), entries);
            } else if (resource.find(string("servers")) != std::string::npos) {
                res = "server-metrics-list";
                units = doc[res.c_str()]["metrics-relations"][res.c_str()]["metrics"][0][current.metric]["units"].GetString();
                name = doc[res.c_str()]["metrics-relations"][res.c_str()]["metrics"][0][current.metric]["name"].GetString();
                const Value &entries = doc[res.c_str()]["metrics-relations"]["server-metrics-list"]["metrics"][0][current.metric]["summary"]["data"]["entry"];
                doPlot(output, current.gnuplot, units, name, string(current.resource), entries);
            } else if (resource.find(string("forests")) != std::string::npos) {
                res = "forest-metrics-list";
                units = doc[res.c_str()]["metrics-relations"][res.c_str()]["metrics"][0][current.metric]["units"].GetString();
                name = doc[res.c_str()]["metrics-relations"][res.c_str()]["metrics"][0][current.metric]["name"].GetString();
                const Value &entries = doc[res.c_str()]["metrics-relations"]["forest-metrics-list"]["metrics"][0][current.metric]["summary"]["data"]["entry"];
                doPlot(output, current.gnuplot, units, name, string(current.resource), entries);
            } else if (resource.find(string("hosts")) != std::string::npos) {
                res = "host-metrics-list";
                units = doc[res.c_str()]["metrics-relations"][res.c_str()]["metrics"][0][current.metric]["units"].GetString();
                name = doc[res.c_str()]["metrics-relations"][res.c_str()]["metrics"][0][current.metric]["name"].GetString();
                const Value &entries = doc[res.c_str()]["metrics-relations"]["host-metrics-list"]["metrics"][0][current.metric]["summary"]["data"]["entry"];
                doPlot(output, current.gnuplot, units, name, string(current.resource), entries);
            }
        }

        if (current.quiet) {

            if (strcmp(current.output, "csv") == 0) {
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
                cout << result << endl;
            } else {
                cout << result << endl;
            }
        }

    } catch (std::bad_alloc) {
        cout << "Error with mlhistory" << endl;
    }
    return EXIT_SUCCESS;
}

