#include "../src/history.cpp"
#include <gtest/gtest.h>

TEST(History,Constructor){
  EXPECT_TRUE(new History());
}

TEST(History,all){
    History* hs = new History();
    char *argv[] = {"ml-hist", NULL};
    int argc = sizeof(argv) / sizeof(char*) - 1;
    hs->options(argc,argv);
    hs->setCurrentArgs(hs->options(argc,argv));
    CommandLineArgs current = hs->getCurrentArgs();
    Config config = hs->getConfig();
    hs->setUrl("8002","/manage/v2","forests","metrics");
    EXPECT_EQ(hs->getCurrentArgs().quiet,false);
    hs->execute();
    string result = hs->getReadBuffer().c_str();
    //EXPECT_FALSE(result.empty());
}

TEST(History,TestUsage){
    History* hs = new History();
    hs->usage("ml-hist");
}

