all: mb

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f mb

