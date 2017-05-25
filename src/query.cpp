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

    Query::Query() {
    };

    Query::~Query() {
    };

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
}