'''
Atomix project, bufferTypes.py, (TODO: summary)
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
'''
#!/bin/python

############################################################################################
#List all your buffer types for fifos here. A buffer type is defined
#by the name and its definition as a C code fragment that either names
#an existing type or defines a new type.
############################################################################################
def createAllTypeDefinitions():

  addBufferType("BUF_Cplx32", "Cplx32")

  addBufferType("BUF_Funny", 
  """struct {
      Uint32 i;
      Uint32 j;
      Uint32 k;
      Uint32 l;
}"""
  )


############################################################################################
## DO NOT EDIT BEYOND THIS POINT
############################################################################################

bufTypes = ""
bufConsDecls = ""

bufTypesFile = "inc/fifoBufferTypes.h"
f1CanonicalName = "__FIFO_BUFFER_TYPES_H__"


bufConsDefsFile = "src/fifoTypedConstructors.c"

f1 = 0
f2 = 0


def addBufferType(t1, t2): 
  global bufTypes
  global bufConsDecls
  bufTypeDef = "typedef " + t2 + " " + t1 + ";\n"
  bufTypes += "\n" + bufTypeDef 

  bufConsDecl = """
FIFO_Status FIFO_createFifo_""" + t1 + """(FIFO_Handle handle);

"""
  bufConsDecls += bufConsDecl

  addTypedConstructor(t1);


def writeTypeDefinitionsAndConsDecls():
  global bufTypes
  global bufConsDecls
  global f1
  #print "Output file: " + bufTypesFile
  f1.write(bufTypes)
  f1.write(bufConsDecls)

def addTypedConstructor(t1):
  fifoConst = """
FIFO_Status FIFO_createFifo_""" + t1 + """(FIFO_Handle handle) {
  if (handle == 0)
    return FIFO_FAILURE;
  else
    handle->bufferSizeInBytes = sizeof(""" + t1 + """);

  return FIFO_createFifo(handle);
}

"""
  f2.write(fifoConst)

def main():
  global f1
  global f2
  global f1CanonicalName

  commonHeader = """
//AUTOMATICALLY GENERATED FILE. DO NOT EDIT. EDITS WILL BE LOST.

#include <osl/inc/swpform.h>
#include <oros/common/src/sfifos/fifoFactory.h>
"""

  f1header = """
#ifndef """ + f1CanonicalName + """
#define """ + f1CanonicalName + """

""" + commonHeader + """
"""

  f1tailer = """
#endif //""" + f1CanonicalName + """
"""

  f2header = commonHeader + """
#include <oros/common/src/sfifos/python/inc/fifoBufferTypes.h>
"""
  f2tailer = """
"""

  f1 = open(bufTypesFile,'w')
  f1.write(f1header)

  f2 = open(bufConsDefsFile, 'w')
  f2.write(f2header)

  createAllTypeDefinitions()
  #print bufTypes
  #print "Writing out buffer type definitions"
  writeTypeDefinitionsAndConsDecls()

  #print "Writing out typed fifo constructors"

  f1.write(f1tailer)
  f1.close()

  f2.write(f2tailer)
  f2.close()


if __name__ == "__main__":
  main()
