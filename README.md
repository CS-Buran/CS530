# CS530

## A1: Hexadecimal File Dump
Opens a regular type file (containing binary or text/ASCII content), reads every
byte in the file and writes both the ASCII hex value for that byte as well
as its printable (human-readable) character to standard output.
For bytes forming non-printable characters, a . character is then printed in its
place.

## A2: SIC/XE Disassembler
SIC/XE is a simple computer architecture and related assembly language used
in learning systems level computer design.

SIC/XE disassembler program opens an XE object file, <filename>.obj and
its accompanying symbol file, <filename>.sym, then it disassembles the
object code, and generates an XE source file, <filename.sic> and XE listing file,
<filename>.lis using the disassembled code. The symbol file, <filename>.sym
will contain the SYMTAB and LITTAB the assembler generated when
assembling the object file.

## A3: Flex & Bison Parser
A parser written with Flex & Bison which recognizes valid statements as described below.

Read a file or from standard input, scan the input, and determine if the statement(s)
is/are valid. Print out the statement, and a pass/fail. If it failed, the reason why is
printed.

Note: id:== identifier, exp :== expression, op :== operator, char :== character

Recognize the following as valid statements: assignments and expression

Assignments have the form: id = exp;
Expressions have the form: id op id {op id}
--any length as long as pairs of op and id are added

A parenthesis pair may be used to group any id op id combination.
Therefore: id op (id op id) op id AND id op id op (id op id) are valid expressions.

Note - each id, op, =, and ; will have a space as it's precedent and antecedent.
The open parenthesis will have a space as it's precedent but it may or may not
have a space immediately following it. The converse is true for the closing
parenthesis (guaranteed a space as it's antecedent but not necessarily preceding it).

An id is any combination of digits and char.
The first position of the identifier must contain a char

A digit is one of: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9

A char is one of:a, b, c, d, e, f, g, h, i, j, k, l m, n, o, p, q, r, s, t, u, v,
w, x, y, z, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z

An op is one of: +, -, *, /, %
