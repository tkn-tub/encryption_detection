
import time
import os
import seaborn as sns
from sklearn.metrics import precision_score, recall_score, f1_score, accuracy_score
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import statistics
from configparser import ConfigParser
import re


configFile = '/home/ahmad/structureBuilder/automation/config.ini'
conf = ConfigParser()
conf.read(configFile)
timeLogFile = conf['File_parameters']['TimeLog']
numOfFeatures = conf['File_parameters']['NumOfFeatures']
numberOfFeatures = int(numOfFeatures)
resultsFile = conf['File_parameters']['ResultsFile']

results_barFile = conf['File_parameters']['ResultsBarchartFile']
time_boxFile = conf['File_parameters']['TimeAnalysisFile']
results_barFile2 = conf['File_parameters']['ResultsBarchartFile2']

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
    if (numberOfFeatures > 16):
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
    if (numberOfFeatures > 16):
        LRTTBT = []
        LRTGCD = []
        LRTBckstack = []
    LFGTmatch = []
    LANNpredictions = []



    NumberOFFlows = 0
    if (numberOfFeatures > 16):
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
        #print("Times Required to handle {} flows in MicroSeconds is :".format(NumberOFFlows-1))
        #print("RTpre:\t{}\nFreq:\t{}\nBlockfreq\t{}\nSer\t{}\nCumsum\t{}\nApEnt\t{}\nDft\t{}\nLruns\t{}\nRuns\t{}\nTBT\t{}\nGCD\t{}\nBckstack\t{}\nGTmatch\t{}\nClassification\t{}".format(sum(LRTpreprocessing), sum(LRTFreq), sum(LRTBlockfreq), sum(LRTSer), sum(LRTCumsum), sum(LRTApEnt), sum(LRTDFT), sum(LRTLruns), sum(LRTRuns), sum(LRTTBT), sum(LRTGCD), sum(LRTBckstack), sum(LFGTmatch), sum(LANNpredictions)))
        RandomnessTest = [sum(LRTpreprocessing), sum(LRTFreq), sum(LRTBlockfreq), sum(LRTSer), sum(LRTCumsum), sum(LRTApEnt), sum(LRTDFT), sum(LRTLruns), sum(LRTRuns), sum(LRTTBT), sum(LRTGCD), sum(LRTBckstack)]
        #print("Times Required to handle {} flows in Seconds is :".format(NumberOFFlows-1))
        #print(sum(RandomnessTest)/1000)

        print(NumberOFFlows-1)
        return sum(LRTpreprocessing), sum(LRTFreq), sum(LRTBlockfreq), sum(LRTSer), sum(LRTCumsum), sum(LRTApEnt), sum(LRTDFT), sum(LRTLruns), sum(LRTRuns), sum(LRTTBT), sum(LRTGCD), sum(LRTBckstack), sum(LFGTmatch), sum(LANNpredictions)
    else:
        for i1, i2, i3, i4, i5, i6, i7, i8, i9, i13 , i14 in zip(RTpreprocessing, RTFreq, RTBlockfreq, RTSer, RTCumsum, RTApEnt, RTDFT, RTLruns, RTRuns, FGTmatch, ANNpredictions):
            if NumberOFFlows == 0:
                print("This is the time consumption of the first flow which includes the initialization of RTs and ANN")
                print("RTpre:\t{},Freq:\t{},Blockfreq\t{},Ser\t{},Cumsum\t{},ApEnt\t{},Dft\t{},Lruns\t{},Runs\t{},GTmatch\t{},Classification\t{}".format(i1, i2, i3, i4, i5, i6, i7, i8, i9, i13 , i14))
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
                LFGTmatch.append(i13)
                LANNpredictions.append(i14)
                NumberOFFlows+=1
        #print("Times Required to handle {} flows in MicroSeconds is :".format(NumberOFFlows-1))
        #print("RTpre:\t{}\nFreq:\t{}\nBlockfreq\t{}\nSer\t{}\nCumsum\t{}\nApEnt\t{}\nDft\t{}\nLruns\t{}\nRuns\t{}\nGTmatch\t{}\nClassification\t{}".format(sum(LRTpreprocessing), sum(LRTFreq), sum(LRTBlockfreq), sum(LRTSer), sum(LRTCumsum), sum(LRTApEnt), sum(LRTDFT), sum(LRTLruns), sum(LRTRuns), sum(LFGTmatch), sum(LANNpredictions)))
        RandomnessTest = [sum(LRTpreprocessing), sum(LRTFreq), sum(LRTBlockfreq), sum(LRTSer), sum(LRTCumsum), sum(LRTApEnt), sum(LRTDFT), sum(LRTLruns), sum(LRTRuns)]
        #print("Times Required to handle {} flows in Seconds is :".format(NumberOFFlows-1))
        #print(sum(RandomnessTest)/1000)

        print(NumberOFFlows-1)
        return sum(LRTpreprocessing), sum(LRTFreq), sum(LRTBlockfreq), sum(LRTSer), sum(LRTCumsum), sum(LRTApEnt), sum(LRTDFT), sum(LRTLruns), sum(LRTRuns), sum(LFGTmatch), sum(LANNpredictions)




