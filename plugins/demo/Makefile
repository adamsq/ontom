SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c,%.o,$(SRCS))
m-deps+=$(OBJS)
EX-OBJS+=build-in.a $(WORKDIR)/exso_demo.so

build-in.a:$(OBJS)
	$(Q)echo "  **AR**      `pwd`/$@"
	$(Q)$(AR) -cqs $@ $^

$(WORKDIR)/exso_demo.so:$(OBJS)
	$(Q)echo "  **LD**      exso_demo.so"
	$(Q)$(LD) -shared $(LDFLAGS) -o $@ $^

include $(WORKDIR)/Rule.make
