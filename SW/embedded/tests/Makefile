TESTS =

# FIFO
TESTS += fifo
SRC_fifo = ../tools/fifo.c

################################################################################
OUTS = $(addsuffix .out,$(TESTS))

# Make objects
$(foreach TEST,$(TESTS),$(eval OBJ_$(TEST) = $(TEST).o $(patsubst \
%.c,%.o,$(SRC_$(TEST)))))

# Cleanup file
CLEANUP = 
$(foreach TEST,$(TESTS),$(eval CLEANUP += $(OBJ_$(TEST))))


LDFLAGS += -lcpputest

################################################################################
all: $(OUTS)

clean: 
	$(RM) $(OUTS) $(CLEANUP)

# Enable second expansion
.SECONDEXPANSION:

%.out: $$(OBJ_%)
	$(CXX) $(LDFLAGS) $? -o $@
	./$@



.PHONY: all clean

