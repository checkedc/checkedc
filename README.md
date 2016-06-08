# Checked C
Checked C is an extension of C that adds static and dynamic checking to C to detect or prevent common programming
errors such as buffer overruns, out-of-bounds memory accesses, and incorrect type casts.   This repo contains the
specification for the extension, test code, and samples. For the latest version of the specification and the 
draft of the next version, see the [Checked C releases page](https://github.com/Microsoft/checkedc/releases).

We are creating a modified version of LLVM/clang that supports Checked C.   The code for the modified
version of LLVM/clang lives in the 
[Checked C clang repo](https://github.com/Microsoft/checked-clang)
and the [Checked C LLVM repo](https://github.com/Microsoft/checkedc-llvm).

# Getting involved
See [Contributing](CONTRIBUTING.md) for how to get involved with Checked C and contribute to the project.

# Licensing
The software in this repository is covered by the MIT license.  See the file LICENSE.txt for the license.   The
Checked C specification is made available by Microsoft under the [OpenWeb Foundation Final
Specification Agreement, version 1.0](http://www.openwebfoundation.org/legal/the-owf-1-0-agreements/owfa-1-0).
Contributions of code to the Checked LLVM/clang repos are
subject to the [CLANG/LLVM licensing terms](https://github.com/Microsoft/checkedc-clang/blob/master/LICENSE.txt).


