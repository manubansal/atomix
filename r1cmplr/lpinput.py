#!/usr/bin/env python
'''
Atomix project, lpinput.py, (TODO: summary)
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
'''

import numpy as np
import sys

def main():
  inpfile = sys.argv[1]
  (inp, out) = lpinput(inpfile)
  print inp
  print out


def lpinput(inpfile):
  #[inp, opt] = example();
  #[inp, opt] = example1();
  #[inp, opt] = example2();
  #[inp, opt] = wifi_6mbps_steady_state();
  #[inp, opt] = wifi_54mbps_steady_state();
  (inp, opt) = new_flowgraphs(inpfile);
  return (inp, opt)

def new_flowgraphs(inpfile):
  opt = {}
  inp = {}

  #problem = 'wifi_54mbps_split';
  #opt["lpoutfile"] = problem + '.lp';
  opt["minimize_makespan"]	= 1;

  opt["constraints_no_overbook"] = 1;
  opt["constraints_communication_allowance"] = 1;
  opt["extra_information_zero_y_i_j"] = 0;
  opt["relaxation_x_not_integer"] = 1;

  opt["constant_scale_down_factor"] = 1;

#  d = 12000;		#deadline
  D = 100000000;	#"infinity" equivalent in expressing the ILP

  #run(['wifi_schedule_data/' problem '.m']);
  modname = inpfile.split('.py')[0]
  print modname
  mod = __import__(modname)
  (p,G,q, dl,T) = mod.model(D)
  print("DEADLINE = %d"%dl)
  d = dl ## State Deadline

  #n = size(p, 1);
  #m = size(p, 2);
  n = p.shape[0];	#number of jobs to schedule
  m = p.shape[1];	#number of processors

  print n, m
  #raw_input()

  # Communication costs between different processors (for some fixed transfer size): {qij}, 1 i m, 1 j m

  #cpu2viterbi = 100;
  #cpu2cpu = 1000;
  
  # FIFO transfer time on same processor
  #singular_fifo_transfer_time = 200;

  #q = D*np.ones((m,m), dtype=np.int);

  #for ii in range(4):
  #  for jj in range(4):
  #    if ii == jj:
  #      q[ii][jj] = singular_fifo_transfer_time;
  #    else:
  #      q[ii][jj] = cpu2cpu;

  #for ii in range(4):
    #for jj in range(3,9):
  #  for jj in range(4,8):
  #    q[ii][jj] = cpu2viterbi;
  #    q[jj][ii] = cpu2viterbi;

  print
  print "q matrix:"
  print q

  inp["n"] = n;
  inp["d"] = d;
  inp["D"] = D;
  inp["G"] = G;
  inp["m"] = m;
  inp["p"] = p;
  inp["q"] = q;
  inp["T"] = T  

  return (inp, opt)



