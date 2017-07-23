# FyreDir
A tool primarily intended for mass file/dir renaming, having originally been a project of need for my close friend, Mazzeh. In the near-future, it will be able to manage other directory needs too such as file checksumming.

## INSTALL ##
**Microsoft Windows w/ Microsoft Visual Studio '17 CE**

First, you will need to compile the [Boost C++](http://www.boost.org/) libraries. The settings you will need to use include compiling it with a ***versioned*** layout, ***static*** linking, ***shared*** runtime-linking, and with ***mutli-threading*** enabled. It does not matter what address modeling you decide to use, as long as it is consistent throughout the compilation process in everything you do.
Next, you will have to create the solution files with [CMake](https://cmake.org/) using the commandline. In an external directory to the source-files, use either of these commands within the ***Native Tools Command Prompt for VS 2017*** command-prompt:

    cmake ..\FyreDir\src -G "Visual Studio 15 2017 Win64"
    
    cmake ..\FyreDir\src -G "Visual Studio 15 2017"
One is for 64-bit CPU architectures and the other for 32-bit. You will now go into ***FyreDir.sln***, right-click on *fyredir* under Solution Explorer and choose Properties. You will then proceed to Linker -> System and choose as your SubSystem, ***Windows (/SUBSYSTEM:WINDOWS)*** rather than ***Console (/SUBSYSTEM:CONSOLE)*** which WILL cause undefined behaviour. You will lastly direct your attention to Linker -> Command Line and in Additional Options, add the following text:

    /ENTRY:"mainCRTStartup"
You may now compile. Enjoy! ^,==,^
