PAPERNAME=checkedc
FILES = abstract.tex \
        changelog.tex \
        checking-variable-bounds.tex \
        core-extensions.tex \
        design-alternatives.tex \
        fragments.tex \
        interoperation.tex \
        introduction.tex \
        lessons.tex \
        pointers-to-pointers.tex \
        related-work.tex \
        roadmap.tex \
        simple-invariants.tex \
        span-compilation.tex \
        structure-bounds.tex \
        variable-bounds.tex \
        void-ptr-replacements.tex \
        sources.bib \
        checkedc.tex

all: $(PAPERNAME).pdf

$(PAPERNAME).pdf: $(FILES) makefile
	pdflatex $(PAPERNAME)
	bibtex --min-crossrefs=300 $(PAPERNAME)
	pdflatex $(PAPERNAME)
	pdflatex $(PAPERNAME)

clean:
	rm -f $(PAPERNAME).pdf *.dvi *.aux *.log *.blg *.bbl *~
