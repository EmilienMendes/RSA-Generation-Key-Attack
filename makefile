.PHONY: generation_premier  

all : generation_premier

compilation :
	$(MAKE) -C generation_premier compilation

generation_premier : 
	$(MAKE) -C generation_premier all

clean :
	$(MAKE) -C generation_premier clean