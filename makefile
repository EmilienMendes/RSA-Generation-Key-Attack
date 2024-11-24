# .PHONY: all compilation  

all : crible_simple

compilation :
	$(MAKE) -C generation_premier compilation

crible_simple : 
	$(MAKE) -C generation_premier crible_simple


clean :
	$(MAKE) -C generation_premier clean