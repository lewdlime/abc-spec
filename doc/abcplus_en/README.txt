README
------

This is the LaTeX source (complete with ABC examples) of "Making Music
with ABC Plus".

To obtain the PDF version, you will need:

  - abcm2ps version 5.9.25. Rename or copy the abcm2ps binary to
  "abcm2ps-5.9.25", or edit the makefig.sh script.
  
  - ghostscript
  
  - a working LaTeX installation

  - a Unix-like shell

Just type 'make'. The figures will be generated and pdflatex will be run
three times.

Type 'make clean' to get rid of temporary files.

Type 'make cleanpdf' to get rid of all PDF files.

---

Note for Ubuntu/Mint users
--------------------------

I compiled the sources on a Mint 11 Linux machine. If you notice that
the default Ghostscript is buggy, I recommend that you download the
latest gs-related packages from a Ubuntu repository, in the
pool/universe/g/ghostscript directory.

Besides, I installed the following texlive-related packages:

texlive
texlive-base
texlive-binaries
texlive-common
texlive-doc-base
texlive-extra-utils
texlive-font-utils
texlive-fonts-recommended
texlive-fonts-recommended-doc
texlive-formats-extra
texlive-generic-extra
texlive-generic-recommended
texlive-humanities
texlive-humanities-doc
texlive-latex-base
texlive-latex-base-doc
texlive-latex-extra
texlive-latex-extra-doc
texlive-latex-recommended
texlive-latex-recommended-doc
texlive-luatex
texlive-pictures
texlive-pictures-doc
texlive-pstricks
texlive-pstricks-doc
