'''
Atomix project, Pex1.py, (TODO: summary)
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
'''
from orapp import *
from __orblocks__ import *

#To write an application, derive from OrApp abstract base class 
#and override the declareApp method. That is where you will declare
#the full application.
class Pex1(OrApp):

  #def declareAppWrongName(self):
  def declareApp(self):
    # declare states
    sta_start = OrState(self, "sta_start")
    sta_trinc = OrState(self, "sta_trinc")
    sta_trdec = OrState(self, "sta_trdec")

    # declare actions
    axn1 = OrAxn()
    axn2 = OrAxn()
    axn3 = OrAxn()

    # create atoms
    a = BlockA()
    b = BlockB()
    x = BlockX()
    y = BlockY()
    dx = BlockDX(sta_start.ID(), sta_trinc.ID(), sta_trdec.ID())
    j = BlockJ(sta_start.ID())
    

    ## Debug info ##
    print("BlockAS ID %d\n"%a.IDval)
    print("BlockB ID %d\n"%b.IDval)
    print("BlockX ID %d\n"%x.IDval)
    print("BlockY ID %d\n"%y.IDval)
    print("BlockDX ID %d\n"%dx.IDval)
    print("BlockJ ID %d\n"%j.IDval)
    #print("BlockAS again %d\n"%as1.IDval)

    # define actions
    axn1.add(x)
    axn1.add(dx)
    axn1.wire(x.out.bOutData, dx.inp.bInpData)
    #axn1.edge(x,dx) 

    axn2.add(x)
    axn2.add(a)
    axn2.add(y)
    axn2.add(j)
    axn2.wire(x.out.bOutData, a.inp.bInpData)
    axn2.wire(a.out.bOutData, y.inp.bInpData)
    axn2.wire(y.out.null, j.inp.null)           #this forces a data dependency between y and j


    axn3.add(x)
    axn3.add(b)
    axn3.add(y)
    axn3.add(j)
    axn3.wire(x.out.bOutData, b.inp.bInpData)
    axn3.wire(b.out.bOutData, y.inp.bInpData)
    axn3.wire(y.out.null, j.inp.null)           #this forces a data dependency between y and j

    # define states
    sta_start.setAxnAndRule(axn1, dx)
    sta_trinc.setAxnAndRule(axn2, j)
    sta_trdec.setAxnAndRule(axn3, j)

    # inter-state edge 
    
    #self.wire(a.out.bOutData, b.inp.bInpData)

    # designate initial state
    sta_start.setAsInitState()

    # add states to this application
    self.addState(sta_start)
    self.addState(sta_trinc)
    self.addState(sta_trdec)

    # deadline for the app to finish on a data unit
    self.setSlack(12000)      #You have upto 12000 time units after receiving the last segment to respond
    # TBD: Somewhere we should encode how many segments can we receive before we have to respond
    self.setThr(4000)         #You need one output every 4000 time units
