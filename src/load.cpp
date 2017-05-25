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

    Load::Load() {//std::cout << "load constructor called\n";
    };

    Load::~Load() {//std::cout << "load destructor called\n";
    };

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
}