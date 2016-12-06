#include <cstring>
#include <string>
#include <vector>
#include <istream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <map>

#include "../status.cpp"

int main(int argc, char* argv[]) {

  try{
  Status status;
  status.setCurrentArgs(status.options(argc,argv));
  CommandLineArgs current = status.getCurrentArgs();
  Config config = status.getConfig();
  
  if(current.quiet == false){
    cout << "ml-utils: mlstatus 1.0 | copyright (c)2015 Jim Fuller | see https://github.com/xquery/ml-utils" << endl;
  }
    
  if(current.verbose != false){
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

  status.setUrl("8002","/manage/v2",current.resource,"status");

  status.execute();
  string result = status.getReadBuffer();

  if(strcmp (current.format,"json") == 0){
    cout<< result <<endl;
  }else{
    cout<< result <<endl;
  }

    
  }catch(std::bad_alloc){
    cout << "Error with mlhistory" << endl;
  }
  return EXIT_SUCCESS;
}
