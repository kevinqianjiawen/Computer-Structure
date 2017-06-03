ELF=$(notdir $(CURDIR)).elf
TOOLROOT=/l/arm2/devtools/bin/

CC=$(TOOLROOT)/arm-none-eabi-gcc
LD=$(TOOLROOT)/arm-none-eabi-gcc
AR=$(TOOLROOT)/arm-none-eabi-ar
AS=$(TOOLROOT)/arm-none-eabi-as
GDB=$(TOOLROOT)/arm-none-eabi-gdb
QEMU=$(TOOLROOT)/qemu-system-arm
OBJDUMP=$(TOOLROOT)/arm-none-eabi-objdump

CFLAGS = -g -mcpu=cortex-m0 -mthumb -specs=rdimon.specs -lc -lrdimon
LDFLAGS =

OBJS = test.o orrs.o

$(ELF) : $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $@

.s.o : 
	$(AS) -g $< -o $@
	$(OBJDUMP) -d $@ > $(@:.o=.lst)

.c.o : 
	$(CC) $(CFLAGS) -c $< -o $@

test : $(ELF)
	$(QEMU)  -cpu cortex-m3 -nographic \
                   -monitor null -serial null \
                   -machine integratorcp \
                   -semihosting -kernel $<

debug: $(ELF)
	$(QEMU) -cpu cortex-m3 -nographic -monitor null \
                 -serial null -semihosting -kernel \
                   -machine integratorcp \
                 $< -gdb tcp::51234 -S &
	arm-none-eabi-gdb $< --command=main.gdb

clean: 
	rm -f $(OBJS) $(ELF) $(OBJS:.o=.lst)

     
