CFLAGS=-O1 -fno-dce -fno-dse -fno-tree-dce -fno-tree-dse

targets := $(patsubst %.c,%,$(wildcard *.c))

% : %.c
	$(CC) $(CFLAGS) -o $@ $<

all: $(targets)
compile: $(targets)

clean:
	rm -rf $(targets)