def get_results_analysis(resultsFile):

    datasetfile = pd.read_csv(resultsFile,sep='\t', comment='#')
    labels = datasetfile['label']
    predictions = datasetfile['pred']
    y_label = []
    y_pred = []



    unknownLabels = 0
    for i, j in zip(labels, predictions):
        if i != -1 and i!= 254:
            y_label.append(i)
            y_pred.append(j)
        elif i == -1:
            unknownLabels+=1

    precision = precision_score(y_label, y_pred)
    recall = recall_score(y_label, y_pred)
    f1 = f1_score(y_label, y_pred)
    accuracy = accuracy_score(y_label, y_pred)
    return precision, recall, f1, accuracy


def extract_same_type_values(listOfValues):
    SameTypeValues = []
    for i in range(len(listOfValues[0])):
        data = [item[i] for item in listOfValues]
        SameTypeValues.append(data)
        # meanList.append(data)
    return SameTypeValues

def edit_config_for_iteration(configFile, n):
    f = open(configFile, 'r')  
    lines = f.readlines()

    newLine = re.sub('[.]', str(0) +'.', lines[n-1])
    print(lines[n-1])
    lines[n-1] = newLine #+ "\n" 
    print(lines[n-1])
    f.close()   # close the file and reopen in write mode to enable writing to file; you can also open in append mode and use "seek", but you will have some unwanted old data if the new data is shorter in length.
    # filee = 'TimeLog= /home/ahmad/structureBuilder/FinalResults/timetestc.log'

    # print(re.sub('[.]', str(1) +'.', timeLogFile))

    f = open(configFile, 'w')
    f.writelines(lines)
    # do the remaining operations on the file
    f.close()
    time.sleep(20)

def run_with_os():
    command = "./command.sh" #command to be executed
    res = os.system(command)

#list of lists, each sublist contains the results for an iteration
#sums of time consumption of Rts,GTmatch and classification
timeExperiments = []
#results analysis of precision,recall,f1 and accuracy
resultsExperiments = []
for i in range(5):
    run_with_os()
    time.sleep(5)
    resultsList = []
    resultsList = get_results_analysis(resultsFile)
    TOfloatValuesresults = [float(ele*100) for ele in resultsList]
    resultsExperiments.append(TOfloatValuesresults)
    ProcessingTimeList = []
    ProcessingTimeList = extract_time_consumption(timeLogFile)
    TOfloatValues = [float(ele/1000000) for ele in ProcessingTimeList]
    timeExperiments.append(TOfloatValues)
    configFileImpl = "../Configuration.ini"
    n=44
    edit_config_for_iteration(configFileImpl, n)
    n = 29
    edit_config_for_iteration(configFile, n)
    configFile = 'config.ini'
    conf = ConfigParser()
    conf.read(configFile)
    timeLogFile = conf['File_parameters']['TimeLog']
    print("Here should be the modified TIMELOGFILE\n\n", timeLogFile)
    print("Iteration {} is DONE_______________________________________".format(i))

print("\n\nResults Experiments \n\n",resultsExperiments)
print("\n\nTime Experiments\n\n",timeExperiments)

