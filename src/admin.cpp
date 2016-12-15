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
        }else if (optstr == "--port") {
            ++i;
            args.format = opts[i];
        }else if (optstr == "--user") {
            ++i;
            args.cuser = opts[i];
        }else if (optstr == "--pass") {
            ++i;
            args.cpass = opts[i];
        }else if (optstr == "--path") {
            ++i;
            args.cpath = opts[i];
        }else if (optstr == "--protocol") {
            ++i;
            args.cprotocol = opts[i];
        }else if (optstr == "-f") {
            ++i;
            args.format = opts[i];
        }else if (optstr == "-n") {
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
         << "   [command] : restart | get | get-properties | create | update | delete | install\n"
         << "    -c       : config file (ex. /home/jfuller/.ml-utils\n"
         << "    -p       : payload path \n"
         << "    -n       : name \n"
         << "    -r       : {resources}/{resource-name} directory\n"
         << "    -v       : verbose (show http call)\n"
         << "    -q       : quiet (suppress banner)" << endl;
    return EXIT_SUCCESS;
}