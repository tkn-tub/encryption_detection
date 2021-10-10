#include <stdlib.h>

#include <string.h>
#include <vector>
//+++for printing with ss using stringstream type
#include <iostream>
#include <sstream>
#include <RInside.h> // install.packages("RInside")
#include "boost/asio/buffer.hpp" //to be installed and tested
#include <stack>
using namespace std;
int chhex(char ch);
void from_hex_to_uchar_array(unsigned char *dest, const char *source, int bytes_n);
void update_payload_list(void *payload, size_t payload_length, char* arrTry);
unsigned char* hexstr_to_char(const char* hexstr);
void mean_standardDeviation(double a[], int n, double& stdev, double& mean);
void mainR(RInside& Rqm, const char *hexPayloadString, double& pValue);
void groundtruth_file_as_dict(string GTfile, std::map <string, string>& GTdict);
void match_flow_to_groundtruth(string fiveTupleFlow , std::map <string, string>& GTdict, int &label, vector<string>& nonFoundFlowsInGT, std::map <string, int>& labels_counters);
void print_undefined_label_flow_info(vector<string> NonFoundFlowsInGT, std::map <string, int>& labels_counters);
string get_five_tuple_as_cs(uint32_t src_ip, uint32_t dst_ip, uint8_t protocol, uint16_t src_p, uint16_t dst_p);
void build_training_file(string trainingFile, string features, int label, uint8_t numbOfRTs);
string get_features_as_cs(double* aNNInput, uint8_t numbOfRTs);
int classification(string ClassificationScriptPy, string python_object_path, string features);
void get_results_file(string resultsFile, string fiveTuple, int label, int prediction, uint8_t packetsNumber, uint64_t id_num);
void write_headers_Files(string trainingFile, char *RtsPlugIns, string reportTime, string resultsFile , string pcap_file);