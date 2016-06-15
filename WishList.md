# A Wish List of Checked C Projects.

There is plenty of work to do on Checked C. Here is a list of projects 
that could be begun now or in the next few months.  The projects cover compiler
implementation, tools, language design, and experimental evaluation.  The 
projects range in nature from implementation to design to open research
problems.  If you decide to work on a project, it would be helpful to let us
know so that we can try to coordinate work among interested parties.

# Compiler implementation

Here are projects for the LLVM/clang implementation of Checked C.  Some 
projects could be begun now.  Others will need to wait until bounds declarations
are added to the clang AST.

- Nightly build automation and build testing.
- Pointer arithmetic for checked pointers types that has overflow checking.
- Non-null checks for checked pointer types.
- Span pointer types: parsing, representation in IR, typechecking, and
  implementation of operations.
- Insert runtime bounds checks during the lowering to LLVM IR.
- Enforce the rule in Section 3.2 that all uses of externally-scoped variables
  within a compilation unit have the same view of the bounds for the variable.
-  Bounds-safe interfaces for functions and static variables:
-- Parsing of interoperation annotations for unchecked pointer types.
-- Insertion of implicit type coercions between checked and unchecked pointer
   types.
-- Compatibility between declarations with and without bounds information.
-- Checks that there are no intervening definitions.
-  Checked/unchecked blocks.
- Bounds declarations for structure types and structure member operations.

Another interesting direction is to begin implementing Checked C in other
compilers such as GCC.

# Tools

- Create tools that modify existing C code to use features in Checked C. One possible project 
  is to build a tool that identifies pointers that are never used with pointer arithmetic
  and convert them to _ptr_.  We have implemented typechecking for the _ptr_ type in LLVM/clang,
  so the typechecking rules for _ptr_ would be enforced.

# Experimental evaluation

- Try applying Checked C to additional code bases.  We are experimenting with OpenSSL.
  It would be useful to gather experiences with other code bases, which will have
  different coding styles and address other problem domains.  

# Language design

- Extend the Checked C design to describe null-terminated array types.
- Extend the C language definition notion of compatibility to declarations
  with and without bounds information. This would allow bounds-safe
  interfaces to be declared for C standard library functions, without 
  modifying the source code for existing headers.
- Add a way to describe that aliasing (or anti-aliasing) properties of data
  structures.  This would allow bounds invariants to be suspended for certain
  data during function calls.
- Ways to check the correct use of the following memory management strategies
  used in C programs:
-- Stack allocation.
-- Reference counting
-- Regions.
- A design for checking the correctness of explicit memory management
strategies.






