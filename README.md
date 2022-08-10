# EtsePy
Simplified lexical parser for Python made for the first exercise of the subject Compilers and Interpreters taught at the University of Santiago de Compostela.

------------

**Table of Contents**

[TOC]

#Introduction
This is the first exercise of the subject Compiler and Interpreters  taught at the University of Santiago de Compostela.

It consists in a lexical parser for the programming language Python, but it is simplified compared to the lexical analysis specification provided in the [Python documentation](https://docs.python.org/3/reference/lexical_analysis.html "Python documentation").

As it implements a subset of the functionality of the Python lexical parser, it will be demonstrated in a dummy program (*wilcoxon.py*) from which all lexical elements will be extracted.

#Lexical components
The parser will identify the following lexical components:
- [String literals](https://docs.python.org/3/reference/lexical_analysis.html#string-and-bytes-literals "String literals")
- [Integer literals](https://docs.python.org/3/reference/lexical_analysis.html#integer-literals "Integer literals")
- [Floating point literals](https://docs.python.org/3/reference/lexical_analysis.html#floating-point-literals "Floating point literals")
- [Identifiers and keywords](https://docs.python.org/3/reference/lexical_analysis.html#identifiers "Identifiers and keywords")
- [Operators](https://docs.python.org/3/reference/lexical_analysis.html#operatorshttp:// "Operators")
- [Delimiters](https://docs.python.org/3/reference/lexical_analysis.html#delimiters "Delimiters")
- [Changes in indentation and lines](https://docs.python.org/3/reference/lexical_analysis.html#line-structure "Changes in indentation and lines")

#Execution instructions
The parser was built and tested in *Ubuntu 20.04.2 LTS*. For demonstration, the following instructions should be followed:
- Open a terminal and navigate to the directory with the code.
- Input the following comands:

```shell
make
./compiler wilcoxon.py
```
- *wilcoxon.py* can be replaced by any Python program to be parsed.

#Sample output
Following, we show the first ten lines that are printed to console when parsing the program *wilcoxon.py*. Between each pair <> we print each lexical element in the code, i.e., the lexeme and its associated lexical component.

    < 401, """
    Created on Tue Sep 15 12:16:21 2015
    """ >
    < 407, NEWLINE >
    < 423, import >
    < 404, scipy >
    < 46, . >
    < 404, stats >
    < 440, as >
    < 404, st >

#Future improvements
Following, we list some possible improvements for our lexical parser:
1. Show where the erros come from to ease the debugging.
2. Extend the number automatas to handle all the number formats.

#References
**[1]**  [ Mark Grosberg. Implementing Efficient State Machines.](http://www.conman.org/projects/essays/states.html " Mark Grosberg. Implementing Efficient State Machines. ")
**[2]** Aho, A. V., Sethi, R., & Ullman, J. D. (1985). Compilers: Principles, Techniques, and Tools.
