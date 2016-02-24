PAPERNAME=checkedc
FILES = abstract.tex \
        aliased-structures.tex \
        array-view-compilation.tex \
        checking-structure-bounds.tex \
        checking-variable-bounds.tex \
        core-extensions.tex \
        design-alternatives.tex \
        fragments.tex \
        interoperation.tex \
        introduction.tex \
        lessons.tex \
        open-issues.tex \
        pointers-to-pointers.tex \
        simple-invariants.tex \
        structure-bounds.tex \
        variable-bounds.tex \
        sources.bib \
	checkedc.tex

all: $(PAPERNAME).pdf

$(PAPERNAME).pdf: $(FILES) makefile
	pdflatex $(PAPERNAME)
	bibtex --min-crossrefs=100 $(PAPERNAME)
	pdflatex $(PAPERNAME)
	pdflatex $(PAPERNAME)

clean:
	rm -f $(PAPERNAME).pdf *.dvi *.aux *.log *.blg *.bbl *~
