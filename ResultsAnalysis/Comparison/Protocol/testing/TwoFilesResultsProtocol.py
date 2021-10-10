from inspect import Parameter
import time
import os
import seaborn as sns
from sklearn.preprocessing import StandardScaler
from sklearn.svm import SVC
from sklearn.metrics import confusion_matrix
from sklearn.metrics import precision_score, recall_score, f1_score, accuracy_score
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import statistics
from configparser import ConfigParser
import re


configFile = '/home/ahmad/nnHandler/finalScripts/TwoFilesConf.ini'
conf = ConfigParser()
conf.read(configFile)
numOfFeatures = conf['File_parameters']['NumOfFeatures']
numberOfFeatures = int(numOfFeatures)
resultsFile_Mein = conf['File_parameters']['ResultsFile_Mein']
resultsFile_Lpi = conf['File_parameters']['ResultsFile_Lpi']
resultsFile_Weka = conf['File_parameters']['ResultsFile_Weka']
results_barFile = conf['File_parameters']['ResultsBarchartFile']

font = {'family': 'serif',
    'color':  'darkred',
    'weight': 'normal',
    'size': 13,
    }

def get_results_analysis(resultsFile):

    datasetfile = pd.read_csv(resultsFile,sep='\t', comment='#')
    #/home/ahmad/structureBuilder/FinalResults/ResultsT2f8.csv
    #/home/ahmad/structureBuilder/FinalResults/ResultsT1c5.csv
    labels = datasetfile['label']
    predictions = datasetfile['pred']
    nonEncryptedCounter = 0
    y_label = []
    y_pred = []



    unknownLabels = 0
    for i, j in zip(labels, predictions):
        if i != -1 and i!= 254:
            if i == 0:
                nonEncryptedCounter+=1
            y_label.append(i)
            y_pred.append(j)
        elif i == -1:
            unknownLabels+=1
    flows = len(y_label)
    print("Total Number Of FLows (Including flows with less than 5 packets):\n", flows)
    print("Number of Non-encrypted Flows:\n", nonEncryptedCounter)
    print("Number of Encrypted Flows:\n", flows - nonEncryptedCounter)
    precision = precision_score(y_label, y_pred)
    recall = recall_score(y_label, y_pred)
    f1 = f1_score(y_label, y_pred)
    accuracy = accuracy_score(y_label, y_pred)
    return precision, recall, f1, accuracy
def get_results_analysis_comma(resultsFile):

    datasetfile = pd.read_csv(resultsFile,sep=',', comment='#')
    #/home/ahmad/structureBuilder/FinalResults/ResultsT2f8.csv
    #/home/ahmad/structureBuilder/FinalResults/ResultsT1c5.csv
    labels = datasetfile['label']
    predictions = datasetfile['pred']
    nonEncryptedCounter = 0
    y_label = []
    y_pred = []



    unknownLabels = 0
    for i, j in zip(labels, predictions):
        if i != -1 and i!= 254:
            if i == 0:
                nonEncryptedCounter+=1
            y_label.append(i)
            y_pred.append(j)
        elif i == -1:
            unknownLabels+=1
    flows = len(y_label)
    print("Total Number Of FLows (Including flows with less than 5 packets):\n", flows)
    print("Number of Non-encrypted Flows:\n", nonEncryptedCounter)
    print("Number of Encrypted Flows:\n", flows - nonEncryptedCounter)
    precision = precision_score(y_label, y_pred)
    recall = recall_score(y_label, y_pred)
    f1 = f1_score(y_label, y_pred)
    accuracy = accuracy_score(y_label, y_pred)
    return precision, recall, f1, accuracy

def combined_barchart(rfile, resultsList, fiveresultsList):
    n_groups = 4
    performance = [float("{:,.2f}".format(ele*100)) for ele in resultsList]
    performanceF = [float("{:,.2f}".format(ele*100)) for ele in fiveresultsList]
    # means_frank = ()
    # means_guido = (85, 62, 54, 20)
    parameters = ["Precision", "Recall", "F1", "Accuracy"]
    # create plot
    fig, ax = plt.subplots()
    index = np.arange(n_groups)
    bar_width = 0.25
    opacity = 0.8

    rects1 = ax.bar(index - bar_width/2, performance, bar_width,
    alpha=opacity,
    color='b',
    label='MY_GT')

    rects2 = ax.bar(index + bar_width/2, performanceF, bar_width,
    alpha=opacity,
    color='g',
    label='Lpi_DPI')

    # plt.ylabel("Performance (%)", fontdict=font)
    # plt.xlabel("Parameters", fontdict=font)
    # plt.title('Results Analysis', fontdict=font)
    # plt.xticks(index + bar_width, (parameters))
    ax.set_ylabel("Performance (%)", fontdict=font)
    ax.set_xlabel("Parameters", fontdict=font)
    #ax.set_title('Results Analysis', fontdict=font)
    ax.set_xticks(index)
    ax.set_xticklabels(parameters)
    plt.legend(bbox_to_anchor =(0.8, 1.1), ncol = 3, fontsize=9)

    ax.bar_label(rects1, padding=3)
    ax.bar_label(rects2, padding=3)

    fig.tight_layout()
    # for i in range(len(performance)):
    #     plt.annotate(f'{performance[i]}\n', xy=(parameters[i], performance[i]), ha='center', va='center')
    # plt.legend()

    # plt.tight_layout()
    # plt.show()
    plt.savefig(rfile)

resultsList_Mein = get_results_analysis(resultsFile_Mein)

resultsList_Lpi = get_results_analysis_comma(resultsFile_Lpi)


combined_barchart(results_barFile, resultsList_Mein, resultsList_Lpi)