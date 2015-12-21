# Makefile for abcplus_en.tex

VERSION =  2015-09-03
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
	   tuplets2.pdf voices.pdf whistle.pdf
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

PROVIDED     = newfont.pdf fonts.pdf logo.eps

$(MANUAL): $(TEXFILE) $(FIGURES) $(FIGURES_LOC) $(PROVIDED)
	pdflatex  $(TEXFILE); \
        pdflatex  $(TEXFILE); \
        pdflatex  $(TEXFILE)

figures: $(FIGURES) $(FIGURES_LOC)

midi: $(MIDIS)

mp3:  $(MIDIS)
	cd Midi; sh makemp3.sh

dist: $(MANUAL)
	ln -s . $(NAME)-$(VERSION); \
        cp ./Midi/*mid . ; \
	cp ./Midi/*mp3 . ; \
        zip -r $(NAME)-$(VERSION).zip \
        $(NAME)-$(VERSION)/$(MANUAL) \
        $(NAME)-$(VERSION)/*mid \
	$(NAME)-$(VERSION)/*mp3

clean:
	/bin/rm -f *.aux Sources/*.aux *aux *.lo? *.out *.toc *.log \
	./Midi/*.m* *.mid *~ $(NAME)-$(VERSION) $(NAME)-$(VERSION).zip

cleanpdf:
	/bin/rm -f $(FIGURES) $(FIGURES_LOC)

love:
	# Not war?!

# COMMON FIGURES

accid.pdf: ./Sources/accid.abc
	sh ./makefig.sh accid.abc
ave.pdf: ./Sources/ave.abc
	sh ./makefig.sh ave.abc
aveverum.pdf: ./Sources/aveverum.abc
	sh ./makefig.sh aveverum.abc
bang.pdf: ./Sources/bang.abc
	sh ./makefig.sh bang.abc
bars.pdf: ./Sources/bars.abc
	sh ./makefig.sh bars.abc
bassalto.pdf: ./Sources/bassalto.abc
	sh ./makefig.sh bassalto.abc
break.pdf: ./Sources/break.abc
	sh ./makefig.sh break.abc
changefont.pdf: ./Sources/changefont.abc
	sh ./makefig.sh changefont.abc
chords.pdf: ./Sources/chords.abc
	sh ./makefig.sh chords.abc
deco.pdf: ./Sources/deco.abc
	sh ./makefig.sh deco.abc
decoline.pdf: ./Sources/decoline.abc
	sh ./makefig.sh decoline.abc
dotted.pdf: ./Sources/dotted.abc
	sh ./makefig.sh dotted.abc
dotted2.pdf: ./Sources/dotted2.abc
	sh ./makefig.sh dotted2.abc
double.pdf: ./Sources/double.abc
	sh ./makefig.sh double.abc
figuredbass.pdf: ./Sources/figuredbass.abc
	sh ./makefig.sh figuredbass.abc
grandstaff.pdf: ./Sources/grandstaff.abc
	sh ./makefig.sh grandstaff.abc
length.pdf: ./Sources/length.abc
	sh ./makefig.sh length.abc
brother1.pdf: ./Sources/brother1.abc
	sh ./makefig.sh brother1.abc
brother2.pdf: ./Sources/brother2.abc
	sh ./makefig.sh brother2.abc
gchord.pdf: ./Sources/gchord.abc
	sh ./makefig.sh gchord.abc
grace.pdf: ./Sources/grace.abc
	sh ./makefig.sh grace.abc
groups.pdf: ./Sources/groups.abc
	sh ./makefig.sh groups.abc
happyb.pdf: ./Sources/happyb.abc
	sh ./makefig.sh happyb.abc
inline.pdf: ./Sources/inline.abc
	sh ./makefig.sh inline.abc
isolatin.pdf: ./Sources/isolatin.abc
	sh ./makefig.sh isolatin.abc
itachords.pdf: ./Sources/itachords.abc
	sh ./makefig.sh itachords.abc
kyrie.pdf: ./Sources/kyrie.abc
	sh ./makefig.sh kyrie.abc
legato.pdf: ./Sources/legato.abc
	sh ./makefig.sh legato.abc
legato2.pdf: ./Sources/legato2.abc
	sh ./makefig.sh legato2.abc
metre.pdf: ./Sources/metre.abc
	sh ./makefig.sh metre.abc
microtonal.pdf: ./Sources/microtonal.abc
	sh ./makefig.sh microtonal.abc
multiplechords.pdf: ./Sources/multiplechords.abc
	sh ./makefig.sh multiplechords.abc
newdot.pdf: ./Sources/newdot.abc
	sh ./makefig.sh newdot.abc
nochord.pdf: ./Sources/nochord.abc
	sh ./makefig.sh nochord.abc
perc.pdf: ./Sources/perc.abc
	sh ./makefig.sh perc.abc
repeat.pdf: ./Sources/repeat.abc
	sh ./makefig.sh repeat.abc
repeat3.pdf: ./Sources/repeat3.abc
	sh ./makefig.sh repeat3.abc
rests2.pdf: ./Sources/rests2.abc
	sh ./makefig.sh rests2.abc
rests.pdf: ./Sources/rests.abc
	sh ./makefig.sh rests.abc
riu.pdf: ./Sources/riu.abc
	sh ./makefig.sh riu.abc
scale1.pdf: ./Sources/scale1.abc
	sh ./makefig.sh scale1.abc
scale2.pdf: ./Sources/scale2.abc
	sh ./makefig.sh scale2.abc
scale3.pdf: ./Sources/scale3.abc
	sh ./makefig.sh scale3.abc
scale4.pdf: ./Sources/scale4.abc
	sh ./makefig.sh scale4.abc
staffnonote.pdf: ./Sources/staffnonote.abc
	sh ./makefig.sh staffnonote.abc
stanzas.pdf: ./Sources/stanzas.abc
	sh ./makefig.sh stanzas.abc
stille.pdf: ./Sources/stille.abc
	sh ./makefig.sh stille.abc
studio.pdf: ./Sources/studio.abc
	sh ./makefig.sh studio.abc
syllables.pdf: ./Sources/syllables.abc
	sh ./makefig.sh syllables.abc
titlepage.pdf: ./Sources/titlepage.abc
	sh ./makefig.sh titlepage.abc
tuplets.pdf: ./Sources/tuplets.abc
	sh ./makefig.sh tuplets.abc
tuplets2.pdf: ./Sources/tuplets2.abc
	sh ./makefig.sh tuplets2.abc
voices.pdf: ./Sources/voices.abc
	sh ./makefig.sh voices.abc
whistle.pdf: ./Sources/whistle.abc
	sh ./makefig.sh whistle.abc

# LOCALISED FIGURES

alldeco_en.pdf: ./Sources/alldeco_en.abc
	sh ./makefig.sh alldeco_en.abc
annotations_en.pdf: ./Sources/annotations_en.abc
	sh ./makefig.sh annotations_en.abc
clefs_en.pdf: ./Sources/clefs_en.abc
	sh ./makefig.sh clefs_en.abc
columns_en.pdf: ./Sources/columns_en.abc
	sh ./makefig.sh columns_en.abc
custdeco_en.pdf: ./Sources/custdeco_en.abc
	sh ./makefig.sh custdeco_en.abc
epsinclude_en.pdf: ./Sources/epsinclude_en.abc
	sh ./makefig.sh epsinclude_en.abc
happyb2_en.pdf: ./Sources/happyb2_en.abc
	sh ./makefig.sh happyb2_en.abc
happyb3_en.pdf: ./Sources/happyb3_en.abc
	sh ./makefig.sh happyb3_en.abc
measures_en.pdf: ./Sources/measures_en.abc
	sh ./makefig.sh measures_en.abc
parts_en.pdf: ./Sources/parts_en.abc
	sh ./makefig.sh parts_en.abc
repeat2_en.pdf: ./Sources/repeat2_en.abc
	sh ./makefig.sh repeat2_en.abc
setfont_en.pdf: ./Sources/setfont_en.abc
	sh ./makefig.sh setfont_en.abc
specialstaves_en.pdf: ./Sources/specialstaves_en.abc
	sh ./makefig.sh specialstaves_en.abc
titles_en.pdf: ./Sources/titles_en.abc
	sh ./makefig.sh titles_en.abc
typeset_en.pdf: ./Sources/typeset_en.abc
	sh ./makefig.sh typeset_en.abc
utf8_en.pdf: ./Sources/utf8_en.abc
	sh ./makefig.sh utf8_en.abc

# MIDIS

amazinggrace.mid: ./Midi/amazinggrace.abc
	abc2midi ./Midi/amazinggrace.abc -o ./Midi/amazinggrace.mid
accomp.mid: ./Midi/accomp.abc
	abc2midi ./Midi/accomp.abc -o ./Midi/accomp.mid
broken.mid: ./Midi/broken.abc
	abc2midi ./Midi/broken.abc -o ./Midi/broken.mid
folia.mid: ./Midi/folia.abc
	abc2midi ./Midi/folia.abc -o ./Midi/folia.mid
garota.mid: ./Midi/garota.abc
	abc2midi ./Midi/garota.abc -o ./Midi/garota.mid
impressioni.mid: ./Midi/impressioni.abc
	abc2midi ./Midi/impressioni.abc -o ./Midi/impressioni.mid
riff.mid: ./Midi/riff.abc
	abc2midi ./Midi/riff.abc -o ./Midi/riff.mid
swallowtail.mid: ./Midi/swallowtail.abc
	abc2midi ./Midi/swallowtail.abc -o ./Midi/swallowtail.mid


# End of Makefile
