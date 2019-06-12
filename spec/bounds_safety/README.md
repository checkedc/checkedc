# Summary

The Checked C specification is written using [LaTeX](https://latex-project.org/intro.html).
LaTeX is used for preparing technical documents.  LaTeX uses text-based files that work
nicely with source-code control systems such as Git.  The text-based files describe the
content and structure of the document and some aspects of the appearance.  LaTex processes
the files to type set the document.

# Type setting instructions

You will need to have a version of LaTex installed on your computer.  See the next
section for information on how to obtain LaTex.  To type set the specification, open a shell
windows and run the following commands:

pdflatex checkedc

bibtex checkedc

pdflatex checkedc

pdflatex checkedc

The multiple runs of pdflatex are needed so that cross-references converge.   The
resulting file will be called checkedc.pdf.

# More information on LaTex.

## Obtaining LaTex 
There are many widely
used free distributions of LaTex available.   One version is [MikTex](http://www.miktex.org),
which is widely used on Windows.  Another version is [TexLive](http://tug.org/texlive/),
which is often used on Linux system.

## Editing LaTex files

Both distributions of LaTex install a nice editor for working with LaTex files called TexWorks.    It has a feature where it will automatically run latex for you from the editor.    Note that when using Texworks with MikTex, you should be sure to run pdflatex from the command-line before using the autorun feature so that the right packages are installed.

## Learning about LaTex

For a very brief introduction to LaTex, read [A document for absolute LaTex beginners](http://ctan.org/pkg/first-latex-doc).   A good starting point for learning about LaTex is a [A (Not So) Short Introduction to Latex2E] (http://ctan.org/pkg/lshort-english).

