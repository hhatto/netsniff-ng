trafgen-libs =	$(shell pkg-config --libs libnl-3.0) \
		$(shell pkg-config --libs libnl-genl-3.0) \
		-lm

trafgen-objs =	xmalloc.o \
		ioops.o \
		xutils.o \
		irq.o \
		str.o \
		mac80211.o \
		ring_tx.o \
		ring.o \
		trafgen_lexer.yy.o \
		trafgen_parser.tab.o \
		trafgen.o

trafgen-lex =	trafgen_lexer.yy.o

trafgen-yaac =	trafgen_parser.tab.o
