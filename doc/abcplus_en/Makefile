# Makefile for abcplus_en.tex

VERSION =  2012-03-30
NAME    =  abcplus_en
MANUAL  =  $(NAME).pdf
TEXFILE =  $(NAME).tex params.tex
FIGURES =  accid.pdf ave.pdf aveverum.pdf bang.pdf bars.pdf \
           bassalto.pdf break.pdf changefont.pdf chords.pdf \
           deco.pdf decoline.pdf dotted.pdf dotted2.pdf double.pdf \
	   figuredbass.pdf grandstaff.pdf inline.pdf \
           length.pdf brother1.pdf brother2.pdf gchord.pdf grace.pdf \
           groups.pdf happyb.pdf isolatin.pdf itachords.pdf \
           kyrie.pdf legato.pdf legato2.pdf metre.pdf \
	   microtonal.pdf multiplechords.pdf newdot.pdf nochord.pdf \
	   perc.pdf repeat.pdf repeat3.pdf rests2.pdf rests.pdf \
	   riu.pdf scale1.pdf scale2.pdf scale3.pdf scale4.pdf \
	   staffnonote.pdf stanzas.pdf stille.pdf \
	   studio.pdf syllables.pdf titlepage.pdf tuplets.pdf \
	   tuplets2.pdf whistle.pdf
ABCFILES = $(FIGURES:.pdf=.abc)
MIDIS =    accomp.mid amazinggrace.mid broken.mid folia.mid garota.mid \
      	   impressioni.mid riff.mid swallowtail.mid
MP3 =      accomp.mp3 amazinggrace.mp3 broken.mp3 folia.mp3 garota.mp3 \
      	   impressioni.mp3 riff.mp3 swallowtail.mp3


# these are the localised figures

FIGURES_LOC =  alldeco_en.pdf annotations_en.pdf clefs_en.pdf \
               columns_en.pdf custdeco_en.pdf \
               epsinclude_en.pdf happyb2_en.pdf happyb3_en.pdf \
               measures_en.pdf parts_en.pdf repeat2_en.pdf \
               setfont_en.pdf specialstaves_en.pdf \
	       titles_en.pdf typeset_en.pdf utf8_en.pdf
ABCFILES_LOC = $(FIGURES_LOC:.pdf=.abc)

# provided

PROVIDED     = newfont.pdf fonts.pdf logo.eps screenshot-linux.png \
	       screenshot-win.png


$(MANUAL): $(TEXFILE) $(FIGURES) $(FIGURES_LOC) $(PROVIDED)
	pdflatex  $(TEXFILE); \
        pdflatex  $(TEXFILE); \
        pdflatex  $(TEXFILE)

figures: $(FIGURES) $(FIGURES_LOC)

midi: $(MIDIS)

mp3:  $(MIDIS)
	cd midi; sh makemp3.sh

