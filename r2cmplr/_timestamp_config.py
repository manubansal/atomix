'''
Atomix project, _timestamp_config.py, (TODO: summary)
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
'''

def populate_timestamp_config(timestamp_conf):
    if timestamp_conf == 'st':
      TIMESTAMP_CONFIG = config_st()
    elif timestamp_conf == 'at':
      TIMESTAMP_CONFIG = config_at()
    elif timestamp_conf == 'atii':
      TIMESTAMP_CONFIG = config_atii()
    elif timestamp_conf == 'none':
      TIMESTAMP_CONFIG = config_none()

    return TIMESTAMP_CONFIG

def config_none():
    TIMESTAMP_CONFIG = {}
    #==============================
    TIMESTAMP_CONFIG['TIMESTAMP_STATES'] = False
    TIMESTAMP_CONFIG['TIMESTAMP_ACTIONS'] = False
    TIMESTAMP_CONFIG['TIMESTAMP_ATOMS'] = False
    TIMESTAMP_CONFIG['TIMESTAMP_ATOMIMPLS'] = False
    TIMESTAMP_CONFIG['TIMESTAMP_ATOMIMPLS_ii_only'] = False
    #=================================
    return TIMESTAMP_CONFIG

def config_ati():
    TIMESTAMP_CONFIG = {}
    #==============================
    #config used for test6-ati
    TIMESTAMP_CONFIG['TIMESTAMP_STATES'] = False
    TIMESTAMP_CONFIG['TIMESTAMP_STATES_TO_TIMESTAMP'] = range(0,20)
    TIMESTAMP_CONFIG['TIMESTAMP_ACTIONS'] = False
    TIMESTAMP_CONFIG['TIMESTAMP_IDENTIFY_ATOMS_PER_AXN'] = False	#just identify blocks
    TIMESTAMP_CONFIG['TIMESTAMP_ATOMS'] = False
    TIMESTAMP_CONFIG['TIMESTAMP_ATOMS_AXN_SCOPES_TO_TIMESTAMP'] = [\
    'axnRxDataDecode54m_M_0', \
    'axnRxDataDecode54m_M_1', \
    'axnRxDataDecode54m_M_2', \
    ]
    TIMESTAMP_CONFIG['TIMESTAMP_ATOMIMPLS'] = True
    TIMESTAMP_CONFIG['TIMESTAMP_ATOMIMPLS_ii_only'] = False
    TIMESTAMP_CONFIG['TIMESTAMP_ATOMIMPLS_TO_TIMESTAMP'] = []	#to internally timestamp all atoms
    #=================================
    return TIMESTAMP_CONFIG

def config_at():
    TIMESTAMP_CONFIG = {}
    #=================================
    #probably the original config used for test6-at
    TIMESTAMP_CONFIG['TIMESTAMP_STATES'] = False
    TIMESTAMP_CONFIG['TIMESTAMP_STATES_TO_TIMESTAMP'] = range(0,20)
    TIMESTAMP_CONFIG['TIMESTAMP_ACTIONS'] = False
    TIMESTAMP_CONFIG['TIMESTAMP_IDENTIFY_ATOMS_PER_AXN'] = False
    TIMESTAMP_CONFIG['TIMESTAMP_ATOMS'] = True
    TIMESTAMP_CONFIG['TIMESTAMP_ATOMS_AXN_SCOPES_TO_TIMESTAMP'] = []
    TIMESTAMP_CONFIG['TIMESTAMP_ATOMIMPLS'] = False
    TIMESTAMP_CONFIG['TIMESTAMP_ATOMIMPLS_ii_only'] = False
    TIMESTAMP_CONFIG['TIMESTAMP_ATOMIMPLS_TO_TIMESTAMP'] = []	#to internally timestamp all atoms
    #=================================
    return TIMESTAMP_CONFIG


def config_unknown():
    TIMESTAMP_CONFIG = {}
    #=================================
    #unknown config
    TIMESTAMP_CONFIG['TIMESTAMP_STATES'] = False
    TIMESTAMP_CONFIG['TIMESTAMP_STATES_TO_TIMESTAMP'] = range(0,20)
    TIMESTAMP_CONFIG['TIMESTAMP_ACTIONS'] = False
    TIMESTAMP_CONFIG['TIMESTAMP_IDENTIFY_ATOMS_PER_AXN'] = True		#to assign different atom ids (and therefore block classes) of the same block type if atoms are in different axns this will lead to code size blowup
    TIMESTAMP_CONFIG['TIMESTAMP_ATOMS'] = True
    TIMESTAMP_CONFIG['TIMESTAMP_ATOMS_AXN_SCOPES_TO_TIMESTAMP'] = [\
    'axnRxDataDecode54m_M_0', \
    'axnRxDataDecode54m_M_1', \
    'axnRxDataDecode54m_M_2', \
    'axnRxDataDecode54m_M_3', \
    ]
    TIMESTAMP_CONFIG['TIMESTAMP_ATOMIMPLS'] = False
    TIMESTAMP_CONFIG['TIMESTAMP_ATOMIMPLS_ii_only'] = False
    TIMESTAMP_CONFIG['TIMESTAMP_ATOMIMPLS_TO_TIMESTAMP'] = []	#to internally timestamp all atoms
    #=================================
    return TIMESTAMP_CONFIG


