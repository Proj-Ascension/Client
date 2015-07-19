# Contributing

Thanks for your interest in contributing to Project Ascension! We have a few guidelines to make sure that your pull request is accepted and merged.

## Official Coding Style

- Directories (folders), classes (as well as their source and header files), structs, and enumerators should be all formatted in PascalCase.
- Functions, methods, and variables should be formatted in mixedCase.
    - Constant variables should be prepended with `c_`: `c_someValue`
    - Global variables (avoid) should be prepended with `g_`: `g_logCounter`
- Exceptions to the above include standard overrides and conforming to existing C/C++ entities.
- Pointers should be formatted with the `*` operator aligned to the type: `int* ptr`
- We use the Allman indentation style:
```C++
void miracleSort(FancyArray<int> numGroup)
{
    while (numGroup.isUnsorted())
    {
        sleep(10000000000);
    }
}
```
- Include directives for both header and source files should be in the following order, with a line break after each grouping:
    1. Local headers (interface first, if applicable)
    2. Headers from third-party libraries (i.e. Qt, Navajo, etc.)
    3. C/C++ system headers
    4. Conditional / OS-dependent inclusions
- Conditional and iterative statements should include a space, and should always be bracketed:
```C++
while/if (a > 3) 
{
    // Logic
}
```
- Use only spaces, and indent 4 spaces at a time. We use spaces for indentation. Do not use tabs in your code. You should set your editor to emit spaces when you hit the tab key.

### Precautions
- Every header file must have the #pragma once directive as a multiple inclusion guard (header guard).
- Avoid multiple inheritance if at all possible, especially with Qt classes.
- Keep your code well documented, but don't go overboard. Classes and involved functions should be **described** in the interface and **explained** in the implementation.
- Try to keep functions short -- if a given function reaches 100 lines in length, consider splitting it up into smaller pieces.

- For all else, refer to the [Google C++ Style Guide](https://google-styleguide.googlecode.com/svn/trunk/cppguide.html)

## Pull Requests
- Commit and push often - avoid committing huge amounts of code at one time.
- Make sure any upstream changes are merged with your branch *before* sumitting a pull request. Sort out as many conflicts as possible on your end.
- Each pull request will be reviewed by at least two members of the team. Be patient.
    - If your changes are OS-dependent, or you suspect that there may be conflict in that regard, please make that clear.
    - Please don’t open a pull request that adds entirely new features or aesthetic - most of this will be decided by the UI team. PRs should extend or complement existing functionality.
- Official releases will be reviewed by members of all teams, and confirmed to function properly on several different operating systems and desktop environments.    

## Clang Format

To automate this style, some of our team use [clang-format](http://clang.llvm.org/docs/ClangFormat.html) with the below setup:  
```YAML
BasedOnStyle: "Google"  
IndentWidth: 4
UseTab: Never   
BreakBeforeBraces: Allman  
PointerAlignment: Left
ColumnLimit: 0
```
