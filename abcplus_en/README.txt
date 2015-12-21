README
------

This is the LaTeX source (complete with ABC examples) of "Making Music
with ABC 2" (formerly "Making Music with Abc Plus").

To obtain the PDF version, you will need:

  - abcm2ps version 7.8.14. Rename or copy the abcm2ps binary to
  "abcm2ps-7.8.14", or edit the makefig.sh script.
  
  - ghostscript
  
  - a working LaTeX installation

  - a Unix-like shell

Just type 'make'. The figures will be generated and pdflatex will be run
three times.

Type 'make clean' to get rid of temporary files.

Type 'make cleanpdf' to get rid of all PDF files except
"abcplus_en.pdf".

---

Note for Ubuntu/Mint users
--------------------------

I compiled the sources on a Mint 17 Linux machine. If you notice that
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
texlive-font-utils **
texlive-fonts-recommended **
texlive-fonts-recommended-doc
texlive-formats-extra
texlive-generic-extra
texlive-generic-recommended
texlive-humanities
texlive-humanities-doc
texlive-latex-base
texlive-latex-base-doc
texlive-latex-extra **
texlive-latex-extra-doc
texlive-latex-recommended
texlive-latex-recommended-doc
texlive-luatex
texlive-pictures
texlive-pictures-doc
texlive-pstricks
texlive-pstricks-doc
(texlive-doc-en contains the info pages)

All you need to install are the packages marked with **. All
others will be installed as dependencies.
