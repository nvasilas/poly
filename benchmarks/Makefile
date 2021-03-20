CPPFLAGS += -DNDEBUG
LDLIBS += -lbenchmark -lpthread

CXXFLAGS += -std=c++17 -march=x86-64 -mtune=generic -O3 -pipe \
	    -Wall -pedantic -Werror \
	    -fno-exceptions -fno-rtti

TARGETS = benchmark_polynomial_add \
	  benchmark_add_overloads

all: $(TARGETS)

clean:
	$(RM) $(TARGETS) perf.data*
