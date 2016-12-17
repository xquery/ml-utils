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

#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else

#include <unistd.h>

#define GetCurrentDir getcwd
#endif

char pPath[FILENAME_MAX];

using namespace std;

bool is_file_exist(string fileName) {
    std::ifstream infile(fileName);
    return infile.good();
}

struct Config {
    string user;
    string pass;
    string host;
    string protocol;
    string port;
    string path;
    string mlconfig;
};

void loadConfig(Config &config, string path) {
    std::string homepath = getenv("HOME");
    GetCurrentDir(pPath, sizeof(pPath));
    std::string currentpath = pPath;
    std::string mlutils;

    if (is_file_exist(path)) {
        mlutils = (path).c_str();
    } else if (is_file_exist(currentpath + "/.ml-utils")) {
        mlutils = (currentpath + "/.ml-utils").c_str();
    } else if (is_file_exist(homepath + "/.ml-utils")) {
        mlutils = (homepath + "/.ml-utils").c_str();
    } else {
        mlutils = (path).c_str();
    }

    ifstream fin(mlutils);
    std:
    string line;

    while (getline(fin, line)) {
        istringstream sin(line.substr(line.find("=") + 1));
        if (line.find("user") != -1)
            sin >> config.user;
        else if (line.find("pass") != -1)
            sin >> config.pass;
        else if (line.find("host") != -1)
            sin >> config.host;
        else if (line.find("protocol") != -1)
            sin >> config.protocol;
        else if (line.find("port") != -1)
            sin >> config.port;
        else if (line.find("ml-config") != -1)
            sin >> config.mlconfig;
        else if (line.find("path") != -1)
            sin >> config.path;    }
}
