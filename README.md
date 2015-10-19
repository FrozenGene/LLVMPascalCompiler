LLVMPascalCompiler
==================

This is the source code of My LLVM Pascal Compiler Tutorial. You can click [this] [1] to visit my blog and this compiler tutorial.

This project will implement almost all ISO Pascal 90 syntax and can work on Windows, Linux and Mac OS X.

Current Status:
==================
Scanner.  100%.    

Parser.   Working...

Semantic. Working...



Compile and Run
==================

Windows:
==

Microsoft Visual Studio 2013.

Click LLVMPascal.sln and Build. You can debug and run in the environment of MSVS 2013 or You can run LLVMPascal.exe in cmd environment. But if you run it in cmd, you should copy scanner_test.pas in the fold of LLVMPascal.exe. Do not be sad, after the completion of this tutorial, we will make this easier.

Linux and Mac OS X:
==

I will change the project build way to [CMake][2] later, But now I will use Visual Studio :-).


License
=================

[BSD][3]

[1]:http://frozengene.github.io/
[2]:https://cmake.org/
[3]:http://opensource.org/licenses/BSD-2-Clause
