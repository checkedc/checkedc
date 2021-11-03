# Checked C

This is the new home of the Checked C project. The Checked C extension was originally developed at Microsoft from 2016-2021. Microsoft chose to stop funding further development of Checked C. We greatly appreciate the support of Microsoft for the project for many years! David Tarditi, the founder of the Checked C project, has asked that development continue here.

Checked C adds static and dynamic checking to C to detect or prevent common programming
errors such as buffer overruns and out-of-bounds memory accesses. 
The goal of the project is to improve systems programming by making fundamental improvements to C.
This repo contains
sample code, the [extension specification](https://github.com/Secure-Software-Development-Project/checkedc/releases),
and test code.

- For a quick overview of Checked C, more information, and pointers to example code,
  see our [Wiki](https://github.com/Microsoft/checkedc/wiki).
- The PDF of the specification is available [here](https://github.com/Secure-Software-Development-Project/checkedc/releases).
- Compilers are available [here](https://github.com/Secure-Software-Development-Project/checkedc-clang/releases).
- The Checked C clang repo is
  [here](https://github.com/Microsoft/checkedc-clang).
- The instructions to build and test the Checked C compiler are documented on
  the [Checked C clang wiki](https://github.com/Secure-Software-Development-Project/checkedc-clang/wiki).

# Publications and Presentations
- We presented a [research paper](https://www.microsoft.com/en-us/research/publication/checkedc-making-c-safe-by-extension/) on
Checked C at the [IEEE 2018 Cybersecurity Development Conference](https://secdev.ieee.org/2018/home):
"Checked C: Making C Safe by Extension".   The paper describes the key ideas of Checked C in 8 pages. Note that we have added features to Checked C for improving type safety (and reducing type confusion)
since writing the paper.  The [Wiki](https://github.com/Microsoft/checkedc/wiki) and [specification](https://github.com/Secure-Software-Development-Project/checkedc/releases) provide up-to-date descriptions of Checked C.

- We presented another [paper](https://www.microsoft.com/en-us/research/uploads/prod/2019/05/checkedc-post2019.pdf)
on Checked C at the [2019 Principles of Security and Trust Conference](http://www.etaps.org/2019/post): 
"Achieving Safety Incrementally With Checked C".
This paper describes a tool for converting existing C code to use Ptr types.  It also proves a blame
property about checked regions that shows that checked regions are blameless for any memory corruption.  This proof is formalized for a core subset of the language extension.

- We presented a
[poster](https://github.com/microsoft/checkedc/blob/master/papers/presentations/llvmdevmeet2019-checkedc_for_memory_safety.pdf)
at the [LLVM Dev Meeting
2019](https://llvm.org/devmtg/2019-10/talk-abstracts.html#post6): "Overflows Be
Gone: Checked C for Memory Safety". The poster provides an introduction to
Checked C, outlines the compiler implementation and presents an experimental
evaluation of Checked C.

- We presented a [talk](https://youtu.be/EuxAzvtX9CI)
  ([slides](https://github.com/microsoft/checkedc/blob/master/papers/presentations/llvmdevmeet2020-checkedc_adding_memory_safety_to_llvm.pdf))
at the [2020 LLVM Virtual Dev
Meeting](https://llvm.org/devmtg/2020-09/program): "Checked C: Adding memory
safety support to LLVM". The talk describes the design of bounds annotations
for checked pointers and array pointers as well as the framework for the static
checking of the soundness of bounds. We also briefly describe novel algorithms
to automatically widen bounds for null-terminated arrays and for comparison of
expressions for equivalence.

# Build Status

*Continuous integration buuilds are not currently available*

# Participating
Weare happy to have the help. You can contribute by trying out Checked C, 
reporting bugs, and giving us feedback. There are other ways to [contribute](CONTRIBUTING.md) too.
You can watch the [announcement page](https://github.com/Secure-Software-Development-Project/checkedc/discussions/categories/announcements)
for announcements about the project.

# Licensing
The software in this repository is covered by the MIT license.  See the file LICENSE.TXT for the license.   The
Checked C specification follows the  [OpenWeb Foundation Final
Specification Agreement, version 1.0](http://www.openwebfoundation.org/legal/the-owf-1-0-agreements/owfa-1-0).
Contributions of code to the Checked LLVM/clang repos are
subject to the [CLANG/LLVM licensing terms](https://github.com/Microsoft/checkedc-clang/blob/master/LICENSE.TXT).

# Code of conduct

This project has adopted a [Code of Conduct](CODE_OF_CONDUCT.md).