#function [inp, opt] = example1()
#  opt["lpoutfile"]					= 'example1.lp'
#  %opt["minimize_makespan"]				= 0;
#  opt["minimize_makespan"]			= 1;
#
#  opt["constraints_no_overbook"] 			= 1;
#  opt["constraints_communication_allowance"] 	= 1;
#  opt["extra_information_zero_y_i_j"]		= 0;
#  opt["relaxation_x_not_integer"]			= 1;
#
#  %opt["constant_scale_down_factor"] 		= 100;
#  opt["constant_scale_down_factor"] 		= 1;
#
#  %  Given input:
#
#%  %  Compute graph G with n compute nodes called jobs {J1, J2, ..., Jn}. Each job needs to execute exactly once without preemption. Jobs have processor affinities characterized ahead.
#  %n = 13; d = 10000; G = [ 1,2; 2,3; 3,4; 4,5; 5,6; 6,7; 7,8; 8,9; 9,10; 10,11; 11,12; 12,13];
#  n = 4; d = 20000; G = [ 1,2; 2,3; 3,4];
#  %D = d + 100;
#  D = 2 * d;
#
#  %  Hardware definition consisting of m processors {M1, M2, ..., Mm}. (M is used to denote a machine in the notion of the well-known job-shop problem which is analogous to processor in our problem). Processors may be identical or different.
#  m = 9				%3 dsps, 4 vcps, 2 fcps
#
#  %  Compute times of jobs on processors: {pij}, 1 i n, 1 j m. If a job i cannot be executed on a certain processor j, pij = inf, where inf is a large enough value to be specified later. This lets us model job-processor affinity.
#  p = [...
#	  200,	200,	200,	D,	D,	D,	D,	D,	D;...
#	  400,	400,	400,	D,	D,	D,	D,	D,	D;...	
#	  200,	200,	200,	D,	D,	D,	D,	D,	D;...
#	  600,	600,	600,	D,	D,	D,	D,	D,	D;...	
#	  ]
#
#  %  Communication costs between different processors (for some fixed transfer size): {qij}, 1 i m, 1 j m
#  q = [...
#	  0,	200,	200,	100,	100,	100,	100,	100,	100;...
#	  200,	0,	200,	100,	100,	100,	100,	100,	100;...
#	  200,	200	0,	100,	100,	100,	100,	100,	100;...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...
#	  ]
#
#
#  inp["n"] = n;
#  inp["d"] = d;
#  inp["D"] = D;
#  inp["G"] = G;
#  inp["m"] = m;
#  inp["p"] = p;
#  inp["q"] = q;
#end
#
#
#function [inp, opt] = example2()
#  opt["lpoutfile"]					= 'example2.lp'
#  %opt["minimize_makespan"]				= 0;
#  opt["minimize_makespan"]			= 1;
#
#  opt["constraints_no_overbook"] 			= 1;
#  opt["constraints_communication_allowance"] 	= 1;
#  opt["extra_information_zero_y_i_j"]		= 0;
#  opt["relaxation_x_not_integer"]			= 1;
#
#  %opt["constant_scale_down_factor"] 		= 100;
#  opt["constant_scale_down_factor"] 		= 1;
#
#  %  Given input:
#
#%  %  Compute graph G with n compute nodes called jobs {J1, J2, ..., Jn}. Each job needs to execute exactly once without preemption. Jobs have processor affinities characterized ahead.
#  %n = 13; d = 10000; G = [ 1,2; 2,3; 3,4; 4,5; 5,6; 6,7; 7,8; 8,9; 9,10; 10,11; 11,12; 12,13];
#  n = 8; d = 10000; G = [ 1,2; 2,3; 3,4; 4,5; 5,6; 6,7; 7,8];
#  %n = 7; d = 10000; G = [ 1,2; 2,3; 3,4; 4,5; 5,6; 6,7];
#  %D = d + 100;
#  D = 2 * d;
#
#  %  Hardware definition consisting of m processors {M1, M2, ..., Mm}. (M is used to denote a machine in the notion of the well-known job-shop problem which is analogous to processor in our problem). Processors may be identical or different.
#  m = 9				%3 dsps, 4 vcps, 2 fcps
#
#  %  Compute times of jobs on processors: {pij}, 1 i n, 1 j m. If a job i cannot be executed on a certain processor j, pij = inf, where inf is a large enough value to be specified later. This lets us model job-processor affinity.
#  p = [...
#	  200,	200,	200,	D,	D,	D,	D,	D,	D;...
#	  400,	400,	400,	D,	D,	D,	D,	D,	D;...	
#	  200,	200,	200,	D,	D,	D,	D,	D,	D;...
#	  600,	600,	600,	D,	D,	D,	D,	D,	D;...	
#	  200,	200,	200,	D,	D,	D,	D,	D,	D;...
#	  200,	200,	200,	D,	D,	D,	D,	D,	D;...
#	  200,	200,	200,	D,	D,	D,	D,	D,	D;...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...	
#	  ]
#
#  %  Communication costs between different processors (for some fixed transfer size): {qij}, 1 i m, 1 j m
#  q = [...
#	  0,	200,	200,	100,	100,	100,	100,	100,	100;...
#	  200,	0,	200,	100,	100,	100,	100,	100,	100;...
#	  200,	200	0,	100,	100,	100,	100,	100,	100;...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...
#	  ]
#
#
#  inp["n"] = n;
#  inp["d"] = d;
#  inp["D"] = D;
#  inp["G"] = G;
#  inp["m"] = m;
#  inp["p"] = p;
#  inp["q"] = q;
#end
#
#
#
#% 1  kFunctionUids_PacketSearch_PutUnalignedGetAligned,
#% 2  kFunctionUids_CfoCorrectorWifi_ApplyGainAndCorrectCfo,
#% 3  kFunctionUids_OfdmDemodulator64pCp16_demodulate,
#% 4  kFunctionUids_OfdmEqualizer_Equalize,
#% 
#% 5  kFunctionUids_SoftDemapper_BpskFromYhstar,
#% 6  kFunctionUids_DeinterleaverLutWifi_Deinterleave,
#% 7  kFunctionUids_ViterbiBranchMetrics_calculate,
#% 8  kFunctionUids_ViterbiDecoderWifi_MidDecode_Start,
#% 
#% 9  <mid decode happens on the VCP>
#%
#% 10 kFunctionUids_ViterbiDecoderWifi_WaitForCompletion,
#% 11 kFunctionUids_ViterbiDecoderWifi_MidDecode_Finish,
#% 12 kFunctionUids_DescramblerWifi_Descramble,
#% 13 kFunctionUids_Crc32Wifi_UpdateCrc32
#
#% G: 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 8 -> 9 -> 10 -> 11 -> 12 -> 13
#% As expressed by the user, the graph has total dependency for a single iteration. The scheduler
#% must come up with a pipelined schedule to meet throughput requirements.
#%
#% Compute times on cpu/vcp:
#% 1: 200, 2: 400, 3: 200, 4: 600, 
#% 5: 200, 6: 200, 7: 200, 8: 100, 
#% 9: 4000 (vcp), D (cpu)
#% 10: 100, 11: 200, 12: 100, 13: 100
#
#function [inp, opt] = wifi_6mbps_steady_state()
#  opt["lpoutfile"]					= 'wifi_6mbps_steady_state.lp';
#  opt["minimize_makespan"]				= 0;
#  %opt["minimize_makespan"]			= 1;
#
#  opt["constraints_no_overbook"] 			= 1;
#  opt["constraints_communication_allowance"] 	= 1;
#  opt["extra_information_zero_y_i_j"]		= 0;
#  opt["relaxation_x_not_integer"]			= 1;
#
#  %opt["constant_scale_down_factor"] 		= 100;
#  opt["constant_scale_down_factor"] 		= 1;
#
#  %  Given input:
#
#%  %  Compute graph G with n compute nodes called jobs {J1, J2, ..., Jn}. Each job needs to execute exactly once without preemption. Jobs have processor affinities characterized ahead.
#  n = 13; d = 10000; G = [ 1,2; 2,3; 3,4; 4,5; 5,6; 6,7; 7,8; 8,9; 9,10; 10,11; 11,12; 12,13];
#  %D = d + 100;
#  D = 2 * d;
#
#  %  Hardware definition consisting of m processors {M1, M2, ..., Mm}. (M is used to denote a machine in the notion of the well-known job-shop problem which is analogous to processor in our problem). Processors may be identical or different.
#  m = 9;				%3 dsps, 4 vcps, 2 fcps
#
#  %  Compute times of jobs on processors: {pij}, 1 i n, 1 j m. If a job i cannot be executed on a certain processor j, pij = inf, where inf is a large enough value to be specified later. This lets us model job-processor affinity.
#  p = [...
#	  200,	200,	200,	D,	D,	D,	D,	D,	D;...
#	  400,	400,	400,	D,	D,	D,	D,	D,	D;...	
#	  200,	200,	200,	D,	D,	D,	D,	D,	D;...
#	  600,	600,	600,	D,	D,	D,	D,	D,	D;...	
#  ...
#	  200,	200,	200,	D,	D,	D,	D,	D,	D;...
#	  200,	200,	200,	D,	D,	D,	D,	D,	D;...
#	  200,	200,	200,	D,	D,	D,	D,	D,	D;...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...	
#  ...
#	  D,	D,	D,	4000,	4000,	4000,	4000,	D,	D;...	
#  ...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...	
#	  200,	200,	200,	D,	D,	D,	D,	D,	D;...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...	
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...	
#	  ];
#
#  %  Communication costs between different processors (for some fixed transfer size): {qij}, 1 i m, 1 j m
#  q = [...
#	  0,	200,	200,	100,	100,	100,	100,	100,	100;...
#	  200,	0,	200,	100,	100,	100,	100,	100,	100;...
#	  200,	200	0,	100,	100,	100,	100,	100,	100;...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...
#	  ];
#
#
#  inp["n"] = n;
#  inp["d"] = d;
#  inp["D"] = D;
#  inp["G"] = G;
#  inp["m"] = m;
#  inp["p"] = p;
#  inp["q"] = q;
#end
#
#function [inp, opt] = wifi_9mbps_steady_state()
#
#end
#
#function [inp, opt] = example()
#  %opt["minimize_makespan"]			= 0;
#  opt["minimize_makespan"]				= 1;
#
#  opt["constraints_no_overbook"] 			= 1;
#  opt["constraints_communication_allowance"] 	= 1;
#  opt["extra_information_zero_y_i_j"]		= 0;
#  opt["relaxation_x_not_integer"]			= 1;
#
#  opt["constant_scale_down_factor"] 		= 1;
#  %opt["constant_scale_down_factor"] = 1;
#
#  %  Given input:
#
#%  d = 4800
#%  D = 5000
#
#%  d = 9600
#%  D = 10000
#
#%  %  Compute graph G with n compute nodes called jobs {J1, J2, ..., Jn}. Each job needs to execute exactly once without preemption. Jobs have processor affinities characterized ahead.
#%  n = 10				%number of jobs in the compute graph G
#%
#%
#%
#%  G = [...
#%	  1,	2;...
#%	  2,	3;...
#%	  3,	4;...
#%  ...
#%	  4,	5;...
#%	  5,	6;...
#%  ...
#%	  4,	7;...
#%	  7,	8;...
#%	  8,	9;...
#%  ...
#%	  6,	10;...
#%	  9,	10;...
#%	  ]
#
#
#  %n = 10; d = 12000; G = [ 1,2; 2,3; 3,4; 4,5; 5,6; 4,7; 7,8; 8,9; 6,10; 9,10;]
#  %n = 9; d = 9000; G = [ 1,2; 2,3; 3,4; 4,5; 5,6; 4,7; 7,8; 8,9; 6,9; ]
#  n = 9; d = 20000; G = [ 1,2; 2,3; 3,4; 4,5; 5,6; 4,7; 7,8; 8,9; 6,9; ]
#  %n = 8; d = 4800; G = [1,2; 2,3; 3,4; 4,5; 5,6; 4,7; 7,8;]
#  %n = 5; d = 2400; G = [ 1,2; 2,3; 3,4; 4,5; ] 
#
#  %G = randomDirectedGraph(n)
#
#  D = d + 100;
#
#  %  Hardware definition consisting of m processors {M1, M2, ..., Mm}. (M is used to denote a machine in the notion of the well-known job-shop problem which is analogous to processor in our problem). Processors may be identical or different.
#  m = 9;				%3 dsps, 4 vcps, 2 fcps
#
#  %  Compute times of jobs on processors: {pij}, 1 i n, 1 j m. If a job i cannot be executed on a certain processor j, pij = inf, where inf is a large enough value to be specified later. This lets us model job-processor affinity.
#  p = [...
#	  200,	200,	200,	D,	D,	D,	D,	D,	D;...
#	  400,	400,	400,	D,	D,	D,	D,	D,	D;...	
#	  600,	600,	600,	D,	D,	D,	D,	D,	D;...	
#	  600,	600,	600,	D,	D,	D,	D,	200,	200;...	
#  ...
#	  500,	500,	500,	D,	D,	D,	D,	D,	D;...	
#	  8000,	8000,	8000,	4000,	4000,	4000,	4000,	D,	D;...	
#  ...
#	  500,	500,	500,	D,	D,	D,	D,	D,	D;...	
#	  300,	300,	300,	D,	D,	D,	D,	D,	D;...	
#	  8000,	8000,	8000,	4000,	4000,	4000,	4000,	D,	D;...	
#  ...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...	
#	  ];
#
#  %  Communication costs between different processors (for some fixed transfer size): {qij}, 1 i m, 1 j m
#%  q = [...
#%	  0,	100,	100,	50,	50,	50,	50,	50,	50;...
#%	  100,	0,	100,	50,	50,	50,	50,	50,	50;...
#%	  100,	100	0,	50,	50,	50,	50,	50,	50;...
#%	  50,	50,	50,	D,	D,	D,	D,	D,	D;...
#%	  50,	50,	50,	D,	D,	D,	D,	D,	D;...
#%	  50,	50,	50,	D,	D,	D,	D,	D,	D;...
#%	  50,	50,	50,	D,	D,	D,	D,	D,	D;...
#%	  50,	50,	50,	D,	D,	D,	D,	D,	D;...
#%	  50,	50,	50,	D,	D,	D,	D,	D,	D;...
#%	  ]
#
#  q = [...
#	  0,	200,	200,	100,	100,	100,	100,	100,	100;...
#	  200,	0,	200,	100,	100,	100,	100,	100,	100;...
#	  200,	200	0,	100,	100,	100,	100,	100,	100;...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...
#	  ];
#
#
#  inp["n"] = n;
#  inp["d"] = d;
#  inp["D"] = D;
#  inp["G"] = G;
#  inp["m"] = m;
#  inp["p"] = p;
#  inp["q"] = q;
#end
#
#
#% 1  kFunctionUids_PacketSearch_PutUnalignedGetAligned,
#% 2  kFunctionUids_CfoCorrectorWifi_ApplyGainAndCorrectCfo,
#% 3  kFunctionUids_OfdmDemodulator64pCp16_demodulate,
#% 4  kFunctionUids_OfdmEqualizer_Equalize,
#% 
#% 5  kFunctionUids_SoftDemapper_64qamTable,
#% 6  kFunctionUids_DeinterleaverLutWifi_Deinterleave,
#% 7  kFunctionUids_ViterbiBranchMetrics_calculate,
#%
#% 8  kFunctionUids_ViterbiDecoderWifi_MidDecode_Start,
#% 9  kFunctionUids_ViterbiDecoderWifi_MidDecode_Start,
#% 10 kFunctionUids_ViterbiDecoderWifi_MidDecode_Start,
#% 11 kFunctionUids_ViterbiDecoderWifi_MidDecode_Start,
#% 
#% 12 <mid decode happens on the VCP>
#% 13 <mid decode happens on the VCP>
#% 14 <mid decode happens on the VCP>
#% 15 <mid decode happens on the VCP>
#%
#% 16 kFunctionUids_ViterbiDecoderWifi_WaitForCompletion,
#% 17 kFunctionUids_ViterbiDecoderWifi_WaitForCompletion,
#% 18 kFunctionUids_ViterbiDecoderWifi_WaitForCompletion,
#% 19 kFunctionUids_ViterbiDecoderWifi_WaitForCompletion,
#
#% 20 kFunctionUids_ViterbiDecoderWifi_MidDecode_Finish,
#% 21 kFunctionUids_ViterbiDecoderWifi_MidDecode_Finish,
#% 22 kFunctionUids_ViterbiDecoderWifi_MidDecode_Finish,
#% 23 kFunctionUids_ViterbiDecoderWifi_MidDecode_Finish,
#
#% 24 kFunctionUids_DescramblerWifi_Descramble,
#% 25 kFunctionUids_Crc32Wifi_UpdateCrc32
#
#% G: 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 
#%    				   7 -> 8 -> 12 -> 16 -> 20
#%    				   7 -> 9 -> 13 -> 17 -> 21
#%    				   7 -> 10 -> 14 -> 18 -> 22
#%    				   7 -> 11 -> 15 -> 19 -> 23
#%							  20 -> 24
#%							  21 -> 24
#%							  22 -> 24
#%							  23 -> 24
#%							  	24 -> 25
#
#% As expressed by the user, the graph has total dependency for a single iteration. The scheduler
#% must come up with a pipelined schedule to meet throughput requirements.
#%
#% Compute times on cpu/vcp:
#% 1: 200, 2: 400, 3: 200, 4: 600, 
#% 5: 200, 6: 200, 7: 200, 8: 100, 9: 100, 10:100, 11:100
#% 12, 13, 14, 15: 4000 (vcp), D (cpu)
#% 16, 17, 18, 19: 100
#% 20, 21, 22, 23: 200
#% 24: 100, 25: 100
#
#
#function [inp, opt] = wifi_54mbps_steady_state()
#  opt["lpoutfile"]					= 'wifi_54mbps_steady_state.lp';
#  %opt["minimize_makespan"]				= 0;
#  opt["minimize_makespan"]			= 1;
#
#  opt["constraints_no_overbook"] 			= 1;
#  opt["constraints_communication_allowance"] 	= 1;
#  opt["extra_information_zero_y_i_j"]		= 0;
#  opt["relaxation_x_not_integer"]			= 1;
#
#  %opt["constant_scale_down_factor"] 		= 100;
#  opt["constant_scale_down_factor"] 		= 1;
#
#  %  Given input:
#
#%  %  Compute graph G with n compute nodes called jobs {J1, J2, ..., Jn}. Each job needs to execute exactly once without preemption. Jobs have processor affinities characterized ahead.
#  %n = 25; d = 10000; G = [ 1,2; 2,3; 3,4; 4,5; 5,6; 6,7;...
#  %n = 25; d = 10000; G = [ 1,2; 2,3; 3,4; 4,5; 5,6; 6,7;...
#  %n = 25; d = 15000; G = [ 1,2; 2,3; 3,4; 4,5; 5,6; 6,7;...
#  n = 25; d = 20000; G = [ 1,2; 2,3; 3,4; 4,5; 5,6; 6,7;...
#  							7,8; 8,12; 12,16; 16,20;...
#							7,9; 9,13; 13,17; 17,21;...
#							7,10; 10,14; 14,18; 18,22;...
#							7,11; 11,15; 15,19; 19,23;...
#							20,24;...
#							21,24;...
#							22,24;...
#							23,24;...
#							   24,25;...
#							   ];
#  %D = d + 100;
#  D = 2 * d;
#
#  %  Hardware definition consisting of m processors {M1, M2, ..., Mm}. (M is used to denote a machine in the notion of the well-known job-shop problem which is analogous to processor in our problem). Processors may be identical or different.
#  m = 9;				%3 dsps, 4 vcps, 2 fcps
#
#  %  Compute times of jobs on processors: {pij}, 1 i n, 1 j m. If a job i cannot be executed on a certain processor j, pij = inf, where inf is a large enough value to be specified later. This lets us model job-processor affinity.
#  p = [...
#	  200,	200,	200,	D,	D,	D,	D,	D,	D;...
#	  400,	400,	400,	D,	D,	D,	D,	D,	D;...	
#	  200,	200,	200,	D,	D,	D,	D,	D,	D;...
#	  600,	600,	600,	D,	D,	D,	D,	D,	D;...	
#  ...
#	  200,	200,	200,	D,	D,	D,	D,	D,	D;...
#	  200,	200,	200,	D,	D,	D,	D,	D,	D;...
#	  200,	200,	200,	D,	D,	D,	D,	D,	D;...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...	
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...	
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...	
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...	
#  ...
#	  D,	D,	D,	4000,	4000,	4000,	4000,	D,	D;...	
#	  D,	D,	D,	4000,	4000,	4000,	4000,	D,	D;...	
#	  D,	D,	D,	4000,	4000,	4000,	4000,	D,	D;...	
#	  D,	D,	D,	4000,	4000,	4000,	4000,	D,	D;...	
#  ...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...	
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...	
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...	
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...	
#  ...
#	  200,	200,	200,	D,	D,	D,	D,	D,	D;...
#	  200,	200,	200,	D,	D,	D,	D,	D,	D;...
#	  200,	200,	200,	D,	D,	D,	D,	D,	D;...
#	  200,	200,	200,	D,	D,	D,	D,	D,	D;...
#  ...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...	
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...	
#	  ];
#
#  %  Communication costs between different processors (for some fixed transfer size): {qij}, 1 i m, 1 j m
#  q = [...
#	  0,	200,	200,	100,	100,	100,	100,	100,	100;...
#	  200,	0,	200,	100,	100,	100,	100,	100,	100;...
#	  200,	200	0,	100,	100,	100,	100,	100,	100;...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...
#	  100,	100,	100,	D,	D,	D,	D,	D,	D;...
#	  ];
#
#
#  inp["n"] = n;
#  inp["d"] = d;
#  inp["D"] = D;
#  inp["G"] = G;
#  inp["m"] = m;
#  inp["p"] = p;
#  inp["q"] = q;
#end
#
if __name__ == "__main__":
  main()
