import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import statistics
from configparser import ConfigParser
import re


configFile = 'config.ini'
conf = ConfigParser()
conf.read(configFile)
timeLogFile = conf['File_parameters']['TimeLog']
timeLogFile2 = conf['File_parameters']['TimeLog2']
stackedBar = conf['File_parameters']['StackedBarChartTimeComparison']

def extract_time_consumption(timeLog):
    datasetfile = pd.read_csv(timeLog, sep='\t', comment='#')
    RTpreprocessing = datasetfile['RTpre']
    RTFreq = datasetfile['Freq']
    RTBlockfreq = datasetfile['Blockfreq']
    RTSer = datasetfile['Ser']
    RTCumsum = datasetfile['Cumsum']
    RTApEnt = datasetfile['ApEnt']
    RTDFT = datasetfile['Dft']
    RTLruns = datasetfile['Lruns']
    RTRuns = datasetfile['Runs']
    RTTBT = datasetfile['TBT']
    RTGCD = datasetfile['GCD']
    RTBckstack = datasetfile['Bckstack']
    FGTmatch = datasetfile['GTmatch']
    ANNpredictions = datasetfile['Classification']
    LRTpreprocessing= []
    LRTFreq = []
    LRTBlockfreq = []
    LRTSer = []
    LRTCumsum = []
    LRTApEnt = []
    LRTDFT = []
    LRTLruns = []
    LRTRuns = []
    LRTTBT = []
    LRTGCD = []
    LRTBckstack = []
    LFGTmatch = []
    LANNpredictions = []



    NumberOFFlows = 0
    for i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13 , i14 in zip(RTpreprocessing, RTFreq, RTBlockfreq, RTSer, RTCumsum, RTApEnt, RTDFT, RTLruns, RTRuns, RTTBT, RTGCD, RTBckstack, FGTmatch, ANNpredictions):
        if NumberOFFlows == 0:
            print("This is the time consumption of the first flow which includes the initialization of RTs and ANN")
            print("RTpre:\t{},Freq:\t{},Blockfreq\t{},Ser\t{},Cumsum\t{},ApEnt\t{},Dft\t{},Lruns\t{},Runs\t{},TBT\t{},GCD\t{},Bckstack\t{},GTmatch\t{},Classification\t{}".format(i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13 , i14))
            NumberOFFlows+=1
        else:
            LRTpreprocessing.append(i1)
            LRTFreq.append(i2)
            LRTBlockfreq.append(i3)
            LRTSer.append(i4)
            LRTCumsum.append(i5)
            LRTApEnt.append(i6)
            LRTDFT.append(i7)
            LRTLruns.append(i8)
            LRTRuns.append(i9)
            LRTTBT.append(i10)
            LRTGCD.append(i11)
            LRTBckstack.append(i12)
            LFGTmatch.append(i13)
            LANNpredictions.append(i14)
            NumberOFFlows+=1
    print("Times Required to handle {} flows in MicroSeconds is :".format(NumberOFFlows-1))
    print("RTpre:\t{}\nFreq:\t{}\nBlockfreq\t{}\nSer\t{}\nCumsum\t{}\nApEnt\t{}\nDft\t{}\nLruns\t{}\nRuns\t{}\nTBT\t{}\nGCD\t{}\nBckstack\t{}\nGTmatch\t{}\nClassification\t{}".format(sum(LRTpreprocessing), sum(LRTFreq), sum(LRTBlockfreq), sum(LRTSer), sum(LRTCumsum), sum(LRTApEnt), sum(LRTDFT), sum(LRTLruns), sum(LRTRuns), sum(LRTTBT), sum(LRTGCD), sum(LRTBckstack), sum(LFGTmatch), sum(LANNpredictions)))
    RandomnessTest = [sum(LRTpreprocessing), sum(LRTFreq), sum(LRTBlockfreq), sum(LRTSer), sum(LRTCumsum), sum(LRTApEnt), sum(LRTDFT), sum(LRTLruns), sum(LRTRuns), sum(LRTTBT), sum(LRTGCD), sum(LRTBckstack)]
    print("Times Required to handle {} flows in Seconds is :".format(NumberOFFlows-1))
    print(sum(RandomnessTest)/1000)

    print(NumberOFFlows-1)
    return sum(LRTpreprocessing), sum(LRTFreq), sum(LRTBlockfreq), sum(LRTSer), sum(LRTCumsum), sum(LRTApEnt), sum(LRTDFT), sum(LRTLruns), sum(LRTRuns), sum(LRTTBT), sum(LRTGCD), sum(LRTBckstack), sum(LFGTmatch), sum(LANNpredictions)

def edit_config_for_iteration(configFile, n):
    f = open(configFile, 'r')  
    lines = f.readlines()
    if n== 6:
        newLine = re.sub('[00000]', '', lines[n-2])
        lines[n-2] = newLine
    if n== 9:
        newLine = re.sub('[00000]', '', lines[n-2])
        lines[n-2] = newLine
    else:
        newLine = re.sub('[.]', str(0) +'.', lines[n-1])
        print(lines[n-1])
        lines[n-1] = newLine
        print(lines[n-1])
    f.close()

    f = open(configFile, 'w')
    f.writelines(lines)
    f.close()

