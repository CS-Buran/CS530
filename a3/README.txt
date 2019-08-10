Connor Rowland, CSSC0403, 820436793
CS 530, Spring 2019
Lab #3, Simple Parser
FILENAME: README.md

File Manifest:
README.md
ex.txt
ex2.txt
ex3.txt
exp.l
exp.y
makefile

Compile Instructions:
Run make exp to create executable

Operating Instructions:
With test input in ex.txt, run ./exp to run the parser on test file

Design Decisions:
I used flex and bison as my tools because it is less code for me to write which
helps me make less errors. I decided to print the valid tokens in my bison file because
I wanted the user to know where bison ran into the error during the line.

Extra features:
I added a line counter for the user to know which line in the file the error occurred.

Bugs:
If there is a missing semi colon, the error's line number is off by 1 line.
The parser believes the error is on the next line. I didn't try to fix this
because I have compiled C code where the error is indicated on a line after
the error is in the source code. I think its a natural behavior in all parsers.

Lessons Learned:
Flex and Bison are great tools! They really help simplify the grammar rules
and help the programmer write a lot less code. I learned it's important to
start small with these projects and just develop one rule at a time and build
up the project slowly instead of trying to implement everything at once.

Grammar:
<lines>      ::= <line> | <lines><line>
<line>       ::= <EOL> | <END> | <assign><EOL> | <exp><EOL>
<assign>     ::= <id><equals><exp><semi>
<exp>        ::= <id> | <id><op><exp> | <openParen><id><op><id><closeParen>
<op>         ::= +|-|*|/|%
<id>         ::= <char> | <id><char> | <id><digit>
<char>       ::= a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|
                 y|z|A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|
                 W|X|Y|Z
<digit>      ::= 0|1|2|3|4|5|6|7|8|9
<semi>       ::= ;
<equals>     ::= =
<openParen>  ::= (
<closeParen> ::= )
<EOL>        ::= '\n'
<END>        ::= <<EOF>>
