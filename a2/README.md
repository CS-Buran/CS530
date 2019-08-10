Filename: README.md

CS 530, Spring 2019

Assignment #2: SIC/XE Disassembler

Brendan Le Tourneau: cssc0414 / 819897930

Connor Rowland - Turn In Account: cssc0403 / 820436793

Rahul Sharma: cssc0417 / 820943039


### FILE MANIFEST
xed.cpp

makefile

xed.h

sample.obj

sample.sym

sample2.obj

sample2.sym

sample3.obj

sample3.sym

README.md

SoftwareDesignDoc.md

### COMPILE INSTRUCTIONS
Compile using the make command

### OPERATING INSTRUCTIONS
Use the xed executable with argument name of the obj and sym file.

EXAMPLE: To disassemble secretCode.obj and secretCode.sym

./xed secretCode

### DESIGN DECISIONS
See software design doc

### Deficiencies
Our disassembler does not track changes to any registers during program execution. If the b register for instance is multiplied, then if a base addressing instruction appears, the associated target address will not be calculated correctly.

Also, if a literal has the same hexadecimal representation as an instruction, the two cannot be distinguished, and is treated like a literal

### Lessons Learned
Planning is an important stage of software development. With larger projects it is impossible to understand the entirety of the program by simply looking at the code and reasoning about its behavior. If we had to do it again, we would have gone for a more modular design by first creating simple building blocks and abstracting from there.
