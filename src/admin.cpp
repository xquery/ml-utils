#include <cstring>
#include <string>
#include <vector>
#include <istream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <map>
#include <dirent.h>

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
        }else if (optstr == "--ml-config") {
            ++i;
            args.cmlconfig = opts[i];
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
         << "    --ml-config : ml-config directory (ex. etc/ml-config)\n"
         << "    -c          : config file (ex. /home/jfuller/.ml-utils\n"
         << "    -p          : payload path \n"
         << "    -n          : name \n"
         << "    -r          : {resources}/{resource-name} directory\n"
         << "    -v          : verbose (show http call)\n"
         << "    -q          : quiet (suppress banner)" << endl;
    return EXIT_SUCCESS;
}

int Admin::walkInstallConfig(Admin admin, Config config, string path){
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (path.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            string name = ent->d_name;
            string fullpath = path + "/" + name;
            if (name != "." && name != ".." && name != ".DS_Store") {
                if (name.find(".json") != std::string::npos) {
                    if (fullpath.find("forests") != std::string::npos) {
                        cout << "installing forest" << endl;
                        admin.setResourceUrl(config.port, config.path, "forests");
                        admin.executeInstallPost(fullpath, "json");
                    } else if (fullpath.find("databases") != std::string::npos) {
                        cout << "installing database" << endl;
                        admin.setResourceUrl(config.port, config.path, "databases");
                        admin.executeInstallPost(fullpath, "json");
                    } else if (fullpath.find("servers") != std::string::npos) {
                        cout << "installing server" << endl;
                        admin.setResourceUrl(config.port, config.path, "servers");
                        admin.executeInstallPost(fullpath, "json");
                    }else if (fullpath.find("amps") != std::string::npos) {
                        cout << "installing amps" << endl;
                        admin.setResourceUrl(config.port, config.path, "amps");
                        admin.executeInstallPost(fullpath, "json");
                    }else if (fullpath.find("external-securities") != std::string::npos) {
                        cout << "installing external-securities" << endl;
                        admin.setResourceUrl(config.port, config.path, "external-security");
                        admin.executeInstallPost(fullpath, "json");
                    }else if (fullpath.find("privileges") != std::string::npos) {
                        cout << "installing privileges" << endl;
                        admin.setResourceUrl(config.port, config.path, "privileges");
                        admin.executeInstallPost(fullpath, "json");
                    }else if (fullpath.find("security/roles") != std::string::npos) {
                        cout << "installing roles" << endl;
                        admin.setResourceUrl(config.port, config.path, "roles");
                        admin.executeInstallPost(fullpath, "json");
                    }else if (fullpath.find("users") != std::string::npos) {
                        cout << "installing users" << endl;
                        admin.setResourceUrl(config.port, config.path, "users");
                        admin.executeInstallPost(fullpath, "json");
                    }else if (fullpath.find("protected-paths") != std::string::npos) {
                        cout << "installing protected-paths" << endl;
                        admin.setResourceUrl(config.port, config.path, "protected-paths");
                        admin.executeInstallPost(fullpath, "json");
                    }
                }else if (name.find(".xml") != std::string::npos) {
                    cout << "ignoring xml properties" << endl;
                }else{
                    walkInstallConfig(admin, config, fullpath);
                }
            }
        }
        closedir (dir);
    } else {
        /* could not open directory */
        cerr << "problem installing" << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}