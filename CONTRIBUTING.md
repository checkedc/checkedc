# Contributing to Checked C

We welcome contributions to the Checked C Project.  Contributions can take many forms depending
on your level of interest:

* Participating in [mailing lists](https://github.com/Microsoft/CheckedC/blob/master/MAILING-LISTS.md) and discussions
  of proposed changes to the specification.
* Helping out with the [Checked C clang compiler implementation](https://github.com/Microsoft/checkedc-clang). We could
 particularly use code reviewers for compiler changes.  There are also a number of self-contained small features that 
 you could implement.
 * Adding test code and samples.
* Making improvements to the specification.  This can include fixing typographical errors or
clarifying wording, adding examples, adding related work, improving the existing discussion of features, or adding new features.
* Proposing significant new functionality.  We have focused on bounds checking so far, and welcome proposals
for checking type casts and memory management.

## Workflow

You can provide feedback on the specification by opening an issue or sending email to the discussion email list.  

We follow the standard [GitHub workflow](https://guides.github.com/introduction/flow/).  You can propose changes
to the specification, tests, examples, or implement compiler features.

 - To submit changes, please create a personal fork of the appropriate repo.  For specification changes,
   examples, and tests, fork the  [Checked C repo](https://github.com/Microsoft/checkedc).   For compiler
   changes, fork the [Checked C clang repo](https://github.com/Microsoft/checkedc-clang).   Less commonly,
   you may need to fork the [Checked C LLVM repo](https://github.com/Microsoft/checkedc-llvm).
-  Make your changes in  your fork and then make a pull request to merge those changes into  the master branch of the appropriate
   repo.
 - For small changes (such as typos and clarifying wording), you can just directly submit a pull request.
 - For more substantial changes or changes where discussion is likely needed, please use the Github issues 
   system for the appropriate repo to track your changes.   For the specification, tests, and examples use
 [Checked C issues](https://github.com/Microsoft/checkedc/issues).
   For compiler implementation issues, use [Checked C clang issues](https://github.com/Microsoft/checkedc-clang) or
   [Checked C LLVM issues](https://github.com/Microsoft/checkedc-llvm). 
 - Please be sure to test your changes before making the pull request. 

## Licensing

You are free to discuss the specification using the email lists or the
[Checked C issues](https://github.com/Microsoft/checkedc/issues) system. These are public forums, so do not
share confidential information.

### Specification
Contributions to the specification are subject to the 
[OpenWeb Foundation Specification agreement](http://www.openwebfoundation.org/legal/the-owf-1-0-agreements/owfa-1-0). 
For minor changes, such as 
improving wording, improving discussion of features, and fixing typos, you do not need to sign anything. 
For more substantial changes (such as proposals for new functionality), we will ask you to sign the
[OpenWeb Contributing License Agreement](http://www.openwebfoundation.org/legal/the-owf-1-0-agreements/owf-contributor-license-agreement-1-0---copyright-and-patent).
This is in the interest of keeping intellectual property rights clear so the community is free
to use the contribution later.

### Code

Contributions of test and sample code to the Checked C repo are subject to the
[licensing terms](https://github.com/Microsoft/CheckedC/blob/master/LICENSE.TXT)
for the repo (MIT license).  Contributions of code to the Checked C LLVM/clang implementation are
subject to the [CLANG/LLVM licensing terms](https://github.com/Microsoft/checkedc-clang/blob/master/LICENSE.TXT).

For minor changes, such as fixing typos, you do not need to sign anything. For other changes, we will ask that you 
sign a [contribution license agreement (CLA)](https://cla.microsoft.com/) before accepting your change. 
If you already have signed a CLA for another Microsoft-related open-source project, it covers the these repos too.
You do not need to sign another CLA.

