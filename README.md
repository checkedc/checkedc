# Checked C
Checked C is an extension to C that adds static and dynamic checking to detect or prevent common programming
errors such as buffer overruns, out-of-bounds memory accesses, and incorrect type casts.   This repo contains the
specification for the extension, test code, and samples. For the latest PDF version of the specification and the 
draft of the next PDF version, see the [Checked C releases page](https://github.com/Microsoft/checkedc/releases).

We are creating a modified version of the LLVM/clang compiler that supports
Checked C.  For more information on the compiler, see the
[Checked C clang repo](https://github.com/Microsoft/checkedc-clang).

For developers who want to try out Checked C as it is being implemented, 
an experimental compiler installer is available  on the Checked C clang
[releases page](https://github.com/Microsoft/checkedc-clang/releases).
The compiler is **not ready  for production use**.  Core features of the
language extension are not implemented completely yet, including 
[some](https://github.com/Microsoft/checkedc-clang/wiki/Missing-runtime-and-static-checks)
of the runtime checks and
[important parts](https://github.com/Microsoft/checkedc-clang/wiki/Missing-runtime-and-static-checks)
of the static checking of bounds declarations.  For details on the language
features that are implemented, see the 
[implementation roadmap and status](https://github.com/Microsoft/checkedc-clang/wiki/Implementation-roadmap-and-status).

# Getting involved
You can join the [mailing lists](https://github.com/Microsoft/CheckedC/blob/master/MAILING-LISTS.md) for
announcements and weekly status updates about the project.  There are a variety of ways to get involved
in the project, including opening issues, contributing changes to the specification, or helping out with
the compiler and tools implementation. See [contributing](CONTRIBUTING.md) for more information.

# Licensing
The software in this repository is covered by the MIT license.  See the file LICENSE.TXT for the license.   The
Checked C specification is made available by Microsoft under the [OpenWeb Foundation Final
Specification Agreement, version 1.0](http://www.openwebfoundation.org/legal/the-owf-1-0-agreements/owfa-1-0).
Contributions of code to the Checked LLVM/clang repos are
subject to the [CLANG/LLVM licensing terms](https://github.com/Microsoft/checkedc-clang/blob/master/LICENSE.TXT).

# Code of conduct

This project has adopted the
[Microsoft Open Source Code of Conduct](https://opensource.microsoft.com/codeofconduct/).
For more information see the
[Code of Conduct FAQ](https://opensource.microsoft.com/codeofconduct/faq/) or
contact [opencode@microsoft.com](mailto:opencode@microsoft.com) with any
additional questions or comments.



