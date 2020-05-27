# Checked C
Checked C adds static and dynamic checking to C to detect or prevent common programming
errors such as buffer overruns and out-of-bounds memory accesses. 
The goal of the project is to improve systems programming by making fundamental improvements to C.
This repo contains
sample code, the [extension specification](https://github.com/Microsoft/checkedc/releases),
and test code.

- For a quick overview of Checked C, more information, and pointers to example code,
  see our [Wiki](https://github.com/Microsoft/checkedc/wiki).
- The PDF of the specification is available [here](https://github.com/Microsoft/checkedc/releases).
- Compilers are available [here](https://github.com/Microsoft/checkedc-clang/releases).

# Papers
We presented a [research paper](https://www.microsoft.com/en-us/research/publication/checkedc-making-c-safe-by-extension/) on
Checked C at the [IEEE 2018 Cybersecurity Development Conference](https://secdev.ieee.org/2018/home):
"Checked C: Making C Safe by Extension".   The paper describes the key ideas of Checked C in 8 pages.

Note that we have added features to Checked C for improving type safety (and reducing type confusion)
since writing the paper.  The [Wiki](https://github.com/Microsoft/checkedc/wiki) and [specification](https://github.com/Microsoft/checkedc/releases) provide up-to-date descriptions of Checked C.

We presented another [paper](https://www.microsoft.com/en-us/research/uploads/prod/2019/05/checkedc-post2019.pdf)
on Checked C at the [2019 Principles of Security and Trust Conference](http://www.etaps.org/2019/post): 
"Achieving Safety Incrementally With Checked C".
This paper describes a tool for converting existing C code to use Ptr types.  It also proves a blame
property about checked regions that shows that checked regions are blameless for any memory corruption.  
This proof is formalized for a core subset of the language extension.

# Build Status

|Configuration|Testing|Status|
|--------|---------------|-------|
|Debug X86 Windows| Checked C and clang regression tests|![Debug X86 Windows status](https://msresearch.visualstudio.com/_apis/public/build/definitions/f6454e27-a46c-49d9-8453-29d89d53d2f9/211/badge)|
|Debug X64 Windows| Checked C and clang regression tests| ![Debug X64 Windows status](https://msresearch.visualstudio.com/_apis/public/build/definitions/f6454e27-a46c-49d9-8453-29d89d53d2f9/205/badge)|
|Debug X64 Linux  | Checked C and clang regression tests| ![Debug X64 Linux status](https://msresearch.visualstudio.com/_apis/public/build/definitions/f6454e27-a46c-49d9-8453-29d89d53d2f9/217/badge)|
|Release X64 Linux| Checked C, clang, and LLVM nightly tests|![Release X64 Linux status](https://msresearch.visualstudio.com/_apis/public/build/definitions/f6454e27-a46c-49d9-8453-29d89d53d2f9/238/badge)|

# Participating
We're happy to have the help! You can contribute by trying out Checked C, 
reporting bugs, and giving us feedback. There are other ways to [contribute](CONTRIBUTING.md) too.
You can join the [mailing lists](https://github.com/Microsoft/CheckedC/blob/master/MAILING-LISTS.md) for
announcements about the project.

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