dist: $(MANUAL) $(MIDIS)
	ln -s . $(NAME)-$(VERSION); \
        cp ./midi/*mid . ; \
	cp ./midi/*mp3 . ; \
        zip -r $(NAME)-$(VERSION).zip \
        $(NAME)-$(VERSION)/$(MANUAL) \
        $(NAME)-$(VERSION)/*mid \
	$(NAME)-$(VERSION)/*mp3

clean:
	/bin/rm -f *.aux sources/*.aux *aux *.lo? *.out *.toc *.log \
	./midi/*.m* *.mid *~ $(NAME)-$(VERSION) $(NAME)-$(VERSION).zip

cleanpdf:
	/bin/rm -f $(FIGURES) $(FIGURES_LOC)

love:
	# Not war?!

# COMMON FIGURES

accid.pdf: ./sources/accid.abc
	sh ./makefig.sh accid.abc
ave.pdf: ./sources/ave.abc
	sh ./makefig.sh ave.abc
aveverum.pdf: ./sources/aveverum.abc
	sh ./makefig.sh aveverum.abc
bang.pdf: ./sources/bang.abc
	sh ./makefig.sh bang.abc
bars.pdf: ./sources/bars.abc
	sh ./makefig.sh bars.abc
bassalto.pdf: ./sources/bassalto.abc
	sh ./makefig.sh bassalto.abc
break.pdf: ./sources/break.abc
	sh ./makefig.sh break.abc
changefont.pdf: ./sources/changefont.abc
	sh ./makefig.sh changefont.abc
chords.pdf: ./sources/chords.abc
	sh ./makefig.sh chords.abc
deco.pdf: ./sources/deco.abc
	sh ./makefig.sh deco.abc
decoline.pdf: ./sources/decoline.abc
	sh ./makefig.sh decoline.abc
dotted.pdf: ./sources/dotted.abc
	sh ./makefig.sh dotted.abc
dotted2.pdf: ./sources/dotted2.abc
	sh ./makefig.sh dotted2.abc
double.pdf: ./sources/double.abc
	sh ./makefig.sh double.abc
figuredbass.pdf: ./sources/figuredbass.abc
	sh ./makefig.sh figuredbass.abc
grandstaff.pdf: ./sources/grandstaff.abc
	sh ./makefig.sh grandstaff.abc
length.pdf: ./sources/length.abc
	sh ./makefig.sh length.abc
brother1.pdf: ./sources/brother1.abc
	sh ./makefig.sh brother1.abc
brother2.pdf: ./sources/brother2.abc
	sh ./makefig.sh brother2.abc
gchord.pdf: ./sources/gchord.abc
	sh ./makefig.sh gchord.abc
grace.pdf: ./sources/grace.abc
	sh ./makefig.sh grace.abc
groups.pdf: ./sources/groups.abc
	sh ./makefig.sh groups.abc
happyb.pdf: ./sources/happyb.abc
	sh ./makefig.sh happyb.abc
inline.pdf: ./sources/inline.abc
	sh ./makefig.sh inline.abc
isolatin.pdf: ./sources/isolatin.abc
	sh ./makefig.sh isolatin.abc
itachords.pdf: ./sources/itachords.abc
	sh ./makefig.sh itachords.abc
kyrie.pdf: ./sources/kyrie.abc
	sh ./makefig.sh kyrie.abc
legato.pdf: ./sources/legato.abc
	sh ./makefig.sh legato.abc
legato2.pdf: ./sources/legato2.abc
	sh ./makefig.sh legato2.abc
metre.pdf: ./sources/metre.abc
	sh ./makefig.sh metre.abc
microtonal.pdf: ./sources/microtonal.abc
	sh ./makefig.sh microtonal.abc
multiplechords.pdf: ./sources/multiplechords.abc
	sh ./makefig.sh multiplechords.abc
newdot.pdf: ./sources/newdot.abc
	sh ./makefig.sh newdot.abc
nochord.pdf: ./sources/nochord.abc
	sh ./makefig.sh nochord.abc
perc.pdf: ./sources/perc.abc
	sh ./makefig.sh perc.abc
repeat.pdf: ./sources/repeat.abc
	sh ./makefig.sh repeat.abc
repeat3.pdf: ./sources/repeat3.abc
	sh ./makefig.sh repeat3.abc
rests2.pdf: ./sources/rests2.abc
	sh ./makefig.sh rests2.abc
rests.pdf: ./sources/rests.abc
	sh ./makefig.sh rests.abc
riu.pdf: ./sources/riu.abc
	sh ./makefig.sh riu.abc
scale1.pdf: ./sources/scale1.abc
	sh ./makefig.sh scale1.abc
scale2.pdf: ./sources/scale2.abc
	sh ./makefig.sh scale2.abc
scale3.pdf: ./sources/scale3.abc
	sh ./makefig.sh scale3.abc
scale4.pdf: ./sources/scale4.abc
	sh ./makefig.sh scale4.abc
staffnonote.pdf: ./sources/staffnonote.abc
	sh ./makefig.sh staffnonote.abc
stanzas.pdf: ./sources/stanzas.abc
	sh ./makefig.sh stanzas.abc
stille.pdf: ./sources/stille.abc
	sh ./makefig.sh stille.abc
studio.pdf: ./sources/studio.abc
	sh ./makefig.sh studio.abc
syllables.pdf: ./sources/syllables.abc
	sh ./makefig.sh syllables.abc
titlepage.pdf: ./sources/titlepage.abc
	sh ./makefig.sh titlepage.abc
tuplets.pdf: ./sources/tuplets.abc
	sh ./makefig.sh tuplets.abc
tuplets2.pdf: ./sources/tuplets2.abc
	sh ./makefig.sh tuplets2.abc
whistle.pdf: ./sources/whistle.abc
	sh ./makefig.sh whistle.abc

# LOCALISED FIGURES

alldeco_en.pdf: ./sources/alldeco_en.abc
	sh ./makefig.sh alldeco_en.abc
annotations_en.pdf: ./sources/annotations_en.abc
	sh ./makefig.sh annotations_en.abc
clefs_en.pdf: ./sources/clefs_en.abc
	sh ./makefig.sh clefs_en.abc
columns_en.pdf: ./sources/columns_en.abc
	sh ./makefig.sh columns_en.abc
custdeco_en.pdf: ./sources/custdeco_en.abc
	sh ./makefig.sh custdeco_en.abc
epsinclude_en.pdf: ./sources/epsinclude_en.abc
	sh ./makefig.sh epsinclude_en.abc
happyb2_en.pdf: ./sources/happyb2_en.abc
	sh ./makefig.sh happyb2_en.abc
happyb3_en.pdf: ./sources/happyb3_en.abc
	sh ./makefig.sh happyb3_en.abc
measures_en.pdf: ./sources/measures_en.abc
	sh ./makefig.sh measures_en.abc
parts_en.pdf: ./sources/parts_en.abc
	sh ./makefig.sh parts_en.abc
repeat2_en.pdf: ./sources/repeat2_en.abc
	sh ./makefig.sh repeat2_en.abc
setfont_en.pdf: ./sources/setfont_en.abc
	sh ./makefig.sh setfont_en.abc
specialstaves_en.pdf: ./sources/specialstaves_en.abc
	sh ./makefig.sh specialstaves_en.abc
titles_en.pdf: ./sources/titles_en.abc
	sh ./makefig.sh titles_en.abc
typeset_en.pdf: ./sources/typeset_en.abc
	sh ./makefig.sh typeset_en.abc
utf8_en.pdf: ./sources/utf8_en.abc
	sh ./makefig.sh utf8_en.abc

# MIDIS

amazinggrace.mid: ./midi/amazinggrace.abc
	abc2midi ./midi/amazinggrace.abc -o ./midi/amazinggrace.mid
accomp.mid: ./midi/accomp.abc
	abc2midi ./midi/accomp.abc -o ./midi/accomp.mid
broken.mid: ./midi/broken.abc
	abc2midi ./midi/broken.abc -o ./midi/broken.mid
folia.mid: ./midi/folia.abc
	abc2midi ./midi/folia.abc -o ./midi/folia.mid
garota.mid: ./midi/garota.abc
	abc2midi ./midi/garota.abc -o ./midi/garota.mid
impressioni.mid: ./midi/impressioni.abc
	abc2midi ./midi/impressioni.abc -o ./midi/impressioni.mid
riff.mid: ./midi/riff.abc
	abc2midi ./midi/riff.abc -o ./midi/riff.mid
swallowtail.mid: ./midi/swallowtail.abc
	abc2midi ./midi/swallowtail.abc -o ./midi/swallowtail.mid


# End of Makefile
