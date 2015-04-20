## Application Programming ##

### Atoms, FIFOs, States ###
TODO

### Debugging, Profiling, and Optimization ###

#### debug.or ####
debug.or is a powerful debugging interface to your Atomix application. 
It lets you log FIFO buffers, trace atom execution sequences and profile 
atoms for timing. debug.or supports the following API:

```
identify:<identifiedAtom>
debug:<debuggedAtom>
ts_atii:...
ts_at:
ts_st:...
ts_ax: ...
```

`identify` makes an entry to be logged every time `identifiedAtom` executes. This is useful 
when you want to debug the control flow of your application.

`debug` makes the input and output buffer contents of `debuggedAtom` to be logged every
time `debuggedAtom` executes. This is useful for debugging the correctness of block implementations
through blackbox testing. Atomix includes a utility program called `parsedat` with a powerful set of
functions suited to comparing buffers in a signal processing application (e.g., comparing complex numbers,
or correlating buffers with varying shifts to detect offsets between reference and test vectors).



#### parsedat ####
TODO


* tsconf
*

