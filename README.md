# Checked C
Checked C adds static and dynamic checking to C to detect or prevent common programming
errors such as buffer overruns and out-of-bounds memory accesses. 
The goal of the project is to improve systems programming by making fundamental improvements to C.
This repo contains
sample code, the [extension specification](https://github.com/Microsoft/checkedc/releases),
and test code.

- For a quick overview of Checked C and more information, see our [Wiki](https://github.com/Microsoft/checkedc/wiki).
- The PDF of the specification is available [here](https://github.com/Microsoft/checkedc/releases).
- Compilers are available [here](https://github.com/Microsoft/checkedc-clang/releases).

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
