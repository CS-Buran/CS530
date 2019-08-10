#### Filename: README.md

#### CS 530, Spring 2019

#### Assignment #2: SIC/XE Disassembler

#### Brendan Le Tourneau: cssc0414 / 819897930

#### Connor Rowland - Turn In Account: cssc0403 / 820436793

#### Rahul Sharma: cssc0417 / 820943039


### Design Decisions
We first check if the argument given is valid, and if it is not we specify the requirements to run the command.

If the argument is valid, we save the contents of both files into strings. From the symbol file string, we parse all of the symbols along with their values and flags. We also parse the literal table to obtain the literals along with their lengths and addresses.

We then generate a map of all the SIC/XE instructions, each mapped to their object code. We manually append the format type of each instruction to the end of its instruction name, to obtain for later use.

The majority of our program involves parsing the object file and printing the disassembled code to the created sic and lis files. We take the object file in one line at a time, which corresponds to one record. We check the record type, and if it is an H record we print the START assembler directive at the address given by the H record.

If the current address we are at is the same as an address in the symbol table, we put the symbol down at that address.

For T records, we start at the first opcode, after the starting address and length. We check the opcode to obtain the format of the instruction. If it is format 1, we take the next byte and save it as the object code of the instruction. For format 2, we take the next 2 bytes. For format 3 and 4, we check the status of the e bit, and if it is 0, we take the next 3 bytes, and if 1, the next 4 bytes.

Now that we have the object code for the current instruction, we work backwards to add anything needed for generating the instruction. For example, if we found the e bit set we include a + before our instruction name. We do all necessary checks to insure the correct modifiers are printed with each instruction.

We then find the operands for each instruction. By checking which bits are set, we can work backwards to find the target address of each line. We then check our symbol table and compare the target address to the addresses. If we find one, we use that symbol as the operand for the instruction. We also check the necessary bits to modify the operand as well, such as adding a ",X" if the x bit is set.

Then we print the object code to the listing file, and move onto the next instruction in the T record.

We also do some checks to see if we need to include the BASE or LTORG assembler directives at anylines, along with accounting for the position of the literals in the object code.

After each record is processed, we print the assembler directives for all the of the reserved and declared memory, and finally print the END assembler directive.


### Pseudo Code:

    Check for input errors
        Display message if incorrect and break
    Open file
    Initialize data to read the symbol file
    Read symbol file and store
    Read the object file and populate output files with code
    Recheck symbol file addresses
        If a symbol address hasn't been met then we will write RESW or RESB

### Program Flow:
File Input -> Symbol Parsing -> Map Generation -> Reading object file -> Initializing data
