SHELL = /bin/sh
INSTALL = @INSTALL@

srcdir = @srcdir@
top_srcdir = @top_srcdir@
VPATH = @srcdir@
prefix = @prefix@
installroot = @installroot@
exec_prefix = @exec_prefix@

bindir = @bindir@
sbindir = @sbindir@
libexecdir = @libexecdir@
datadir = @datadir@
datarootdir = @datarootdir@
sysconfdir = @sysconfdir@
sharedstatedir = @sharedstatedir@
localstatedir = @localstatedir@
libdir = @libdir@
infodir = @infodir@
mandir = @mandir@
includedir = @includedir@
oldincludedir = /usr/include

REPL_EXT=@replsuffix@
SPEC_EXT=@specsuffix@
LIB_DIR=@datadir@/catdoc

all: @charsetcheck@
clean:
install: install-dirs install-spc install-charsets
install-spc: tex.spc ascii.spc tex.rpl ascii.rpl
	    $(INSTALL) -p -m 644 ascii.spc $(DESTDIR)$(installroot)$(LIB_DIR)/ascii$(SPEC_EXT) 
	    $(INSTALL) -p -m 644 tex.spc $(DESTDIR)$(installroot)$(LIB_DIR)/tex$(SPEC_EXT) 
	    $(INSTALL) -p -m 644 ascii.rpl $(DESTDIR)$(installroot)$(LIB_DIR)/ascii$(REPL_EXT)
	    $(INSTALL) -p -m 644 tex.rpl $(DESTDIR)$(installroot)$(LIB_DIR)/tex$(REPL_EXT)
install-charsets:
	for i in *.txt; do\
	   $(INSTALL) -p -m 0644 $$i $(DESTDIR)$(installroot)$(LIB_DIR);\
	done
install-dirs:
	../mkinstalldirs $(DESTDIR)$(installroot)$(LIB_DIR)
distclean:
	rm Makefile
