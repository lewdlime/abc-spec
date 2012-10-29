# Makefile
#
# This Makefile will compile abcpp using Linux, djgpp or mingw out of the box.
# If you use cygwin but want to make a mingw binary, change CFLAGS below.

ABCPP   = abcpp
VERSION = 1.4.5
CC      = gcc
# settings for Linux, djgpp or mingw32
CFLAGS  = -O3 -Wall -D VERSION=\"$(VERSION)\"
# to make a mingw executable using cygwin, use these CFLAGS:
# CFLAGS = -O3 -Wall -mno-cygwin -DWIN32

SOURCE  = $(ABCPP).c

$(ABCPP): $(SOURCE)
	$(CC) $(CFLAGS) -o $(ABCPP) $(SOURCE)

tgz:    $(ABCPP)
	ln -s . $(ABCPP)-$(VERSION); \
	tar -zcvf $(ABCPP)-$(VERSION).tar.gz \
        $(ABCPP)-$(VERSION)/COPYING \
        $(ABCPP)-$(VERSION)/abcpp.spec \
        $(ABCPP)-$(VERSION)/LEGGIMI \
        $(ABCPP)-$(VERSION)/Makefile \
        $(ABCPP)-$(VERSION)/README \
        $(ABCPP)-$(VERSION)/$(SOURCE) \
        $(ABCPP)-$(VERSION)/examples/*

zip:    $(ABCPP)
	cd ..; zip -r $(ABCPP)-$(VERSION).zip $(ABCPP)-$(VERSION)/*

clean:
	rm -f *.o

# End of file Makefile
