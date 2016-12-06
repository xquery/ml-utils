#include <cstring>
#include <string>
#include <vector>
#include <istream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <map>

#include "status.hpp"

Status::Status(){
  //std::cout << "status constructor called\n";
};
Status::~Status(){
  //std::cout << "status destructor called\n";
};

int Status::usage(const char *progname)
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

  cerr << "" << name << " Resources/resource [options] - return resource status" << endl;
  cerr << "    -c    : config file (ex. /home/jfuller/.ml-utils)" << endl;
  cerr << "    -f    : format (xml|json)" << endl;
  cerr << "    -v    : verbose (show http call)" << endl;
  cerr << "    -q    : quiet (suppress banner)" << endl;
  return EXIT_SUCCESS;
}

CommandLineArgs Status::options(int n_opts,char *opts[])
{
  CommandLineArgs args;

  args.group="Default";
  
  for(int i = 1; i < n_opts; ++i) {
      if(opts[i][1] == 'f') {
        ++i;
        args.format = opts[i];
      }
      else if(opts[i][1] == 'q') {
        args.quiet = true;
      }
      else if(opts[i][1] == 'v') {
        args.verbose = true;
      }
      else if(opts[i][1] == 'r') {
        ++i;
        args.resource = opts[i];
      }
      else if(opts[i][1] == 'g') {
        ++i;
        args.group = opts[i];
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
