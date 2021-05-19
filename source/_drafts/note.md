front end 40+ passes

Compile time
- part0 (no any pass)
    source -> translation unit (AST)
    	AST
	- Declaration
	- Expression
	- Statement
- part1
    (BinaryIR)LIR (TU) -> optimized LIR
    BasicBlock -> Callgraph

Why reverse callgraph?

Link time
- part2
    LIR virtual registers
    LIR -> TU ???
    why remove dead code not in part0/1
- linker

