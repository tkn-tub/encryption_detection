
#include <stdio.h>
#include <assert.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
//+++for printing with ss using stringstream type
#include <iostream>
#include <sstream>
#include "boost/asio/buffer.hpp" //to be installed and tested
#include <stack>
#include <RInside.h> // install.packages("RInside")

void tbtInR(RInside& Rqm, const char *hexPayloadString, double& pValue)
{


    double pValue_v[2];
    // std::cout << hexPayloadString << std::endl;
    Rqm["input"] = hexPayloadString;
    std::string str =
        "library(\"modules\"); "
        "mt <- modules::use(\"/home/ahmad/structureBuilder/Rtests/tbt.R\"); "
        "tbtOutput <- mt$tbtfunc(input); ";

    Rqm.parseEvalQ(str);
    Rcpp::NumericVector v = Rqm.parseEval(str);
    pValue = v[0]; //it gets only the last value returned from str
    std::string str2 = "gc(); ";
    Rqm.parseEvalQ(str2);


}

void gcdInR(RInside& Rqm, const char *hexPayloadString, double& pValue)
{


    double pValue_v[2];
    Rqm["input"] = hexPayloadString;
    std::string str =
        "library(\"modules\"); "
        "mg <- modules::use(\"/home/ahmad/structureBuilder/Rtests/gcd.R\"); "
        "gcdOutput <-mg$gcdfunc(input); ";
    Rqm.parseEvalQ(str);
    Rcpp::NumericVector v = Rqm.parseEval(str);
    pValue = v[0]; //it gets only the last value returned from str
    std::string str2 = "gc(); ";
    Rqm.parseEvalQ(str2);
}

void bookStackInR(RInside& Rqm, const char *hexPayloadString, double& pValue)
{


    double pValue_v[2];
    Rqm["input"] = hexPayloadString;
    std::string str =
        "library(\"modules\"); "
        "mb <- modules::use(\"/home/ahmad/structureBuilder/Rtests/bookstack.R\"); "
        "bookStackOutput <- mb$bookstackfunc(input); ";

    Rqm.parseEvalQ(str);
    Rcpp::NumericVector v = Rqm.parseEval(str);
    pValue = v[0]; //it gets only the last value returned from str
    std::string str2 = "gc(); ";
    Rqm.parseEvalQ(str2);
}