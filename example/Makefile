compile:
	$(CC) -o librand.so -shared -fPIC librand.c -ldl
	$(CC) -o dice dice.c
	
run:
	@LD_PRELOAD=./librand.so ./dice

help:
	@echo "compile   to compile"
	@echo "run       to preload librand.so"

clean:
	rm dice
	rm librand.so
