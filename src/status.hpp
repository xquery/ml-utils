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

#include "command.hpp"

using namespace std;

class Status : public Command {
public:
    Status();
    virtual ~Status();
    virtual CommandLineArgs options(int n_opts, char *opts[]);
    virtual int usage(const char *progname);
};


