# Checked C

Checked C extends C with bounds checking and improved type safety.  It helps programmers retrofit existing C code to
be more secure. This repo contains
the [Checked C specification](https://github.com/checkedc/checkedc/releases),
sample code, and test code.

- For a quick overview of Checked C and pointers to sample code,
  see our [Wiki](https://github.com/checkedc/checkedc/wiki).
- You can download Checked C clang compiler releases for Windows, Mac, and Ubuntu
  [here](https://github.com/checkedc/checkedc-llvm-project/releases).
- The specification is available [here](https://github.com/checkedc/checkedc/releases).
- The repo for the Checked C clang compiler is
  [here](https://github.com/checkedc/checkedc-llvm-project).  The compiler is a fork of
  LLVM/clang. Instructions for building the compiler from source code are on the
  [Checked C clang wiki](https://github.com/checkedc/checkedc-llvm-project/wiki).

# Build Status

[![Checked C Clang CI [Linux]](https://github.com/checkedc/workflows/actions/workflows/check-clang-linux.yml/badge.svg)](https://github.com/checkedc/workflows/actions/workflows/check-clang-linux.yml)

[![Checked C Clang CI [MacOS]](https://github.com/checkedc/workflows/actions/workflows/checkedc-clang-macos.yml/badge.svg)](https://github.com/checkedc/workflows/actions/workflows/checkedc-clang-macos.yml)

[![Checked C Clang CI [WINDOWS]](https://github.com/checkedc/workflows/actions/workflows/check-clang-windows.yml/badge.svg)](https://github.com/checkedc/workflows/actions/workflows/check-clang-windows.yml)

# History

Checked C is an independent open-source project.   It started as a research project at Microsoft in 2015.
similar to Checked C.  We were looking for a way to improve the security of existing systems software and
eliminate classes of bugs.

One approach is to rewrite the software in a newer language such as Rust.
However, rewriting code is challenging for a number of reasons:  it is costly, there are subtle differences in even
basic language features such as arithmetic across languages, and it can take a long time before you
have a working system.  Combined, this makes a rewrite a high-risk software development project.  These kinds
of rewrites are unlikely to be done just to improve security.  We decided to pursue an incremental approach that allows
existing C code to be improved gradually and at much lower cost.

Researchers from many universities and companies have contributed to Checked C.
Researchers at the University of Maryland, the University of Rochester, the University of Washington, Samsung,
Rutgers University, and the University of Pennsylvania have contributed to Checked C.
Apple has proposed a C extension similar to Checked C that relies on more dynamic checking.


# Publications and Presentations

- [Fat Pointers For Temporal Memory Safety of C](https://dl.acm.org/doi/pdf/10.1145/3586038) by Jie Zhou,
  John Criswell, and Michael Hicks. This appeared in
  [OOPSLA 2023](https://2023.splashcon.org/details/splash-2023-oopsla/12/Fat-Pointers-for-Temporal-Memory-Safety-of-C).
  It describes an extension to Checked C that adds new pointers that provide temporal memory safety.

- [C to Checked C by 3C](https://dl.acm.org/doi/10.1145/3527322), by Aravind Machiry, John Kastner, Matt McCutchen, Aaron Eline,
  Kyle Headley, and Michael Hicks. This paper describes the semi-automated 3C tool for converting C to Checked C.
  It [won](https://2022.splashcon.org/details/splash-2022-oopsla/13/C-to-checked-C-by-3c) a SIGPLAN Distinguished Paper award
  at OOPSLA 2022.

- [A Formal Model of Checked C](https://ieeexplore.ieee.org/document/9919657), by Liyi Li, Deena Postol, Leonida
  Lampropoulos, David Van Horn, and Michael Hicks. This was published in the 2022 IEEE 35th Computer Security Foundations
  Symposlium. It describe a formal model of Checked C.  The model was formalized using the Coq theorem prover.

- [Achieving Safety Incrementally With Checked C](https://www.microsoft.com/en-us/research/uploads/prod/2019/05/checkedc-post2019.pdf).
  This was presented at the [2019 Principles of Security and Trust Conference](http://www.etaps.org/2019/post):.
  This paper describes an early version of 3C that convert existing C code to use Ptr types.  It also proves a blame
   property about checked regions that shows that checked regions are blameless for any memory corruption.
  This proof is formalized for a core subset of the language extension.

- [Checked C: Making C Safe by Extension](https://www.microsoft.com/en-us/research/publication/checkedc-making-c-safe-by-extension/) by
  David Tarditi, Samuel Elliott, Andrew Ruef, and Michael Hicks.  This appeared in the
  [IEEE 2018 Cybersecurity Development Conference](https://secdev.ieee.org/2018/home). It describes the key ideas of Checked C bounds
   checking in 8 pages.  We have added features to Checked C since then.
   The [Wiki](https://github.com/checkedc/checkedc/wiki) and [specification](https://github.com/checkedc/checkedc/releases)
   provide up-to-date descriptions of Checked C.


- There was a
[poster](https://github.com/checkedc/checkedc/blob/main/papers/presentations/llvmdevmeet2019-checkedc_for_memory_safety.pdf)
presented at the [LLVM Dev Meeting
2019](https://llvm.org/devmtg/2019-10/talk-abstracts.html#post6): "Overflows Be
Gone: Checked C for Memory Safety". The poster provides an introduction to
Checked C, outlines the compiler implementation and presents an experimental
evaluation of Checked C.

- There was a [talk](https://youtu.be/EuxAzvtX9CI)
  ([slides](https://github.com/checkedc/checkedc/blob/main/papers/presentations/llvmdevmeet2020-checkedc_adding_memory_safety_to_llvm.pdf))
at the [2020 LLVM Virtual Dev
Meeting](https://llvm.org/devmtg/2020-09/program): "Checked C: Adding memory
safety support to LLVM". The talk describes the design of bounds annotations
for checked pointers and array pointers as well as the framework for the static
checking of the soundness of bounds. The talk also briefly describes novel algorithms
to automatically widen bounds for null-terminated arrays and for comparison of
expressions for equivalence.


# Participating
We are happy to have the help. You can contribute by trying out Checked C,
reporting bugs, and giving us feedback. There are other ways to [contribute](CONTRIBUTING.md) too.

# Licensing
The software in this repository is covered by the MIT license.  See the file LICENSE.TXT for the license.  The
Checked C specification is made available by Microsoft under the [OpenWeb Foundation Final
Specification Agreement, version 1.0](http://www.openwebfoundation.org/legal/the-owf-1-0-agreements/owfa-1-0).
Contributions of code to the Checked LLVM/clang repos are
subject to the [LLVM/clang licensing terms](https://github.com/checkedc/checkedc-llvm-project/blob/main/clang/LICENSE.TXT).

# Code of conduct

This project has adopted a [Code of Conduct](CODE_OF_CONDUCT.md).
