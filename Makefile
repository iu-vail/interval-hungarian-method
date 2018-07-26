
CXX	= g++
CFLAGS	= -O3 -Wall #-fpermissive -Wl,-b64
AR	= ar
ARFLAGS	= -cr


DIRS = .  plot
PLOTDIR = plot

all: ihm kmh

PLOT_OBJS = $(PLOTDIR)/Cgnuplot.o
KMH_OBJS = Hungarian.o \
	Assignment.o \
	BipartiteGraph.o \
	PlotGraph.o \
	CmdParser.o \
	IHMethod.o

KMH_LIB = hungarian.a

$(PLOTDIR)/Cgnuplot.o:
#	@for $(i) in $(DIRS); do \
#        echo "make all in $(i)..."; 
#	(cd $(i); $(MAKE) $(MFLAGS)); done
	(cd $(PLOTDIR); $(MAKE) $(MFLAGS))
	@echo Done making Ggnuplot.

$(KMH_LIB): $(KMH_OBJS) $(PLOT_OBJS)
	$(AR) $(ARFLAGS) $(KMH_LIB) $(PLOT_OBJS) $(KMH_OBJS)
	@echo Done making hungarian library.
 
%: main_%.cpp $(KMH_LIB)
	$(CXX) $(CFLAGS) $< $(KMH_LIB) -o $@
	@echo Done making $@.

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $<

clean:
	rm -f kmh ihm test *.o *.a *~ *.swp
	(cd $(PLOTDIR); $(MAKE) clean )


