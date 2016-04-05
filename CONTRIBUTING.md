# Contributing to Checked C

We welcome contributions to the Checked C Project!   Contributions can take many forms, depending
on your level of interest and available time:

* Participating in [mailing lists](https://github.com/Microsoft/CheckedC/blob/master/MAILING-LISTS.md) and discussions
  of proposed changes to the specification.
* Making improvements to the specification.  This can include adding new features, improving the existing discussion
of features, adding exaples, clarifying wording, and fixing errors.
* Proposing significant new functionality.  We have focused on bounds checking so far, and welcome proposals
for checking type casts and memory management.
* Adding test code and samples.
* Implementing Checked C in existing C compilers.

## Workflow

You can provide feedback on the specification by opening an issue or sending email to the discussion email list.  

We follow the standard [GitHub workflow](https://guides.github.com/introduction/flow/).  You can propose changes
to the specification or new examples or tests using that work flow.   
 - To submit changes, please create a personal fork of the CheckedC repository, push
   your changes there, and issue a pull request to merge those changes into
   the master branch of the main repository.
 - For small changes (such as typos and clarifying wording), you can just directly submit a pull request.
 - For more substantial changes, new files, or changes where discussion is likely needed, please use the [Github issues 
   system](https://github.com/Microsoft/CheckedC/issues) to track your changes.
 - Please be sure that the LaTex document still typesets.
 
Implementations of Checked C in existing compilers will be done in other repos.
Those compilers will typically have their own licensing terms.  We also want the 
language design and language validation tests to be independent of any specific compiler 
implementation.  Implementations will have their work flow.

## Licensing

You are of course free to discuss the specification using the email lists or the [issues system]
(https://github.com/Microsoft/CheckedC/issues).
Please do not send or disclose any confidential information.   These are public forums and the
information will no longer be confidential if you do that.

### Specification
Contributions to the specification are subject to the [OpenWeb Foundation Specification agreement]
(http://www.openwebfoundation.org/legal/the-owf-1-0-agreements/owfa-1-0).  For minor changes, such as 
improving wording, improving discussion of features, and fixing typos, you do not need to sign anything. 
For more substantial changes (such as proposals for new functionality), we will ask you to sign the
[OpenWeb Contributing License Agreement]
(http://www.openwebfoundation.org/legal/the-owf-1-0-agreements/owf-contributor-license-agreement-1-0---copyright-and-patent).
This is in the interest of keeping intellectual property rights clear so the community is free
to use the contribution later.

### Code

Contributions of code will be subject to the [licensing terms](https://github.com/Microsoft/CheckedC/blob/master/LICENSE.txt)
for the repo (MIT license). In addition, we will ask that you sign a [contribution license agreement (CLA)]
(https://cla.microsoft.com/) before accepting your change.  If you've already signed a CLA for another
Microsoft-related open-source project on Github, it should cover this project too, so you don't need to sign
the agreement again.
