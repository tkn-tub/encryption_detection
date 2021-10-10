
"""This script generates the labeled files from an lpi_protoident output file
Three types of labeling is provided for binary and protocol (5 and 6 protocols) for evaluation
the headers (LpiOutput,label,pred) need to be added manually to the output files
The unlabeled flows such flows without payload or UDP flows will be execluded"""

import csv



classes = 2
"""This function converts tap seperated file to a .csv file (comma seperated file)
"""
def convert_from_space_to_csv(labelFile, csvLabelFile):
  input_file = open(labelFile, 'r')
  output_file = open(csvLabelFile, 'w')
  for line in input_file:
      line = line.strip().split(' ')
      output_file.write(','.join(line) + '\n')
  input_file.close()
  output_file.close()

def get_defined_label(portNumber, lpiOutput):
    base = int(portNumber)/100
    base = int(base) * 100
    if classes == 6:
        Lpi_switch = {"FTP_Control": 0, "FTP_Data": 0, "Unknown_TCP": 7, "HTTP_NonStandard":2, "SSH": 3, "SSL/TLS": 4}
        prediction = Lpi_switch.get(lpiOutput, "default")
        switch = {2000: 0, 6000: 1, 8000:2, 3000: 3, 4000: 4, 7000: 5}
        value = switch.get(base, "default")
        return value, prediction
    if classes == 5:
        Lpi_switch = {"FTP_Control": 0, "FTP_Data": 0, "Unknown_TCP": 7, "HTTP_NonStandard":2, "SSH": 3, "SSL/TLS": 4}
        prediction = Lpi_switch.get(lpiOutput, "default")
        switch = {2000: 0, 6000: 1, 8000:2, 3000: 3, 4000: 4, 7000: 3}
        value = switch.get(base, "default")
        return value, prediction
    if classes == 2:
        if lpiOutput == "HTTP_NonStandard" or lpiOutput == "FTP_Control" or lpiOutput == "FTP_Data" or lpiOutput == "Unknown_TCP":
            prediction = "0"
        else:
            prediction = "1"
        if (base == 8000 or base == 2000 or base == 6000):
            return "0", prediction
        elif (base == 3000 or base == 7000 or base == 4000):
            return "1", prediction
        else:
            return "-1"


def extract_label_file(csvLabelFile, finalizedFile):
    finalFile = open(finalizedFile, 'w')    
    csv_finalFile = csv.writer(finalFile)
    fileGT = open(csvLabelFile)
    csv_GT = csv.reader(fileGT)
    for line in csv_GT:
        if line[0] == "No_Payload" or line[0] == "Unknown_UDP":
            continue
        label = get_defined_label(line[3], line[0])
        csv_finalFile.writerow(line[0:1] + [label[0]] + [label[1]])
    fileGT.close()
    
    finalFile.close()


labelFile = '../LPIOutput/a15M.log' #should be exist
csvLabelFile = '../LPIOutput/a15M.csv' #will be created
convert_from_space_to_csv(labelFile, csvLabelFile)

# The labeled files are comma seperated and the headers (LpiOutput,label,pred) need to be added manually.

finalizedFile = '../LPIOutput/a15Binary.csv' # Labeled file
extract_label_file(csvLabelFile, finalizedFile)

classes = 5
finalizedFile = '../LPIOutput/a15_p5.csv' # Labeled file
extract_label_file(csvLabelFile, finalizedFile)
classes = 6
finalizedFile = '../LPIOutput/a15_p6.csv' # Labeled file
extract_label_file(csvLabelFile, finalizedFile)