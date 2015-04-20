'''
Atomix project, _db.py, (TODO: summary)
Copyright (c) 2015 Stanford University
Released under the Apache License v2.0. See the LICENSE file for details.
Author(s): Manu Bansal
'''
import sqlite3
import sys
from _util import *

class DB:
  ################################################################
  # DB select queries
  ################################################################
  def db_select_n_states(self):
    self.db.row_factory = sqlite3.Row
    c = self.db.cursor()
    c.execute('SELECT Count(*) FROM states')
    return c.fetchall()

  def db_select_n_cores(self):
    self.db.row_factory = sqlite3.Row
    c = self.db.cursor()
    c.execute('SELECT Max(corenum) FROM stateps')
    return c.fetchall()

  def db_select_states(self):
    self.db.row_factory = sqlite3.Row
    c = self.db.cursor()
    c.execute('SELECT * FROM states ORDER BY statenum')
    return c.fetchall()

  #def db_select_decways(self):
  #  self.db.row_factory = sqlite3.Row
  #  c = self.db.cursor()
  #  c.execute('SELECT * FROM decways JOIN atoms ON decways.instancename = atoms.instancename')
  #  return c.fetchall()

  def db_select_confs(self):
    self.db.row_factory = sqlite3.Row
    c = self.db.cursor()
    c.execute('SELECT * FROM confs JOIN atoms ON confs.instancename = atoms.instancename')
    return c.fetchall()

  def db_select_missingconfs(self):
    self.db.row_factory = sqlite3.Row
    c = self.db.cursor()
    c.execute('SELECT * FROM confs JOIN atoms ON confs.instancename = atoms.instancename')
    c.execute('''
    SELECT * FROM atoms 
    LEFT OUTER JOIN confs
    ON atoms.instancename = confs.instancename
    WHERE confs.instancename IS null
    AND atoms.hasConf = 1
    ''')
    return c.fetchall()

  def db_select_missingwires(self):
    self.db.row_factory = sqlite3.Row
    c = self.db.cursor()
    c.execute('SELECT * FROM wires JOIN atoms ON wires.instancename = atoms.instancename')
    c.execute('''
    SELECT * FROM atoms 
    LEFT OUTER JOIN wires 
    ON atoms.instancename = wires.instancename
    WHERE wires.instancename IS null
    ''')
    return c.fetchall()


  def db_select_stateps(self):
    self.db.row_factory = sqlite3.Row
    c = self.db.cursor()
    c.execute('SELECT * FROM stateps JOIN states ON stateps.statename = states.statename')
    return c.fetchall()

  def db_select_actions(self):
    self.db.row_factory = sqlite3.Row
    c = self.db.cursor()
    c.execute('SELECT * FROM actions')
    return c.fetchall()

  def db_select_action_atomseq(self, atomseqid):
    self.db.row_factory = sqlite3.Row
    c = self.db.cursor()
    self.logger.debug('atomseqid=%s' % atomseqid)
    ##c.execute('''SELECT * FROM 
    ##actionatomseqs JOIN atoms ON actionatomseqs.instancename = atoms.instancename
    ##WHERE actionatomseqs.atomseqid = %s
    ##ORDER BY orderid''' % atomseqid)
    ###''' % atomseqid)
    c.execute('''SELECT * FROM 
    actionatomseqs JOIN atom_wblocks ON actionatomseqs.instancename = atom_wblocks.atomname
    WHERE actionatomseqs.atomseqid = %s
    ORDER BY orderid''' % atomseqid)

    return c.fetchall()

  def db_select_state_atomseq(self, atomseqid):
    self.db.row_factory = sqlite3.Row
    c = self.db.cursor()
    self.logger.debug('atomseqid=%s' % atomseqid)
    #c.execute('''SELECT * FROM 
    #stateatomseqs JOIN atoms ON stateatomseqs.instancename = atoms.instancename
    #WHERE stateatomseqs.atomseqid = %s
    #ORDER BY orderid''' % atomseqid)
    c.execute('''SELECT * FROM 
    stateatomseqs JOIN atom_wblocks ON stateatomseqs.instancename = atom_wblocks.atomname
    WHERE stateatomseqs.atomseqid = %s
    ORDER BY orderid''' % atomseqid)
    #''' % atomseqid)
    return c.fetchall()

  ################################################################
  def db_select_fifo_buftypes(self, fifoname):
    self.db.row_factory = sqlite3.Row
    c = self.db.cursor()
    #self.db.execute('Create table fifos_buftypes_many (instancename, atomname, buftype, inpouttype)')
    #c.execute('SELECT DISTINCT instancename, buftype FROM fifos_buftypes_many WHERE instancename = "%s" AND buftype != "void"' % fifoname)
    c.execute('SELECT DISTINCT instancename, buftype FROM fifos_buftypes_many WHERE instancename = "%s" AND buftype != "void"' % fifoname)
    return c.fetchall()

  def db_is_fifo_wired_to_optimized_atom(self, fifoname):
    self.db.row_factory = sqlite3.Row
    c = self.db.cursor()
    c.execute('SELECT MAX(optimized_atom) FROM fifos_buftypes_many WHERE instancename = "%s"' % fifoname)
    dbresp = c.fetchall()
    val, = dbresp[0]
    return val 

  def db_select_fifo_buftypes_debug(self, fifoname):
    self.db.row_factory = sqlite3.Row
    c = self.db.cursor()
    #self.db.execute('Create table fifos_buftypes_many (instancename, atomname, buftype, inpouttype)')
    c.execute('SELECT DISTINCT * FROM fifos_buftypes_many WHERE instancename = "%s"' % fifoname)
    return c.fetchall()

  def db_select_fifos_untyped(self):
    self.db.row_factory = sqlite3.Row
    c = self.db.cursor()
    c.execute('SELECT * FROM fifos')
    return c.fetchall()

  def db_select_fifos_typed_unfilled(self):
    self.db.row_factory = sqlite3.Row
    c = self.db.cursor()
    #c.execute('''
    #SELECT * FROM fifos
    #LEFT OUTER JOIN fills
    #ON fifos.instancename = fills.fifo_name
    #WHERE fills.fifo_name IS null''')
    c.execute('''
    SELECT * FROM 
    (SELECT * FROM fifos JOIN fifos_buftypes_unique WHERE fifos.instancename = fifos_buftypes_unique.fifoname) AS a
    LEFT OUTER JOIN fills
    ON a.instancename = fills.fifo_name
    WHERE fills.fifo_name IS null''')
    return c.fetchall()

  def db_select_fifos_typed_filled(self):
    self.db.row_factory = sqlite3.Row
    c = self.db.cursor()
    c.execute('''
    SELECT * FROM fifos
    INNER JOIN fills
    ON fifos.instancename = fills.fifo_name''')
    return c.fetchall()


  def db_select_fills(self):
    self.db.row_factory = sqlite3.Row
    c = self.db.cursor()
    c.execute('SELECT * FROM fills')
    return c.fetchall()

  def db_select_links(self):
    self.db.row_factory = sqlite3.Row
    c = self.db.cursor()
    #c.execute('SELECT * FROM links')
    #c.execute('SELECT links.*, numbufs AS numbufs1, numbufs AS numbufs2 FROM links, fifos AS fifos1, fifos AS fifos2 \
    #	WHERE links.fifo_src = fifos1.instancename, links.fifo_dst = fifos2.instancename')
    #c.execute('SELECT links.*, numbufs FROM links, fifos WHERE links.fifo_src = fifos.instancename')
    c.execute('SELECT links.*, fifos_src.numbufs AS numbufs_src, fifos_src.coreid AS coreid_src, fifos_dst.numbufs AS numbufs_dst FROM links, fifos fifos_src, fifos fifos_dst \
    WHERE links.fifo_src = fifos_src.instancename AND links.fifo_dst = fifos_dst.instancename')
    return c.fetchall()

  def db_select_atom_row(self, atomname):
    self.db.row_factory = sqlite3.Row
    c = self.db.cursor()
    c.execute('SELECT * FROM atoms WHERE atoms.instancename="%s"' % atomname)
    return c.fetchall()

  def db_select_blocks(self):
    #self.db.execute('Create table atoms (instancename PRIMARY KEY, typename, coreid INTEGER, inptstr, outtstr, conftstr, hasConf INTEGER)')
    #self.db.execute('''Create table wires (instancename, fifolist,
    #		FOREIGN KEY(instancename) REFERENCES atoms(instancename)
    #		)''')

    self.db.row_factory = sqlite3.Row
    c = self.db.cursor()
    #c.execute('SELECT DISTINCT typename FROM atoms') 
    c.execute('SELECT wires.*, typename FROM wires, atoms WHERE wires.instancename = atoms.instancename')
    return c.fetchall()

  def db_select_atoms(self):
    #  def where(w, *a):
    #    c = self.db.cursor()
    #    c.execute('Select * From Users where %s' % w, *a)
    #    return c.fetchall()
    self.db.row_factory = sqlite3.Row
    c = self.db.cursor()
    #c.execute('SELECT * FROM atoms')
    c.execute('SELECT atoms.*, atom_wblocks.wblockname FROM atoms INNER JOIN atom_wblocks ON atoms.instancename = atom_wblocks.atomname')
    return c.fetchall()

  def db_select_wires(self):
    self.db.row_factory = sqlite3.Row
    c = self.db.cursor()
    #c.execute('SELECT * FROM wires')
    c.execute('SELECT wires.*, typename FROM wires, atoms WHERE wires.instancename = atoms.instancename')
    return c.fetchall()

  def db_select_wires_with_wblocknames(self):
    self.db.row_factory = sqlite3.Row
    c = self.db.cursor()
    #c.execute('SELECT * FROM wires')
    #c.execute('SELECT wires.*, typename FROM wires, atoms WHERE wires.instancename = atoms.instancename')
    c.execute('SELECT wires.*, wblockname FROM wires, atom_wblocks WHERE wires.instancename = atom_wblocks.atomname') 
    return c.fetchall()

  def db_select_wires_joined_atoms(self):
  #  self.db.execute('Create table atoms (instancename PRIMARY KEY, typename, coreid INTEGER, inptstr, outtstr, conftstr, hasConf INTEGER)')
  #  #self.db.execute('Create table fifos (instancename PRIMARY KEY, numbufs INTEGER, bufsize INTEGER, type STRING, coreid INTEGER)')
  #  self.db.execute('Create table fifos (instancename PRIMARY KEY, numbufs INTEGER, srcdsttype STRING, coreid INTEGER)')
  #  self.db.execute('Create table fifos_buftypes_many (instancename, atomname, buftype, inpouttype)')
  #  self.db.execute('Create table fifos_buftypes_unique (fifoname, buftype)')
  #
  #  self.db.execute('Create table fills (fifo_name, data STRING, FOREIGN KEY(fifo_name) REFERENCES fifos(instancename))')
  #  self.db.execute('''Create table links (fifo_src, fifo_dst, link_owner, 
  #		FOREIGN KEY(fifo_src) REFERENCES fifos(instancename),
  #		FOREIGN KEY(fifo_dst) REFERENCES fifos(instancename)
  #		)''')
  #  self.db.execute('''Create table wires (instancename, fifolist,
  #  		FOREIGN KEY(instancename) REFERENCES atoms(instancename)
  #		)''')
    self.db.row_factory = sqlite3.Row
    c = self.db.cursor()
    c.execute('SELECT * FROM wires INNER JOIN atoms ON wires.instancename = atoms.instancename')
    return c.fetchall()


  def db_select_paras(self):
    self.db.row_factory = sqlite3.Row
    c = self.db.cursor()
    c.execute('SELECT paras.*, typename FROM paras, atoms WHERE paras.instancename = atoms.instancename')
    return c.fetchall()

  def db_insert_fifo_access_table(self, fifo_access_table):
    self.db.execute('pragma foreign_keys=ON')
    c = self.db.cursor()
    for row in fifo_access_table:
      #fifo_access_row = (fifoname, atomname, inpouttype, fifocoreid, atomcoreid)
      c.execute('INSERT INTO fifo_access_table VALUES (?, ?, ?, ?, ?)', row)
    


  def db_select_fifo_access_table(self):
    self.db.row_factory = sqlite3.Row
    c = self.db.cursor()
    c.execute('SELECT * from fifo_access_table')
    return c.fetchall()


  def db_create(self):
    self.db = sqlite3.connect(':memory:')
    #self.db.execute('Create table Users (Name, Location, Type)')
    #self.db.execute('Create table atoms (instancename PRIMARY KEY, typename, coreid INTEGER)')
    self.db.execute('pragma foreign_keys=ON')
    #self.db.execute('Create table atoms (instancename PRIMARY KEY, typename, coreid INTEGER, inptstr, outtstr, conftstr, hasConf INTEGER)')
    self.db.execute('Create table atoms (instancename PRIMARY KEY, typename, coreid INTEGER, inptstr, outtstr, conftstr, hasConf INTEGER, optimized_atom INTEGER)')
    #self.db.execute('Create table fifos (instancename PRIMARY KEY, numbufs INTEGER, bufsize INTEGER, type STRING, coreid INTEGER)')
    self.db.execute('Create table fifos (instancename PRIMARY KEY, numbufs INTEGER, srcdsttype STRING, coreid INTEGER)')
    self.db.execute('Create table fifos_buftypes_many (instancename, atomname, buftype, inpouttype, optimized_atom INTEGER)')
    self.db.execute('Create table fifos_buftypes_unique (fifoname, buftype, fifo_optimized INTEGER)')

    self.db.execute('''Create table fifo_access_table (fifoname, atomname, inpouttype, fifocoreid, atomcoreid)''')

    self.db.execute('''Create table fifos_whether_multicore (fifoname, is_multicore BOOLEAN,
		  FOREIGN KEY (fifoname) REFERENCES fifos (instancename)
		  )''')

    self.db.execute('Create table fills (fifo_name, data STRING, FOREIGN KEY(fifo_name) REFERENCES fifos(instancename))')
    self.db.execute('''Create table links (fifo_src, fifo_dst, link_owner, 
		  FOREIGN KEY(fifo_src) REFERENCES fifos(instancename),
		  FOREIGN KEY(fifo_dst) REFERENCES fifos(instancename)
		  )''')
    self.db.execute('''Create table wires (instancename, fifolist,
		  FOREIGN KEY(instancename) REFERENCES atoms(instancename)
		  )''')
    self.db.execute('''Create table paras (instancename, parslist,
		  FOREIGN KEY(instancename) REFERENCES atoms(instancename)
		  )''')

    #wblockrow = (atomname, wblockname, blockname, inptstr, outtstr, conftstr, hasConf, fifo_access_signature_string)
    self.db.execute('''Create table atom_wblocks (atomname, wblockname, blockname, inptstr, outtstr, conftstr, hasConf, fifo_access_signature_string,
		  FOREIGN KEY(atomname) REFERENCES atoms(instancename)
		  )''')
		  

    #self.db.execute('Create table actions (axnname PRIMARY KEY NOT NULL, atomseqid INTEGER AUTO_INCREMENT)')
    self.db.execute('Create table actions (axnname UNIQUE NOT NULL, atomseqid INTEGER PRIMARY KEY)')
    
    self.db.execute('''Create table states (statename UNIQUE NOT NULL, statenum INTEGER PRIMARY KEY)''')

  #  self.db.execute('''Create table states (statename UNIQUE NOT NULL, statenum INTEGER, corenum INTEGER, axnname, 
  #		atomseqid INTEGER AUTO_INCREMENT, 
  #		PRIMARY KEY (statenum, corenum),
  #		FOREIGN KEY (axnname) REFERENCES actions(axnname)
  #		)''')
    self.db.execute('''Create table stateps (statename NOT NULL, corenum INTEGER, axnname, 
		  atomseqid INTEGER PRIMARY KEY, 
		  UNIQUE (statename, corenum),
		  FOREIGN KEY (axnname) REFERENCES actions(axnname)
		  )''')

  #  self.db.execute('''Create table actionatomseqs 
  #		(atomseqid, orderid INTEGER AUTO_INCREMENT, instancename NOT NULL, 
  #		FOREIGN KEY (atomseqid) REFERENCES actions(atomseqid),
  #		FOREIGN KEY (instancename) REFERENCES atoms(instancename)
  #		)''')
    self.db.execute('''Create table actionatomseqs 
		  (atomseqid, orderid INTEGER PRIMARY KEY, instancename NOT NULL, 
		  FOREIGN KEY (atomseqid) REFERENCES actions(atomseqid),
		  FOREIGN KEY (instancename) REFERENCES atoms(instancename)
		  )''')

  #  self.db.execute('''Create table stateatomseqs 
  #  		(atomseqid , orderid INTEGER AUTO_INCREMENT, instancename NOT NULL, 
  #		FOREIGN KEY (atomseqid) REFERENCES states(atomseqid),
  #		FOREIGN KEY (instancename) REFERENCES atoms(instancename)
  #		)''')
    self.db.execute('''Create table stateatomseqs 
		  (atomseqid, orderid INTEGER PRIMARY KEY, instancename NOT NULL, 
		  FOREIGN KEY (atomseqid) REFERENCES stateps(atomseqid),
		  FOREIGN KEY (instancename) REFERENCES atoms(instancename)
		  )''')

  #  self.db.execute('''Create table decways (instancename, way, statename,
  #  		UNIQUE (instancename, way, statename),
  #		FOREIGN KEY (instancename) REFERENCES atoms(instancename)
  #		FOREIGN KEY (statename) REFERENCES states(statename)
  #		)''')

    self.db.execute('''Create table confs (instancename NOT NULL, vals,
		  UNIQUE (instancename),
		  FOREIGN KEY (instancename) REFERENCES atoms(instancename)
		  )''')
    
  def db_select_statep_atoms_core_integrity_query(self):
    self.db.row_factory = sqlite3.Row
    c = self.db.cursor()
    c.execute('''SELECT * FROM 
    stateps 
    INNER JOIN stateatomseqs ON stateps.atomseqid = stateatomseqs.atomseqid
    INNER JOIN atoms ON stateatomseqs.instancename = atoms.instancename
    ''')
    return c.fetchall()
    
  def db_select_axn_atoms_core_integrity_query(self):
    self.db.row_factory = sqlite3.Row
    c = self.db.cursor()
    c.execute('''SELECT * FROM 
    stateps 
    INNER JOIN actions ON stateps.axnname = actions.axnname 
    INNER JOIN actionatomseqs ON actions.atomseqid = actionatomseqs.atomseqid
    INNER JOIN atoms ON actionatomseqs.instancename = atoms.instancename
    ''')
    return c.fetchall()

  def db_insert_fifo_is_multicore_row(self, row):
    #row: (fifoname, fifo_is_multicore)
    self.db.execute('pragma foreign_keys=ON')
    c = self.db.cursor()
    c.execute('INSERT INTO fifos_whether_multicore VALUES (?, ?)', row)
    

  def db_select_fifos_whether_multicore(self):
    self.db.row_factory = sqlite3.Row
    c = self.db.cursor()
    c.execute('SELECT * FROM fifos_whether_multicore')
    return c.fetchall()
    

  def db_insert_atom_wblock_row(self, row):
    self.logger.debug(row)
    self.db.execute('pragma foreign_keys=ON')
    c = self.db.cursor()
    #wblockrow = (atomname, wblockname, blockname, inptstr, outtstr, conftstr, hasConf, fifo_access_signature_string)
    c.execute('INSERT INTO atom_wblocks VALUES (?, ?, ?, ?, ?, ?, ?, ?)', row)
    

  def db_select_distinct_wblocks(self):
    #wblockrow = (atomname, wblockname, blockname, inptstr, outtstr, conftstr, hasConf, fifo_access_signature_string)
    #self.db.execute('''Create table atom_wblocks (atomname, wblockname, blockname, inptstr, outtstr, conftstr, hasConf, fifo_access_signature_string)
    #		FOREIGN KEY(atomname) REFERENCES atoms(instancename)
    #		)''')
    self.db.row_factory = sqlite3.Row
    c = self.db.cursor()
    c.execute('SELECT DISTINCT wblockname, blockname, inptstr, outtstr, conftstr, hasConf, fifo_access_signature_string FROM atom_wblocks')
    return c.fetchall()

  def db_select_wblock_row(self, wblockname):
    #wblockrow = (atomname, wblockname, blockname, inptstr, outtstr, conftstr, hasConf, fifo_access_signature_string)
    #self.db.execute('''Create table atom_wblocks (atomname, wblockname, blockname, inptstr, outtstr, conftstr, hasConf, fifo_access_signature_string)
    #		FOREIGN KEY(atomname) REFERENCES atoms(instancename)
    #		)''')
    self.db.row_factory = sqlite3.Row
    c = self.db.cursor()
    c.execute('SELECT DISTINCT wblockname, blockname, inptstr, outtstr, conftstr, hasConf, fifo_access_signature_string FROM atom_wblocks WHERE wblockname="%s"' % wblockname)
    return c.fetchall()


  def db_insert_fifos_buftype_unique_row(self, row):
    self.logger.debug(row)
    self.db.execute('pragma foreign_keys=ON')
    c = self.db.cursor()
    c.execute('INSERT INTO fifos_buftypes_unique VALUES (?, ?, ?)', row)
    

  def db_insert_fifos_buftypes_many_row(self, row):
    self.logger.debug(row)
    self.db.execute('pragma foreign_keys=ON')
    #self.db.execute('Create table fifos_buftypes_many (instancename, atomname, buftype, inpouttype)')
    c = self.db.cursor()
    c.execute('INSERT INTO fifos_buftypes_many VALUES (?, ?, ?, ?, ?)', row)
    


  def db_insert_action_row(self, action_row):
    self.logger.debug(action_row)
    self.db.execute('pragma foreign_keys=ON')
    c = self.db.cursor()
    c.execute('INSERT INTO actions (axnname) VALUES (?)', action_row)
    atomseqid = c.lastrowid
    self.logger.debug("last actions atomseqid=%d" % (atomseqid))
    self.db.commit()
    #
    return atomseqid

  def db_insert_state_row(self, state_row):
    self.logger.debug(state_row)
    self.db.execute('pragma foreign_keys=ON')
    c = self.db.cursor()
    c.execute('INSERT INTO states (statename) VALUES (?)', state_row)
    stateid = c.lastrowid
    self.logger.debug("last state's id=%d" % (stateid))
    self.db.commit()
    #
    return stateid 
    
  def db_insert_statep_row(self, statep_row):
    self.logger.debug(statep_row)
    self.db.execute('pragma foreign_keys=ON')
    c = self.db.cursor()
    c.execute('INSERT INTO stateps (statename, corenum, axnname) VALUES (?, ?, ?)', statep_row)
    atomseqid = c.lastrowid
    self.logger.debug("last statep's atomseqid=%d" % (atomseqid))
    self.db.commit()
    #
    return atomseqid

  def db_insert_actionatomseq_row(self, atomseqid, atomseq):
    self.logger.debug('%s, %s' % (atomseqid, atomseq))
    self.db.execute('pragma foreign_keys=ON')
    c = self.db.cursor()
    atoms = atomseq.split(';')
    lastatom = atoms[-1]
    if lastatom != '':
      self.logger.error('PARSE ERROR: Missing a semicolon in atom sequence definition?')
      exit(9)
    atoms = atoms[0:-1]
    for atom in atoms:
      atom = atom.strip()
      self.logger.debug('Query: c.execute(\'INSERT INTO actionatomseqs (atomseqid, instancename) values (?, ?)\', (atomseqid, atom) with (%s, %s)' % (atomseqid, atom))
      try:
	c.execute('INSERT INTO actionatomseqs (atomseqid, instancename) values (?, ?)', (atomseqid, atom))
	orderid = c.lastrowid
      except (sqlite3.OperationalError, sqlite3.IntegrityError) as e:
	print_line()
	self.logger.error("error parsing action: Atom named %s is not defined." % atom)
	print_line()
	#db_atomseqids = self.db.execute('''SELECT atomseqid FROM states''')
	#db_atomnames  = self.db.execute('''SELECT instancename FROM atoms''')
	sys.exit(1)
    self.db.commit()

  def db_insert_stateatomseq_row(self, atomseqid, atomseq):
    self.logger.debug('%s, %s' % (atomseqid, atomseq))
    self.db.execute('pragma foreign_keys=ON')
    c = self.db.cursor()
    atoms = atomseq.split(';')
    lastatom = atoms[-1]
    if lastatom != '':
      self.logger.error('PARSE ERROR: Missing a semicolon in atom sequence definition?')
      exit(10)
    atoms = atoms[0:-1]
    for atom in atoms:
      atom = atom.strip()
      self.logger.debug('Inserting atom "%s" into stateatomseqs' % atom)
      try:
	c.execute('INSERT INTO stateatomseqs (atomseqid, instancename) values (?, ?)', (atomseqid, atom))
	orderid = c.lastrowid
      except sqlite3.OperationalError as e:

  #      self.db.execute('''Create table stateatomseqs 
  #  		(atomseqid, orderid INTEGER PRIMARY KEY, instancename NOT NULL, 
  #		FOREIGN KEY (atomseqid) REFERENCES states(atomseqid),
  #		FOREIGN KEY (instancename) REFERENCES atoms(instancename)
  #		)''')
	self.logger.error("sqlite3.OperationalError")
	self.logger.error(e)
	db_atomseqids = self.db.execute('''SELECT atomseqid FROM states''')
	db_atomnames  = self.db.execute('''SELECT instancename FROM atoms''')
	for row in db_atomseqids:
	  self.logger.error('atomseqid: "%s"' % row["atomseqid"])
	for row in db_atomnames:
	  self.logger.error('atomname: "%s"' % row["instancename"])
	sys.exit(1)
    self.db.commit()

  #def db_insert_decway_row(self, decway_row):
  #  c = self.db.cursor()
  #  c.execute('INSERT INTO decways VALUES (?, ?, ?)', decway_row)
  #  self.db.commit()
  #  

  def db_insert_conf_row(self, conf_row):
    self.logger.debug(conf_row)
    self.db.execute('pragma foreign_keys=ON')
    c = self.db.cursor()
    c.execute('INSERT INTO confs VALUES (?, ?)', conf_row)
    self.db.commit()
    

  #  self.db.executemany('Insert into Users values(:Name, :Location, :Type)', [
  #     dict(Name="Mr. Foo", Location="Boston", Type="Secondary"),
  #     dict(Name="Mr. Bar", Location="New York", Type="Primary"),
  #     dict(Name="Mr. Quux", Location="Chicago", Type="Secondary"),
  #     ])
  #  self.db.commit()
  #  self.db.row_factory = sqlite3.Row
  #
  #  and now your in-memory tiny "db" is ready to go. It's no harder to make a DB in a disk file and/or read the initial values from a text file, a CSV, and so forth, of course.
  #
  #  Querying is especially flexible, easy and sweet, e.g., you can mix string insertion and parameter substitution at will...:
  #
  #  def where(w, *a):
  #    c = self.db.cursor()
  #    c.execute('Select * From Users where %s' % w, *a)
  #    return c.fetchall()

  def db_insert_fifo_row(self, fifo_row):
    #self.db.execute('INSERT INTO fifos VALUES (?, ?, ?, ?, ?)', fifo_row)
    self.logger.debug(fifo_row)
    self.db.execute('pragma foreign_keys=ON')
    self.db.execute('INSERT INTO fifos VALUES (?, ?, ?, ?)', fifo_row)
    self.db.commit()
    

  def db_insert_fill_row(self, fill_row):
    self.logger.debug(fill_row)
    self.db.execute('pragma foreign_keys=ON')
    self.db.execute('INSERT INTO fills VALUES (?, ?)', fill_row)
    self.db.commit()
    

  def db_insert_link_row(self, link_row):
    self.logger.debug(link_row)
    self.db.execute('pragma foreign_keys=ON')
    self.db.execute('INSERT INTO links VALUES (?, ?, ?)', link_row)
    self.db.commit()
    

  def db_insert_atom_row(self, atom_row):
    self.logger.debug(atom_row)
    self.db.execute('pragma foreign_keys=ON')
    #self.db.execute('INSERT INTO atoms VALUES (?, ?, ?)', atom_row)
    self.db.execute('INSERT INTO atoms VALUES (?, ?, ?, ?, ?, ?, ?, ?)', atom_row)
    self.db.commit()
    

  def db_insert_wire_row(self, wire_row):
    self.logger.debug(wire_row)
    self.db.execute('pragma foreign_keys=ON')
    self.db.execute('INSERT INTO wires VALUES (?, ?)', wire_row)
    self.db.commit()
    

  def db_insert_pars_row(self, pars_row):
    self.logger.debug(pars_row)
    self.db.execute('pragma foreign_keys=ON')
    self.db.execute('INSERT INTO paras VALUES (?, ?)', pars_row)
    self.db.commit()
    

  def __init__(self, logger):
    self.logger = logger
    self.db_create()
