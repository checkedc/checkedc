
TEX_SCRIPTS=bm_results_tab.tex bm_results_macros.tex
GRAPHS=modifications.pdf overheads.pdf
RESULTS=$(TEX_SCRIPTS) $(GRAPHS)

.PHONY : clean all install
all : $(RESULTS)

install : bm_res_install.r
  ./bm_res_install.r

clean :
	rm $(RESULTS)


$(RESULTS) : bm_res.r bm_res.csv
	./bm_res.r

