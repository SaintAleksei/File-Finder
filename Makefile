BUILDDIR:= $(CURDIR)/build
TARGET  := $(BUILDDIR)/ff
INCDIRS := $(CURDIR)
OBJS    := $(addprefix $(BUILDDIR)/,$(patsubst %.c,%.o,$(wildcard *.c)))
CFLAGS  := -Wall -Wextra -Werror -g

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

$(OBJS): $(BUILDDIR)/%.o: %.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -MMD $(addprefix -I,$(INCDIRS)) -c -o $@ $< 

$(BUILDDIR): 
	mkdir $@

clean:
	rm -rf $(BUILDDIR)

-include $(patsubst %.o,%.d,$(EXTOBJS))
