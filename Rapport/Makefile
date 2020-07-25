LATEX= pdflatex
all: rapport.pdf

rapport.pdf: comp
	rm -f *.log
	rm -f *.aux
	rm -f *.toc
	rm -f *.out


comp: rapport.tex
	$(LATEX)  rapport.tex > /dev/null
	
clean:
	rm -f *.log
	rm -f *.pdf
	rm -f *.aux
	rm -f *.toc
	rm -f *.out
