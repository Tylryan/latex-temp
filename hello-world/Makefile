
LATEX_EXT=.tex
OUT_DIR=out
PDF_FILE=$(OUT_DIR)/*.pdf

PDF_APP=zathura

COMPILER=pdflatex
FLAGS= --output-directory $(OUT_DIR)

all: compile
	killall $(PDF_APP) || :
	$(PDF_APP) $(PDF_FILE) &

# Output 	Depends on
compile: $(PDF_FILE)

# Don't know why $(PDF_FILE) wouldn't work though
# Output 		  Depends on
$(OUT_DIR)/%.pdf: %.tex
	[[ -d $(OUT_DIR) ]] || mkdir $(OUT_DIR)
	$(COMPILER) $(FLAGS) *$(LATEX_EXT)

clean:
	rm $(OUT_DIR)/*
