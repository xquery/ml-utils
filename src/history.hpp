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

class History : public Command
{
public:
  History();
  virtual ~History();
  virtual int usage(const char *progname);
  virtual CommandLineArgs options(int n_opts, char *opts[]);
};
