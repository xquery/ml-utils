#include <cstring>
#include <string>
#include <vector>
#include <istream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <map>

#include "../load.cpp"

int main(int argc, char* argv[]) {

  try{
  Load load;
  load.setCurrentArgs(load.options(argc,argv));
  CommandLineArgs current = load.getCurrentArgs();
  Config config = load.getConfig();
  
  if(current.quiet == false){
    cout << "ml-utils: ml-load 1.0 | copyright (c)2015 Jim Fuller | see https://github.com/xquery/ml-utils" << endl;
  }
    
  if(current.verbose){
    cout << "----------------" << endl;
    cout << "~/.ml-utils" << endl;
    cout << "----------------" << endl;
    cout << "user: " <<config.user << endl;
    cout << "pass: " <<config.pass << endl;
    cout << "host: " <<config.host << endl;
    cout << "----------------" << endl;
    cout << "options" << endl;
    cout << "----------------" << endl;
    cout << "format: " << current.format <<endl;
    cout << "resource: " << current.resource <<endl;
    }

  load.setUrl("8000","/v1/documents","","");
  
  return load.executeLoadPost();
    
  }catch(std::bad_alloc){
    cout << "Error with mlhistory" << endl;
  }
  return EXIT_SUCCESS;
}
