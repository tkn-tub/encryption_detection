
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import statistics
from configparser import ConfigParser
import re


configFile = 'config.ini'
conf = ConfigParser()
conf.read(configFile)
timeLogFile2 = conf['File_parameters']['TimeLog2']
time_boxFile = conf['File_parameters']['BoxChart']

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


def extract_dfDictionary_for_stackedbarchart(timeLFile, n):
    #Run program in for loop: change config file to conclude the 5 experiments
    timeExperiments = []
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
    'size': 17,
    }
font2 = {'family': 'serif',
    'color':  'darkred',
    'weight': 'normal',
    'size': 15,
    } 

timeDIct = {}
timeDIct = extract_dfDictionary_for_stackedbarchart(timeLogFile2, 8)
def time_consumption_boxchart(boxchartFile):
    newtimeDict = {}
    parameters = []
    for key, value in timeDIct.items():
        data = np.array(value)
        MEAN = statistics.mean(data)
        print("DATA_____________________________", MEAN)
        newtimeDict[key] = MEAN
        # parameters.append(key)
    SortedDictionary = sorted(newtimeDict.items(), key=lambda x: x[1])
    dfTime = pd.DataFrame.from_dict(timeDIct)
    for key in SortedDictionary:
        parameters.append(key[0])
    print("PARAMETERS_____________________________", parameters)
    dfTime_plot = dfTime[parameters]
    fig, ax = plt.subplots()
    # dfTime_plot.sort()
    print(dfTime_plot.head())

    plt.figure(figsize = (19, 14))
    #plt.title('Time Consumption - 5 Run Of Implemetation', fontdict=font)
    plt.xlabel("Processes", fontdict=font)
    plt.ylabel("Time (Sec)", fontdict=font)
    plt.semilogy()
    fig = dfTime_plot.boxplot()
    fig.set_xlabel("Processes", fontdict=font)
    fig.set_xticklabels(parameters, fontdict=font2, rotation= 45)
    fig.get_figure().savefig(boxchartFile)

time_consumption_boxchart(time_boxFile)





