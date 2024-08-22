# Checked C

Checked C adds static and dynamic checking to C to detect or prevent common programming
errors such as buffer overruns and out-of-bounds memory accesses.
The goal of Checked C is to improve systems programming by making fundamental improvements to C.
This repo contains
sample code, the  [extension specification](https://github.com/checkedc/checkedc/releases),
and test code.

- For a quick overview of Checked C, more information, and pointers to example code,
  see our [Wiki](https://github.com/checkedc/checkedc/wiki).
- The PDF of the specification is available [here](https://github.com/checkedc/checkedc/releases).
- The Checked C fork of LLVM/clang is
  [here](https://github.com/checkedc/checkedc-llvm-project).
- The instructions to build and test the Checked C compiler are documented on
  the [Checked C clang wiki](https://github.com/checkedc/checkedc-llvm-project/wiki).

# History

Checked C is an independent open-source project.   It started as a research project at Microsoft in 2015.
similar to Checked C.  We were looking for a way to improve the security of existing systems software and
eliminate classes of bugs.  One approach is to rewrite the software in a newer language such as Rust.
However, rewriting code is challenging.  It is costly.  There are subtle nuanced differencse in even 
basic language features such as arithmetic across languages.  It also can take a long time before you
have a working system, which does not fit with continuous integration and always having something that
works.  We decided to pursue an incremental approach.

Researchers from many universities and companies have contributed to Checked C.   Researchers at the University of Maryland, the University of Rochester, the University of Washington, Samsung, Rutgers University, and the University of Pennsylvania have contributed to Checked C.   Apple has proposed a C extension similar to Checked C that relies on more dynamic checking.

# Publications and Presentations
- There was a [research paper](https://www.microsoft.com/en-us/research/publication/checkedc-making-c-safe-by-extension/) on
Checked C presented at the [IEEE 2018 Cybersecurity Development Conference](https://secdev.ieee.org/2018/home):
"Checked C: Making C Safe by Extension".   The paper describes the key ideas of Checked C in 8 pages. Note that we have added features to Checked C for improving type safety (and reducing type confusion)
since writing the paper.  The [Wiki](https://github.com/checkedc/checkedc/wiki) and [specification](https://github.com/checkedc/checkedc/releases) provide up-to-date descriptions of Checked C.

- There was another [paper](https://www.microsoft.com/en-us/research/uploads/prod/2019/05/checkedc-post2019.pdf)
on Checked C at the [2019 Principles of Security and Trust Conference](http://www.etaps.org/2019/post):
"Achieving Safety Incrementally With Checked C".
This paper describes a tool for converting existing C code to use Ptr types.  It also proves a blame
property about checked regions that shows that checked regions are blameless for any memory corruption.  This proof is formalized for a core subset of the language extension.

- There was a
[poster](https://github.com/checkedc/checkedc/blob/main/papers/presentations/llvmdevmeet2019-checkedc_for_memory_safety.pdf)
presented at the [LLVM Dev Meeting
2019](https://llvm.org/devmtg/2019-10/talk-abstracts.html#post6): "Overflows Be
Gone: Checked C for Memory Safety". The poster provides an introduction to
Checked C, outlines the compiler implementation and presents an experimental
evaluation of Checked C.

- Therew was a [talk](https://youtu.be/EuxAzvtX9CI)
  ([slides](https://github.com/checkedc/checkedc/blob/main/papers/presentations/llvmdevmeet2020-checkedc_adding_memory_safety_to_llvm.pdf))
at the [2020 LLVM Virtual Dev
Meeting](https://llvm.org/devmtg/2020-09/program): "Checked C: Adding memory
safety support to LLVM". The talk describes the design of bounds annotations
for checked pointers and array pointers as well as the framework for the static
checking of the soundness of bounds. The talk also briefly describes novel algorithms
to automatically widen bounds for null-terminated arrays and for comparison of
expressions for equivalence.

# Build Status

Automated testing is not currently available.

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
