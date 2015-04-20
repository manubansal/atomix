#!/usr/bin/python
'''
Atomix project, hw_info.py, (TODO: summary)
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
'''
from _scrape_b_i import *
import numpy as np

class OrHW:
    hw_name = ""
    num_instances = 0
    IDval = 0

    def __init__(self, ID_val, name):
        self.IDval = ID_val
        self.hw_name = name

    def get_running_time(self, block_name):
        #print("Getting running time hw_name %s block_name %s"%(self.hw_name,block_name))
        if(self.hw_name == "CPU"):
            return 75
        return 200; #Dummy variable. Here we should link to 
                    # appropriate routines in C language

    def benchmark_hw (self, block_name):
        running_time = self.get_running_time(block_name)
        return running_time

class OrHWMgr:
    def __init__(self, hwinfo_file):
        self.hwList = []; #array
        self.benchmark_file = "hw_benchmark"
        self.q = []
        # This function reads the ORHWlist file and
        # generates a list of hw instances reading from a file
        #fname = os.getcwd()+'/'+hwinfo_file;
        fname = hwinfo_file;
        f = open(fname, 'r')
        lines = f.readlines()
        f.close()
        ctr = 0
        first = True
        INFINITY = 9999999
        for line in lines:
            print("%s \n" %line);
            split_str = line.split(' ')
            if(split_str[0] == "HW"): 
                num_instances = int(split_str[2]);
                for i in range(0, num_instances):
                    hw = OrHW(ctr, split_str[1]) #ID, name
                    ctr = ctr + 1 
                    self.hwList.append(hw)
            elif(split_str[0] == "COMM"):
                # Just the first time, allocate the q array
                if(first):
                    n = len(self.hwList);
                    self.q = INFINITY*np.ones((n,n), dtype=np.int)
                    first = False
                hw1 = split_str[1];
                hw2 = split_str[2];
                comm_cost = split_str[3];

                # Go through the list of HW and populate the matrix 
                for hwctr in self.hwList:
                    if(hwctr.hw_name == hw1):
                        for hwctr2 in self.hwList:
                            if(hwctr2.hw_name == hw2):
                                id1 = hwctr.IDval
                                id2 = hwctr2.IDval
                                #if(id1 != id2):
                                self.q[id1][id2] = comm_cost
                                self.q[id2][id1] = comm_cost
                    # Set self comm costs to own_cost anyway
                    #self.q[hwctr.IDval][hwctr.IDval] = own_cost
            else:
                #Anything else, just ignore
                continue
        print("Q matrix is:\n");
        print self.q                
        
    def benchmark_block(self, block):
        f = open(self.benchmark_file, 'a')
        for hw in self.hwList:
            running_time = hw.benchmark_hw(block)
            f.write("%d %d %f\n" %(block.IDval,hw.IDval,running_time))
        f.close()

    def init_benchmark_file(self):
        f = open(self.benchmark_file, "w")
        f.close()
    
    def getNumHw(self):
        return len(self.hwList)

    def getBenchMarkFile(self):
        return self.benchmark_file
    
    def getQ(self):
        return self.q

    def getNumCores(self):
       numCores = 0
       for hw in self.hwList:
         if hw.hw_name == "CPU":
            numCores = numCores + 1
       return numCores      
