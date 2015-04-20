#!/usr/bin/env python
'''
Atomix project, gencplex.py, (TODO: summary)
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
'''

import sys
from lpinput import *
from math import *
from scipy.sparse import coo_matrix

def main(modelfile):
#  modelfile = sys.argv[1]
  print modelfile
  genschedlp(modelfile)
  gencplexscript(modelfile)

def gencplexscript(modelfile):
  modname = modelfile.split('.py')[0]
  print modname
  outfile = modname + '.cplex'
  fp=open(outfile,'w');
  fp.write('read ' + modname + '.lp\n')
  fp.write('mipopt\n')
  fp.write('write ' + modname + '.sol\n')
  #fp.write('y\n')
  fp.write('quit\n\n');  
  fp.close()

def genschedlp(modelfile):
  #outfile='schedlp.lp';
  modname = modelfile.split('.py')[0]
  print modname
  outfile = modname + '.lp'

  global fp;

  [inp, opt] = lpinput(modelfile);
  #print inp
  #print opt
  
  #if (opt["lpoutfile"]):
  #  outfile = opt["lpoutfile"]
  #end
  fp=open(outfile,'w');

  n = inp["n"];
  d = inp["d"];
  D = inp["D"];
  G = inp["G"];
  m = inp["m"];
  p = inp["p"];
  q = inp["q"];
  T = inp["T"];

  opt["constraints_modulo_scheduling"] = 1;

  if (opt["constraints_modulo_scheduling"]):
 #   T = 4000; #inp["T"];
    R = int(floor(d/T));

  print("G is :\n")
  print G
  #raw_input()
  GI = []; GJ = []; Gdata = []
  Grows = G.split('\n')[1:]
  for row in Grows[:-1]:
    print "----"
    print row
    #raw_input()
    #GI.append(int(row.split(' ')[0].strip()) - 1) Kanthi commented out - check with manu
    #GJ.append(int(row.split(' ')[1].strip()) - 1)
    GI.append(int(row.split(' ')[0].strip()))
    GJ.append(int(row.split(' ')[1].strip()))
    Gdata.append(int(row.split(' ')[2].strip()))

  #for row in zip(I,J):
  #  print row

  #Gsparse = sparse(G(:,1),G(:,2),1,n,n);
  #Gsparse_undir = Gsparse+Gsparse';
  Gsparse = coo_matrix((Gdata, (GI, GJ)), shape=(n,n), dtype=np.int)
  Gsparse_undir = Gsparse + Gsparse.transpose()
  #Gdense_undir = Gsparse_undir.todense()
  #print Gdense_undir
  #raw_input()
  #print Gsparse_undir

  #-------------------------------------------------------------------------------------------------------------------------
  d = d/opt["constant_scale_down_factor"];
  D = D/opt["constant_scale_down_factor"];
  p = p/opt["constant_scale_down_factor"];
  q = q/opt["constant_scale_down_factor"];
  #-------------------------------------------------------------------------------------------------------------------------

  #-------------------------------------------------------------------------------------------------------------------------
  #  Output variables:

  #  Processing start times of jobs {xi}, 1 i n

  #  Processor assignment indicator variables {yij}, 1 i m, 1 j m where yij = 1 if Ji is assigned to Mj, 0 otherwise.

  #  Intermediate or derived variables:
  #  Job completion times {C_i}, 1 <= i <= n  where C_i = x_i + sum_{j} y_ij p_ij,  1 <= j <= m

  #  Objective function, makespan Cmax, which is defined in terms of Ci variables through a constraint in the program.
  #  Pairwise job-order indicator variables {zijk} where zijk  = 1 if Ji and Jj are both assigned to Mk and precedes Jj in the schedule, 0 if both are assigned to Mk and Jj precedes Ji. These variables are used to place serialization constraints on jobs assigned to the same processor so that no processor is allocated multiple jobs for the same time slot. In case the jobs are assigned to different processors, zijk can assume either value without affecting the linear program.
  #  Dependent-job collocation indicator variables {ust} where ust = (yis +yjt - 2). Thus, ust is 0 if Ji is assigned to Ms and Jj is assigned to Mt and greater than 0 in all other cases. These variables are used to isolate the applicable communication cost between the two jobs.

  #-------------------------------------------------------------------------------------------------------------------------
  #  ILP description:
  #  Minimize Cmax  (optimization objective)
  if (opt["minimize_makespan"]):
    out('Minimize')
    out('obj: C_max')
  else:
    out('min: ;')

  out('Subject To')

  #  such that
  #  Job completion times {C_i}, 1 <= i <= n  where C_i = x_i + sum_{j} y_i_j p_i_j,  1 <= j <= m
  for i in range(n):
    #istr = str(i)
    istr = str(i+1)
    ypsumstr='';
    for j in range(m):
      #jstr = str(j)
      jstr = str(j+1)

      #eliminate y_i_j's where y_i_j is known to be zero
      if (p[i][j] == D):
        continue

      ypsumstr = ypsumstr + ' - ' + str(p[i][j]) + ' ' + 'y_' + istr + '_' + jstr
    out('C_' + istr + ' - x_' + istr + ypsumstr + ' = 0')

  #  Cmax <= d (deadline and throughput constraint)
  #out('C_max <= d;')
  cn = n+1;
  out('C_max <= ' + str(d))

  #  Ci <= Cmax  for 1 <= i <= n  (objective function definition)
  for i in range(n):
    #out('C_' + str(i) + ' - C_max <= 0');
    out('C_' + str(i+1) + ' - C_max <= 0');

  cn = cn + n;

  #%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  #out('/* Each job is allocated to one and only one processor. */');
  #%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  #  sum_{j} y_i_j = 1 for 1 <= i <= n, 1 <= j <= m (each job is allocated to exactly one processor)
  for i in range(n):
    #istr = str(i);
    istr = str(i+1);
    ysumstr='';
    for j in range(m):
      #jstr = str(j);
      jstr = str(j+1);

      #eliminate y_i_j's where y_i_j is known to be zero
      if (p[i][j] == D):
        continue;

      if ysumstr == '':
        ysumstr = 'y_'+istr+'_'+jstr
      else:
        ysumstr = ysumstr+' + '+'y_'+istr+'_'+jstr

    if ysumstr != '':
      out(ysumstr+' = 1');

  Dstr = str(D);
  bintvarlist = '';

  #%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  #out('/* Ensure processors are not overbooked. */');
  #%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  #  (2 - y_i_k - y_j_k)D  + z_i_j_k (p_j_k + D)  + (x_i - x_j) >= p_j_k  for 1 <= i, j <= n, 1 <= k <= m
  #  (2 - y_i_k - y_j_k)D  + (1 - z_i_j_k) (p_i_k + D)  + (x_j - x_i) >= p_i_k  for 1 <= i, j <= n, 1 <= k <= m
  #  (processors are not overbooked)
  if (opt["constraints_no_overbook"]):
    for i in range((n-1)):
      #istr = str(i);
      istr = str(i+1);
      for j in range(i+1,n):
        #jstr = str(j);
        jstr = str(j+1);
        for k in range(m):
          #kstr = str(k);
          kstr = str(k+1);

          #eliminate constraints where y_i_k or y_j_k is known to be zero
          if (p[i][k] == D or p[j][k] == D):
            continue;

          #if the node pair already has an edge between them, they will be serialized by the dependency/comm constraint
          #so we can eliminate it from overbooking constraints
	  #print i, j, n, n
	  #print Gsparse_undir.shape
	  #print Gdense_undir.shape
          #if (Gsparse_undir[i][j]):
          #if (Gdense_undir[i][j]):
          if (Gsparse_undir[i,j]):
            continue;

          out(Dstr+' y_'+istr+'_'+kstr+' + '+Dstr+' y_'+jstr+'_'+kstr+' - '+
            str(p[j][k] + D)+' z_'+istr+'_'+jstr+'_'+kstr+' - x_'+istr+' + x_'+jstr+' <= '+str(D * 2 - p[j][k]));

          out(Dstr+' y_'+istr+'_'+kstr+' + '+Dstr+' y_'+jstr+'_'+kstr+' + '+
        str(p[j][k] + D)+' z_'+istr+'_'+jstr+'_'+kstr+' - x_'+jstr+' + x_'+istr+' <= '+str(p[j][k] + D * 3 - p[i][k]));

          bintvarlist = bintvarlist+ ' z_'+istr+'_'+jstr+'_'+kstr;

  #  xi + kyik pik < xj  for 1i, j n, 1k m where G has an edge (Ji, Jj) (dependency constraints)
  #  x_j - x_i >= y_i_s p_i_s + q_s_t + D(y_i_s + y_j_t - 2) for 1 <= i, j <= n, 1 <= s, t <= m where G has an edge (J_i, J_j) (communication cost allowance)
  if (opt["constraints_communication_allowance"]):
    #%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    #out('/* Dependency constraints with communication allowance. */');
    #%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    Dstr = str(D);
    Dstr2 = str(D * 2);
    for ei in range(len(GI)):
      #[i j] = G(ei, :);
      i = GI[ei]
      j = GJ[ei]
      #istr = str(i);
      istr = str(i+1);
      #jstr = str(j);
      jstr = str(j+1);
      for s in range(m):
        sstr = str(s+1);
        for t in range(m):
          #tstr = str(t);
          tstr = str(t+1);
          #out(['x_',jstr,' - x_',istr,' >= ',str(p(i,s)),' y_',istr,'_',sstr,' + ',str(q(s,t) * D),' (y_',istr,'_',sstr,' + y_',jstr,'_',tstr,' - 2)']);

          #an obvious optimization that is still functionally equivalent
          #out(['x_',jstr,' - x_',istr,' >= ',str(p(i,s)),' y_',istr,'_',sstr,' + ',str(q(s,t) + 2 * D),' (y_',istr,'_',sstr,' + y_',jstr,'_',tstr,' - 2)',';']);
          #fstr = str(q(s,t) + 2 * D);
          #fstr2 = str(2*(q(s,t) + 2 * D));

          #eliminate constraints where y_n_m is known to be 0
          if (p[i][s] == D or p[j][t] == D):
            continue;

	  print s, t, q[s][t]
          out('- x_'+jstr+' + x_'+istr+' + '+str(p[i][s])+' y_'+istr+'_'+sstr+' + '+
	    Dstr+' y_'+istr+'_'+sstr+ ' + '+ Dstr+' y_'+jstr+'_'+tstr+ ' <= '+str(D*2 - q[s][t]));

  for i in range(n):
    #istr = str(i);
    istr = str(i+1);
    out('x_'+istr+' >= 0');

  if (opt["extra_information_zero_y_i_j"]):
    #[I, J] = find(p == D);
    [I, J] = np.nonzero(p == D);
    for ii in range(len(I)):
      #istr = str(I[ii]); jstr = str(J[ii]);
      istr = str(I[ii]+1); jstr = str(J[ii]+1);
      out('y_'+istr+'_'+jstr+' = 0');


  #  Note that the constraints for communication cost allowance are stronger than dependency constraints. Thus, dependency constraints are redundant and eliminated from the program when communication constraints are imposed.

  #  D is an ???infinite??? value used to render constraints useless whenever D is included. A sufficiently large value for D needs to be chosen for the desired effect. Brief inspection suggests that an D > d will suffice. Since the complexity of ILP solution is pseudo-polynomial time that includes a polynomial term in the largest input ???value???, it is important to use the smallest possible value for D to avoid blowing up the search space and computation time. Hence, we can choose D = d + 1.


  

  #(2 - yik - yjk)D  + zijkr (pjk+D)+(xi - xjr) >= pjk  for 1 <= i < j <= n, 1 <= k <= m, 0 < |r| <= R
  #(2 - yik - yjk)D  + (1-zijkr) (pik+D)+(xjr - xi) >= pik  for 1 <= i < j <= n, 1 <= k <= m, 0 < |r| <= R
  #(schedule can be repeated modulo symbol arrival period/iteration period T)
  #
  #where zijkr are arbitrary binary integers, R = floor(d/T) and xjr = xj + r.T for an integer r.
  #
  if (opt["constraints_modulo_scheduling"]):
      #%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      #out('/* Modulo scheduling constraints for throughput matching. */');
      #%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      Dstr2 = str(D * 2);
      for r in range(R):
          #rstr = str(r);
          rstr = str(r+1);
          for i in range(n):
              #istr = str(i);
              istr = str(i+1);
              for j in range(n):
        	  if j == i:
        	      continue;
        	  #jstr = str(j);
        	  jstr = str(j+1);
        	  for k in range(m):
        	      #kstr = str(k);
        	      kstr = str(k+1);
        	      
        	      #eliminate constraints where y_i_k or y_j_k is known to be zero
        	      if (p[i][k] == D or p[j][k] == D):
        		  continue;
        	      
        	      #out('x_'+jstr+'_'+rstr+' - x_'+jstr+' = '+str(r * T));
        	      out('x_'+jstr+'_'+rstr+' - x_'+jstr+' = '+str((r + 1) * T));
        	      
        	      out(' - '+Dstr+' y_'+istr+'_'+kstr+' - '+Dstr+' y_'+jstr+'_'+kstr+' + '+
        		  str(p[j][k] + D)+' z_'+istr+'_'+jstr+'_'+kstr+'_'+rstr+' + '+
        		  'x_'+istr+' - x_'+jstr+'_'+rstr+' >= '+str(p[j][k] - 2*D));
        	      
        	      out(' - '+Dstr+' y_'+istr+'_'+kstr+' - '+Dstr+' y_'+jstr+'_'+kstr+
        		  ' - '+str(p[j][k] + D)+' z_'+istr+'_'+jstr+'_'+kstr+'_'+rstr+' + '+
        		  'x_'+jstr+'_'+rstr+' - x_'+istr+' >= '+str(p[i][k] - 3*D - p[j][k]));
        	      
        	      bintvarlist = bintvarlist+ ' z_'+istr+'_'+jstr+'_'+kstr+'_'+rstr;

  if (~opt["relaxation_x_not_integer"]):
    #intvarlist = 'x_1';
    #for i = 2:n
    intvarlist='';
    for i in range(n):
      #istr = str(i);
      istr = str(i+1);
      #intvarlist = [intvarlist, ', x_',istr];
      intvarlist = intvarlist+ ' x_'+istr;
    out('General');
    out(intvarlist);


  for i in range(n):
    #istr = str(i);
    istr = str(i+1);
    for k in range(m):
      #kstr = str(k);
      kstr = str(k+1);

      #eliminate y_i_k where it is not part of our system
      if (p[i][k] == D):
        continue;

      bintvarlist = bintvarlist+ ' y_'+istr+'_'+kstr;

  #for i in range(n):
  #  istr = str(i);
  #  for j in range(n):
  #    jstr = str(j);
  #    for k in range(m):
  #      kstr = str(k);
  #      bintvarlist = [bintvarlist, ' z_',istr,'_',jstr,'_',kstr];
  #    end
  #  end
  #end

  out('Binary');
  #newline = sprintf('\n');
  #bintvarlist = strrep(bintvarlist, ' ', newline);
  bintvarlist = '\n'.join(bintvarlist.split(' '))
  out(bintvarlist);
  out('End');

  fp.close()

def out(str):
  #display(str);
  #fprintf(1,'%s\n',str);

  global fp;
  fp.write('%s\n' % str);

if __name__ == "__main__":
  main(sys.argv[1])
