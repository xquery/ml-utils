#include <cstring>
#include <string>
#include <vector>
#include <istream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <map>
#include <ctime>

#include "history.hpp"

using namespace std;

History::History(){
  //std::cout << "history constructor called\n";
};
History::~History(){
  //std::cout << "history destructor called\n";
};

CommandLineArgs History::options(int n_opts,char *opts[])
{
  CommandLineArgs args;

  args.period="hour";
  args.start="";
  args.end="";
  
  for(int i = 1; i < n_opts; ++i) {
      if(opts[i][1] == 'f') {
        ++i;
        args.format = opts[i];
      }
      else if(opts[i][1] == 'p') {
        ++i;
        args.period = opts[i];
      }
      else if(opts[i][1] == 's') {
        ++i;
        args.start = opts[i];
      }
      else if(opts[i][1] == 'e') {
        ++i;
        args.end = opts[i];
      }
      else if(opts[i][1] == 'm') {
        ++i;
        args.metric = opts[i];
      }
      else if(opts[i][1] == 'r') {
        ++i;
        args.resource = opts[i];
      }
      else if(opts[i][1] == 'o') {
        ++i;
        args.output = opts[i];
      }
      else if(opts[i][1] == 'g') {
        ++i;
        args.gnuplot = opts[i];
      }
      else if(opts[i][1] == 'q') {
        args.quiet = true;
      }
      else if(opts[i][1] == 'v') {
        args.verbose = true;
      }
      else if(opts[i][1] == 'h') {
        usage(opts[0]);
        exit(1);
      }
      else if(opts[i][1] == 'c') {
          ++i;
          args.config = opts[i];
      }
      else {
        //usage(opts[0]);
      }
    }
    args.check(opts[0]);
    return args;
}

int History::usage(const char *progname)
{
    const char *name = progname;
    while(*progname != 0) {
        if(*progname == '/' || *progname == '\\') {
            ++progname;
            name = progname;
        } else {
            ++progname;
        }
    }

    cout << "ml-utils: ml-hist 1.0 | copyright (c)2015 Jim Fuller | see https://github.com/xquery/ml-utils" << endl;
    cout << "retrieve MarkLogic history metrics" << endl;
    cout << ">" << name << " resources/{resource-name} [options] " << endl;
    cout << "    -c    : config file (ex. /home/jfuller/.ml-utils)" << endl;
    cout << "    -f    : format (xml|json)" << endl;
    cout << "    -p    : period (raw|hour|day)" << endl;
    cout << "    -s    : start date (ex. 2015-03-21T17:38:00)" << endl;
    cout << "    -e    : end date (ex. 2015-03-22T17:58:00)" << endl;
    cout << "    -m    : meter (resource specific)" << endl;
    cout << "    -r    : resource (databases|forests|servers|hosts) / resource-name" << endl;
    cout << "    -o    : graph output" << endl;
    cout << "    -g    : custom gnuplot script" << endl;
    cout << "    -v    : verbose (show http call)" << endl;
    cout << "    -q    : quiet (suppress banner)" << endl;
    return EXIT_SUCCESS;
}