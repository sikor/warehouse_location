#!/usr/local/bin/python2.7
# -*- coding: utf-8 -*-

import subprocess
import StringIO
import os
problemName = 'warehouse'
path = '../workspace/'+problemName+'/Release/'+problemName
def solveIt(inputData):
    # Modify this code to run your optimization algorithm
    tmpFileName = 'tmp.data'
    tmpFile = open(tmpFileName, 'w')
    tmpFile.write(inputData)
    tmpFile.close()
    tmpFile = open(tmpFileName, 'r')
    actualOutput = subprocess.check_output([path], stdin=tmpFile).strip()
    tmpFile.close()
    os.remove(tmpFileName)
    return actualOutput
    


import sys

if __name__ == '__main__':
    if len(sys.argv) > 1:
        fileLocation = sys.argv[1].strip()
        inputDataFile = open(fileLocation, 'r')
        inputData = ''.join(inputDataFile.readlines())
        inputDataFile.close()
        print 'Solving:', fileLocation
        print solveIt(inputData)
    else:
        print 'This test requires an input file.  Please select one from the data directory. (i.e. python solver.py ./data/wl_16_1)'

