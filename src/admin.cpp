#include <cstring>
#include <string>
#include <vector>
#include <istream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <map>

#include "admin.hpp"

Admin::Admin() {
};

Admin::~Admin() {
};

CommandLineArgs Admin::options(int n_opts, char *opts[]) {
    CommandLineArgs args;

    for (int i = 1; i < n_opts; ++i) {
        string optstr = opts[i];
        if(optstr == "--host"){
            ++i;
            args.chost = opts[i];
        }else if (opts[i][1] == '--port') {
            ++i;
            args.format = opts[i];
        }else if (opts[i][1] == '--user') {
            ++i;
            args.cuser = opts[i];
        }else if (opts[i][1] == '--pass') {
            ++i;
            args.cpass = opts[i];
        }else if (opts[i][1] == '--path') {
            ++i;
            args.cpath = opts[i];
        }else if (opts[i][1] == '--protocol') {
            ++i;
            args.cprotocol = opts[i];
        }else if (opts[i][1] == 'f') {
            ++i;
            args.format = opts[i];
        } else if (opts[i][1] == 'q') {
            args.quiet = true;
        } else if (opts[i][1] == 'r') {
            ++i;
            args.resource = opts[i];
        } else if (opts[i][1] == 'c') {
            ++i;
            args.config = opts[i];
        } else if (opts[i][1] == 'v') {
            args.verbose = true;
        } else if (opts[i][1] == 'h') {
            usage(opts[0]);
            exit(1);
        } else {
            args.command= opts[i];
        }
    }
    args.check(opts[0]);
    return args;
}

int Admin::usage(const char *progname) {
    const char *name =  getprogname(progname);
    cerr << "ml-utils: ml-config 1.0 | copyright (c)2015 Jim Fuller | see https://github.com/xquery/ml-utils\n"
         << "configure MarkLogic\n"
         << ">" << name << " command [options]\n"
         << "   [command] : restart-local-cluster | get | get-properties | create | update | delete | install\n"
         << "    -c       : config file (ex. /home/jfuller/.ml-utils\n"
         << "    -r       : {resources}/{resource-name} directory\n"
         << "    -v       : verbose (show http call)\n"
         << "    -q       : quiet (suppress banner)" << endl;
    return EXIT_SUCCESS;
}