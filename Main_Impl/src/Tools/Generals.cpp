#define __STDC_FORMAT_MACROS
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <Generals.h>

using namespace std;
namespace ExtractConf
{

        void loadConfiguration(Info &config, std::string config_file)
        {
                ifstream fin(config_file);
                string line;
                config.RTsPlugin = (char *)malloc(sizeof(char *) * 11);
                int i =0;
                while(config.RTsPlugin[i]!= '\0'){
                        config.RTsPlugin[i] == '0';
                        i++;
                }
                while (getline(fin, line))
                { // add explanation to config and ignore them here
                        if (line.rfind("#", 0) == 0)
                                continue;
                        istringstream sin(line.substr(line.find("=") + 1));
                        if (line.find("mainthreshold") != -1)
                                sin >> config.mainthreshold;
                        else if (line.find("pcap_file") != -1)
                                sin >> config.pcap_file;
                        // else if (line.find("data_size") != -1)
                        //         sin >> config.data_size;
                        else if (line.find("Train") != -1)
                                sin >> config.Train;
                        else if (line.find("interface") != -1)
                                sin >> config.interface;
                        // else if (line.find("RTsPlugin") != -1)
                        //         sin >> config.RTsPlugin;
                        else if (line.find("classify") != -1)
                                sin >> config.classify;
                        else if (line.find("resultsFile") != -1)
                                sin >> config.resultsFile;
                        else if (line.find("trainingFile") != -1)
                                sin >> config.trainingFile;
                        else if (line.find("GTfile") != -1)
                                sin >> config.GTfile;
                        else if (line.find("ClassificationScriptPy") != -1)
                                sin >> config.ClassificationScriptPy;
                        else if (line.find("python_object_path") != -1)
                                sin >> config.python_object_path;
                        else if (line.find("reportTime") != -1)
                                sin >> config.reportTime;
                        else if (line.find("FrequencyTest") != -1)
                                config.RTsPlugin[0] = 't';
                        else if (line.find("BlockFreqTest") != -1)
                                config.RTsPlugin[1] = 't';
                        else if (line.find("SerialTest") != -1)
                                config.RTsPlugin[2] = 't';
                        else if (line.find("CumulativeSumTest") != -1)
                                config.RTsPlugin[3] = 't';
                        else if (line.find("ApproximateEntropyTest") != -1)
                                config.RTsPlugin[4] = 't';
                        else if (line.find("DescreteFourierTransformTest") != -1)
                                config.RTsPlugin[5] = 't';
                        else if (line.find("LongestRunsOfOnesTest") != -1)
                                config.RTsPlugin[6] = 't';
                        else if (line.find("RunsTest") != -1)
                                config.RTsPlugin[7] = 't';
                        else if (line.find("TopologicalBinaryTest") != -1)
                                config.RTsPlugin[8] = 't';
                        else if (line.find("GreatesCommonDivisorTest") != -1)
                                config.RTsPlugin[9] = 't';
                        else if (line.find("BoockStackTest") != -1)
                                config.RTsPlugin[10] = 't';
                }
                if (config.resultsFile == "" || config.pcap_file == "" || config.ClassificationScriptPy == "" || config.python_object_path == "" || config.Train == -1 || config.classify == -1 || config.mainthreshold == -1 || config.RTsPlugin == "")
                {
                        std::cerr << "Config file contains uncorrect values.";
                        exit(-1);
                }
        }



} 

// #Interface to be used to capture traffic. if you leave it empty, pcap file will be used.
// interface = 
// #Path to pcap file 
// pcap_file = /mnt/c/Users/ahmad/Desktop/Thesis/pcap_files/t1.pcap

// # threshold of number of packets for each flow based on the features will be extracted
// mainthreshold = 5
// #Number of Randomness Tests: THIS HAS TO BE MATCHED WITH THE NUMBER OF RTS USED IN IMPL
// #- commenting any <RT>_plugin function in RTHandler means reducing this by one
// numOfRTs = 11
// # for training phase train is 1
// training = 1
// # for classification phase classify is 1
// classify = 1
// ##########Note that it is possible to do training and classification at the same time
// #path to file which saves results (fivetuple, label, pred) in (if classifiy and training are 1 the file will contain only the classification results)
// resultsFile = ./results/tt1Results.csv
// #path to trainingfile saves the finalized ground truth which can be used for ANN training later (features,label)
// trainingFile = ./results/tt1training.csv
// #GTfile contains flow info fivetuple,<otherinfo>,labels (will be translated to single label)
// GTfile = ./results/t1id.csv
// #ClassificationScriptPy this is the python module will be used from inside cpp to do the classification and
// #return prediction value. It has two main functions one to load the (saved) ML model and second to do prediction
// ClassificationScriptPy = classificationInPy
// #python_object_path the absolute path to the saved ML model (after training)
// #is loaded and used in classification phase
// python_object_path = /home/ahmad/structureBuilder/ANNinPython/t2trainedModel_10X30.h5
// int processed_flows = 0;
