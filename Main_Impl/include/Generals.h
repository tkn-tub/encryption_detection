#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>


namespace ExtractConf {

struct Info
{
        int mainthreshold =-1;
        char *RTsPlugin;
        std::string ml_model;
        std::string pcap_file;
        int Train =-1;
        int classify =-1;
        std::string interface;
        std::string resultsFile;
        std::string trainingFile;
        std::string GTfile;
        std::string ClassificationScriptPy;
        std::string python_object_path;
        std::string reportTime;

};

void loadConfiguration(Info &config, std::string config_file);
}