timeDict = {}

organizeTimeList = extract_same_type_values(timeExperiments)
organizeResultsList = extract_same_type_values(resultsExperiments)

timeDict["Preprocessing"] = organizeTimeList[0]
timeDict["Freq"] = organizeTimeList[1]
timeDict["BlockFreq"] = organizeTimeList[2]
timeDict["Ser"] = organizeTimeList[3]
timeDict["Cumsum"] = organizeTimeList[4]
timeDict["AprEnt"] = organizeTimeList[5]
timeDict["DFT"] = organizeTimeList[6]
timeDict["LRuns"] = organizeTimeList[7]
timeDict["Runs"] = organizeTimeList[8]
if numberOfFeatures > 16:
    timeDict["TBT"] = organizeTimeList[9]
    timeDict["GCD"] = organizeTimeList[10]
    timeDict["Bckstack"] = organizeTimeList[11]
    timeDict["GTmatch"] = organizeTimeList[12]
    timeDict["Classification"] = organizeTimeList[13]
else:
    timeDict["GTmatch"] = organizeTimeList[9]
    timeDict["Classification"] = organizeTimeList[10]
font = {'family': 'serif',
    'color':  'darkred',
    'weight': 'normal',
    'size': 18,
    }
def time_consumption_boxchart(boxchartFile):
    newtimeDict = {}
    parameters = []
    for key, value in timeDict.items():
        data = np.array(value)
        MEAN = statistics.mean(data)
        print("DATA_____________________________", MEAN)
        newtimeDict[key] = MEAN
        # parameters.append(key)
    SortedDictionary = sorted(newtimeDict.items(), key=lambda x: x[1])
    dfTime = pd.DataFrame.from_dict(timeDict)

    for key in SortedDictionary:
        parameters.append(key[0])
    print("PARAMETERS_____________________________", parameters)
    dfTime_plot = dfTime[parameters]

    print(dfTime_plot.head())

    plt.figure(figsize = (15, 10))
    plt.title('Time Consumption Analysis on Multiple Run Of Implemetation', fontdict=font)
    plt.xlabel("Processes", fontdict=font)
    plt.ylabel("Time (Sec)", fontdict=font)
    fig = dfTime_plot.boxplot()
    fig.get_figure().savefig(time_boxFile)

def results_paramerters(barchartFile):
    objects = ("Precision", "Recall", "F1", "Accuracy")
    y_pos = np.arange(len(objects))
    resultsList = get_results_analysis(resultsFile)
    performance = [float(ele*100) for ele in resultsList]
    plt.bar(y_pos, performance, align='center', alpha=0.5)
    plt.xticks(y_pos, objects)
    plt.ylabel("Performance (%)", fontdict=font)
    plt.xlabel("Parameters", fontdict=font)
    plt.title('Results Analysis')

    plt.savefig(results_barFile)


results_paramerters(results_barFile)

time_consumption_boxchart(time_boxFile)

def bar_chart(file):
    resultsList = get_results_analysis(resultsFile)
    performance = [float(ele*100) for ele in resultsList]
    data = {"Parameters": ["Precision", "Recall", "F1", "Accuracy"],
        "Results": performance}
    # Now convert this dictionary type data into a pandas dataframe
    # specifying what are the column names
    df = pd.DataFrame(data, columns=['Parameters', 'Results'])
    
    
    # Defining the plot size
    plt.figure(figsize=(10, 8))
    
    # Defining the values for x-axis, y-axis
    # and from which datafarme the values are to be picked
    plots = sns.barplot(x="Parameters", y="Results", data=df)
  
    # Iterrating over the bars one-by-one
    for bar in plots.patches:
        
        plots.annotate(format(bar.get_height(), '.2f'), 
                        (bar.get_x() + bar.get_width() / 2, 
                            bar.get_height()), ha='center', va='center',
                        size=15, xytext=(0, 8),
                        textcoords='offset points')
    plt.ylabel("Performance (%)", fontdict=font)
    plt.xlabel("Parameters", fontdict=font)
    plt.title('Results Analysis', fontdict=font)

    plt.savefig(file)


bar_chart(results_barFile2)
