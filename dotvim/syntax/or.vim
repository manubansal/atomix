" Vim syntax file
" Language: Open Radio
" Maintainer: Manu Bansal
" Latest Revision: Sept 09, 2011

if exists("b:current_syntax")
  finish
endif

syn keyword orTodo contained TODO FIXME XXX NOTE
syn match orComment "#.*$" contains=orTodo

"----------------------------------------------------------------
" Celestia Star Catalog Numbers
"----------------------------------------------------------------

syn match orSep ":" contained

" Regular int like number with - + or nothing in front
syn match orNumber '[^\h]\d\+' 
syn match orNumber '[^\h][-+]\d\+' 
syn match orNumber '[^\h]\d\+[MmGgKku]' 
syn match orNumber '[^\h][-+]\d\+[MmGgKku]' 
syn match orNumber '[^\h]\d\+ULL' 
syn case ignore
"syn match orNumber  display "[\H]0x\x\+\(u\=l\=\|lu\)"
syn match orNumber  display "[\H]0x\x\+"
syn case match


" Floating point number with decimal no E or e (+,-)
syn match orNumber '[^\h]\d\+\.\d*' 
syn match orNumber '[^\h][-+]\d\+\.\d*' 

" Floating point like number with E and no decimal point (+,-)
syn match orNumber '[^\h][-+]\=\d[[:digit:]]*[eE][\-+]\=\d\+' 
syn match orNumber '[^\h]\d[[:digit:]]*[eE][\-+]\=\d\+' 

" Floating point like number with E and decimal point (+,-)
syn match orNumber '[^\h][-+]\=\d[[:digit:]]*\.\d*[eE[\-+]\=\d\+' 
syn match orNumber '[^\h]\d[[:digit:]]*\.\d*[eE[\-+]\=\d\+' 
syn match orNumber '[^\h][-+]\=\d[[:digit:]]*\.\d*[MmGgKku]' 
syn match orNumber '[^\h]\d[[:digit:]]*\.\d*[MmGgKku]' 

syn region orString start='"' end='"' contained
syn region orDesc start='"' end='"'

syn match orIdentifier '[_a-zA-Z][_a-zA-Z0-9]*'
syn match orOperators "->\|<-\|<\->|<|>\|==\|<=\|>=\|:=:\|||\|&&\|!=\|<<\|>>"
syn match orOperators "+\|-\|*\|/"
"manu

syn keyword langKeywords atom fifo wire action state statep statei
syn keyword hintOrSanityKeywords conf typehint null noaxn
syn keyword hintOrSanityKeywords sin

let b:current_syntax = "or"

hi def link orTodo        Todo
hi def link orComment     Comment
"hi def link orBlockCmd    Statement

"manu
hi def link langKeywords  Statement
hi def link langTypes     Type
hi def link orIdentifier  Identifier
hi def link orField 	  Function

hi def link orString      Constant
hi def link orDesc        PreProc
hi def link orNumber      Constant
hi def link orBraces	  Delimiter
hi def link orOperators   Operator
hi def link orSep	  Delimiter
hi def link hintOrSanityKeywords PreProc
hi def link deprecatedKeywords Todo
