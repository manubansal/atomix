'''
Atomix project, Pex2.py, (TODO: summary)
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
'''
from orapp import *
from __orblocks__ import *

#To write an application, derive from OrApp abstract base class 
#and override the declareApp method. That is where you will declare
#the full application.

## This example has a stateful Block. So, R2 will insert some new edges
# in the R2 graph

class Pex2(OrApp):

  #def declareAppWrongName(self):
  def declareApp(self):
    # declare states
    sta_start = OrState(self, "sta_start")
    sta_trinc = OrState(self, "sta_trinc")

    # declare actions
    axn1 = OrAxn()
    axn2 = OrAxn()

    # create atoms
    a = BlockA()
    b = BlockB()
    x = BlockX()
    y = BlockY()
    asb = BlockAS()                 # stateful block
    j = BlockJ(sta_start.ID())
    j1 = BlockJ(sta_trinc.ID())

    # define actions
    axn1.add(asb)
    axn1.add(y)
    axn1.add(j1)
    axn1.wire(asb.out.bOutData, y.inp.bInpData)
    axn1.wire(y.out.null, j1.inp.null)

    axn2.add(asb)
    axn2.add(b)
    axn2.add(j)
    axn2.wire(asb.out.bOutData, b.inp.bInpData)
    axn2.wire(b.out.null, j.inp.null)
    

    # define states
    sta_start.setAxnAndRule(axn1, j1)
    sta_trinc.setAxnAndRule(axn2, j)

    # designate initial state
    sta_start.setAsInitState()

    # add states to this application
    self.addState(sta_start)
    self.addState(sta_trinc)

    # deadline for the app to finish on a data unit
    self.setSlack(12000)
    self.setThr(4000)
