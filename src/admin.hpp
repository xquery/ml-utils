#pragma once

#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
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

namespace mlutil {
    class Admin : public Command {
    public:
        Admin();

        virtual ~Admin();

        virtual CommandLineArgs options(int n_opts, char *opts[]);

        virtual int usage(const char *progname);

        virtual int walkInstallConfig(Admin admin, Config config, string path);
    };
}