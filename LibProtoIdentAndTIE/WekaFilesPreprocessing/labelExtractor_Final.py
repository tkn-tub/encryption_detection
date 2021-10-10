
"""This script is doing preprocessing to get the ground truth represented in .arff files
features and labels to be used in Weka application
By matching the five tuple from the output of lpi_protoident tool to identify the flow
execluding the flows without payload and the unlabeled flows.
the headers lines from input file .arff must be removed manually before processing
Also, the new headers must be added manually to the finalized files to be used in Weka application

"""

import csv



classes = 14
"""This function converts tap seperated file to a .csv file (comma seperated file)
"""
def convert_from_space_to_csv(labelFile, csvLabelFile):
  input_file = open(labelFile, 'r')
  output_file = open(csvLabelFile, 'w')
   #input_file.readline() # skip first line
  for line in input_file:
      line = line.strip().split(' ')
      output_file.write(','.join(line) + '\n')
  input_file.close()
  output_file.close()

def get_defined_label(portNumber):
    base = int(portNumber)/100
    base = int(base) * 100
    if classes == 6:
        switch = {2000: 0, 6000: 1, 8000:2, 3000: 3, 4000: 4, 7000: 5}
        value = switch.get(base, "default")
        return value
    if classes == 5:
        switch = {2000: 0, 6000: 1, 8000:2, 3000: 3, 4000: 4, 7000: 3}
        value = switch.get(base, "default")
        return value
    if classes == 2:
        if (base == 8000 or base == 2000 or base == 6000):
            return "0"
        elif (base == 3000 or base == 7000 or base == 4000):
            return "1"
        else:
            return "-1"
    elif classes == 14:
        if base == 3000 or base == 4000 or base == 7000:
            return 13
        else:
            return int(portNumber) % base
"""This function creates final .arrf file which includes features and labels
from lpi_protoident and from .arff file by matching the five_tuple from both files
to execlude the unlabeled flows such flows without payload or UDP flows.
and finally taking the featrues and label from arffFile """
def extract_features_label_file(arffFile, csvLabelFile, finalizedFile):
    arff = open(arffFile)
    
    finalFile = open(finalizedFile, 'w')
    csv_arff = csv.reader(arff)
    
    csv_finalFile = csv.writer(finalFile)
    noPayloadCounter= 0
    for line in csv_arff:


        fileGT = open(csvLabelFile)
        csv_GT = csv.reader(fileGT)
        for lineGT in csv_GT:
            if lineGT[2] == line[1] and lineGT[1] == line[2] and lineGT[5] == line[3] and lineGT[4] == line[4] and lineGT[3] == line[5]:
                if lineGT[0] == "No_Payload":
                    noPayloadCounter+=1
                    break
                elif lineGT[5] == "17":
                    print("UDP FLow exlecluded")
                    break
                else:
                    label = get_defined_label(line[5])
                    csv_finalFile.writerow(line[15:-2] + [label])
                    fileGT.close()
                    break
    
    print("No payloads flows:", noPayloadCounter)
    arff.close()
    finalFile.close()

# This file is the output file of lpi_protoident tool
labelFile = '../LPIOutput/b15M.log' #should be exist
csvLabelFile = '../LPIOutput/b15M.csv' #will be created
convert_from_space_to_csv(labelFile, csvLabelFile)
# this file is the output of the TIE tool (considering only the first five packet)
arffFile = '../b15M/features_Noheaders.arff' #from arffoutput
# The following files are the processed .arff files with the labels according to the classification type
# The headers of these files must be added manually
finalizedFile = '../b15M/features_labeled_Content.arff' # ready for Weka - MLP
extract_features_label_file(arffFile, csvLabelFile, finalizedFile)



classes = 5
finalizedFile = '../b15M/features_labeled_p5.arff' # ready for Weka - MLP
extract_features_label_file(arffFile, csvLabelFile, finalizedFile)
classes = 6
finalizedFile = '../b15M/features_labeled_p6.arff' # ready for Weka - MLP
extract_features_label_file(arffFile, csvLabelFile, finalizedFile)
classes = 2
finalizedFile = '../b15M/features_labeled_Binary.arff' # ready for Weka - MLP
extract_features_label_file(arffFile, csvLabelFile, finalizedFile)