def extract_same_type_values(listOfValues):
    SameTypeValues = []
    # meanList = []
    for i in range(len(listOfValues[0])):
        data = [item[i] for item in listOfValues]
        SameTypeValues.append(data)
        # meanList.append(data)
    return SameTypeValues

ProcessingTimeList = extract_time_consumption(timeLogFile)
print("Here is the Processing time list\n\n", ProcessingTimeList)

def extract_dfDictionary_for_stackedbarchart(timeLFile, n):
    #Run program in for loop: change config file to 
    timeExperiments = []
    #results analysis of precision,recall,f1 and accuracy
    resultsExperiments = []
    for i in range(5):
        ProcessingTimeList = []
        ProcessingTimeList = extract_time_consumption(timeLFile)
        TOfloatValues = [float(ele/1000000) for ele in ProcessingTimeList]
        timeExperiments.append(TOfloatValues)

        edit_config_for_iteration(configFile, n)
        conf = ConfigParser()
        conf.read(configFile)
        if n==5:
            timeLFile = conf['File_parameters']['TimeLog']
        else:
            timeLFile = conf['File_parameters']['TimeLog2']
        print("Here should be the modified TIMELOGFILE\n\n", timeLFile)
        print("Iteration {} is DONE_______________________________________".format(i))
    if n==5:
        n=6
    if n==8:
        n=9
    edit_config_for_iteration(configFile, n)
    organizeTimeList = extract_same_type_values(timeExperiments)
    timeDict = {}

    timeDict["Preprocessing"] = organizeTimeList[0]
    timeDict["Freq"] = organizeTimeList[1]
    timeDict["BlockFreq"] = organizeTimeList[2]
    timeDict["Ser"] = organizeTimeList[3]
    timeDict["Cumsum"] = organizeTimeList[4]
    timeDict["AprEnt"] = organizeTimeList[5]
    timeDict["DFT"] = organizeTimeList[6]
    timeDict["LRuns"] = organizeTimeList[7]
    timeDict["Runs"] = organizeTimeList[8]
    timeDict["TBT"] = organizeTimeList[9]
    timeDict["GCD"] = organizeTimeList[10]
    timeDict["Bckstack"] = organizeTimeList[11]
    timeDict["GTmatch"] = organizeTimeList[12]
    timeDict["Classification"] = organizeTimeList[13]
    return timeDict

font = {'family': 'serif',
    'color':  'darkred',
    'weight': 'normal',
    'size': 12,
    }
timeDict = extract_dfDictionary_for_stackedbarchart(timeLogFile, 5)
#this is the second file time to combine the two stacked barcharts in one chart 
#and show the comparison between the 16 features and 22 features time consumption
timeDIct2 = {}
timeDict2 = extract_dfDictionary_for_stackedbarchart(timeLogFile2, 8)


def get_parameters_for_Stacked_bar(timeDictionary):
    newtimeDict = {}
    parameters = []
    meanList = []
    stdevList = []
    tmpDict = {}
    #New dictionary to save each parameter with mean of its values
    for key, value in timeDictionary.items():
        data = np.array(value)
        mean = statistics.mean(data)
        stdev = statistics.stdev(data)
        newtimeDict[key] = mean
        tmpDict[key] = [mean, stdev]
    #sort the parameters in desciding order in a list of tuples (key(first element) is the parameter and value is the mean)
    SortedDictionary = sorted(newtimeDict.items(), key=lambda x: x[1])
    for item in SortedDictionary:
        meanList.append(item[1])
        dictList = tmpDict[item[0]]
        stdevList.append(dictList[1])
        parameters.append(item[0])
    return parameters, meanList, stdevList



def try_combined_pands(stackedBar):
    print("PRINT THE TIMEDICT", timeDict)
    parameters, meanList, stdevList = get_parameters_for_Stacked_bar(timeDict)
    parameters2, meanList2, stdevList2 = get_parameters_for_Stacked_bar(timeDict2)
    print(timeDict2)
    colors = ['darkred', 'grey', 'purple', 'lime', 'yellow', 'pink', 'sienna', 'cyan', 'orange', 'c', 'r', 'g', 'b', 'k']
    labelIndex = []
    combineMeanList = [meanList, meanList2]
    combineStdList = []
    for i in range(len(stdevList2)):
        combineStdList.append([stdevList[i], stdevList2[i]])
    print(len(parameters), len(parameters2))
    for i in range(len(parameters2)):
        labelIndex.append("{}_16F, {}_22F".format(parameters[i], parameters2[i]))
    plotdata = pd.DataFrame(combineMeanList ,columns=labelIndex, index=["16Features", "22Features"])
    plotdata.plot(kind='bar', stacked=True, color=colors, rot=0, yerr= combineStdList, figsize=(12, 8))
    plt.xlabel("Number Of Features", fontdict=font)
    plt.ylabel("Time (Sec)", fontdict=font)
    plt.semilogy()
    plt.legend(bbox_to_anchor =(0.85, 1.17), ncol = 3, fontsize=9)
    plt.savefig(stackedBar)

try_combined_pands(stackedBar)


