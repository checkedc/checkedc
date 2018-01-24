# This Makefile uses latexmk as far as possible
# because it understands LaTeX files better than
# a hand-coded makefile. latexmk should be in any
# modern Tex Live distribution.
# Info: http://mg.readthedocs.io/latexmk.html

.PHONY: tr02.pdf all clean cleanall pres

all : tr02.pdf

pres : $(addprefix scripts/,modifications_pres.pdf overheads_pres.pdf)

clean :
	latexmk -c

cleanall :
	latexmk -C
	$(MAKE) -C scripts clean

GEN_TEX_SCRIPTS=$(addprefix scripts/,bm_results_tab.tex bm_results_macros.tex)
GEN_GRAPHS=$(addprefix scripts/,modifications.pdf overheads.pdf)
GEN_RESULTS=$(GEN_TEX_SCRIPTS) $(GEN_GRAPHS)

tr02.pdf : $(GEN_RESULTS)
	latexmk -pdf -use-make tr02.tex

scripts/%.pdf : scripts
	$(MAKE) -C scripts $(@F)
scripts/%.tex : scripts
	$(MAKE) -C scripts $(@F)