def config_st():
    TIMESTAMP_CONFIG = {}
    #=================================
    #config probably for test6-st
    TIMESTAMP_CONFIG['TIMESTAMP_STATES'] = True
    #TIMESTAMP_CONFIG['TIMESTAMP_STATES'] = False
    TIMESTAMP_CONFIG['TIMESTAMP_STATES_TO_TIMESTAMP'] = range(0,20)
    #TIMESTAMP_CONFIG['TIMESTAMP_STATES_TO_TIMESTAMP'] = range(9,14)
    #-----------
    #TIMESTAMP_CONFIG['TIMESTAMP_ACTIONS']= True
    TIMESTAMP_CONFIG['TIMESTAMP_ACTIONS'] = False
    #-----------
    TIMESTAMP_CONFIG['TIMESTAMP_IDENTIFY_ATOMS_PER_AXN'] = False	#just identify blocks
    #TIMESTAMP_CONFIG['TIMESTAMP_IDENTIFY_ATOMS_PER_AXN'] = True		#to assign different atom ids (and therefore block classes) of the same block type if atoms are in different axns this will lead to code size blowup
    #-----------
    #TIMESTAMP_CONFIG['TIMESTAMP_ATOMS'] = True
    TIMESTAMP_CONFIG['TIMESTAMP_ATOMS'] = False
    #to profile atoms in all axns
    #TIMESTAMP_CONFIG['TIMESTAMP_ATOMS_AXN_SCOPES_TO_TIMESTAMP'] = []	
    #to profile atoms in selected axns
    TIMESTAMP_CONFIG['TIMESTAMP_ATOMS_AXN_SCOPES_TO_TIMESTAMP'] = [\
    'axnRxDataDecode54m_M_0', \
    'axnRxDataDecode54m_M_1', \
    'axnRxDataDecode54m_M_2', \
    'axnRxDataDecode54m_M_3', \
    ]
    #'axnRxDataDecode54m_M_3', \
    #'axnRxDataDecode54m_T_0', \
    #'axnRxDataDecode54m_T_1', \
    #'axnRxDataDecode54m_T_2', \
    #'axnRxDataDecode54m_T_3', \
    #-------------
    #TIMESTAMP_CONFIG['TIMESTAMP_ATOMIMPLS'] = True
    TIMESTAMP_CONFIG['TIMESTAMP_ATOMIMPLS'] = False
    #TIMESTAMP_CONFIG['TIMESTAMP_ATOMIMPLS_ii_only'] = True
    TIMESTAMP_CONFIG['TIMESTAMP_ATOMIMPLS_ii_only'] = False
    TIMESTAMP_CONFIG['TIMESTAMP_ATOMIMPLS_TO_TIMESTAMP'] = []	#to internally timestamp all atoms
    #TIMESTAMP_CONFIG['TIMESTAMP_ATOMIMPLS_TO_TIMESTAMP'] = [\
    #103007, \
    #]
    #=================================
    return TIMESTAMP_CONFIG


def config_atii():
    TIMESTAMP_CONFIG = {}
    #=================================
    #config  for set1 results, atii
    TIMESTAMP_CONFIG['TIMESTAMP_STATES'] = False
    #TIMESTAMP_CONFIG['TIMESTAMP_STATES_TO_TIMESTAMP'] = range(0,20)
    TIMESTAMP_CONFIG['TIMESTAMP_STATES_TO_TIMESTAMP'] = range(0,100)
    TIMESTAMP_CONFIG['TIMESTAMP_ACTIONS'] = False
    TIMESTAMP_CONFIG['TIMESTAMP_IDENTIFY_ATOMS_PER_AXN'] = False	#just identify blocks
    #TIMESTAMP_CONFIG['TIMESTAMP_IDENTIFY_ATOMS_PER_AXN'] = True		#to assign different atom ids (and therefore block classes) of the same block type if atoms are in different axns this will lead to code size blowup
    #-----------
    TIMESTAMP_CONFIG['TIMESTAMP_ATOMS'] = False
    #to profile atoms in all axns
    TIMESTAMP_CONFIG['TIMESTAMP_ATOMS_AXN_SCOPES_TO_TIMESTAMP'] = []	
    #to profile atoms in selected axns
    #TIMESTAMP_CONFIG['TIMESTAMP_ATOMS_AXN_SCOPES_TO_TIMESTAMP'] = [\
    #'axnRxDataDecode54m_M_0', \
    #'axnRxDataDecode54m_M_1', \
    #'axnRxDataDecode54m_M_2', \
    #'axnRxDataDecode54m_M_3', \
    #]
    #'axnRxDataDecode54m_M_3', \
    #'axnRxDataDecode54m_T_0', \
    #'axnRxDataDecode54m_T_1', \
    #'axnRxDataDecode54m_T_2', \
    #'axnRxDataDecode54m_T_3', \
    #-------------
    TIMESTAMP_CONFIG['TIMESTAMP_ATOMIMPLS'] = True
    TIMESTAMP_CONFIG['TIMESTAMP_ATOMIMPLS_ii_only'] = True
    TIMESTAMP_CONFIG['TIMESTAMP_ATOMIMPLS_TO_TIMESTAMP'] = []	#to internally timestamp all atoms
    #TIMESTAMP_CONFIG['TIMESTAMP_ATOMIMPLS_TO_TIMESTAMP'] = [\
    #103007, \
    #]
    #=================================

    return TIMESTAMP_CONFIG
