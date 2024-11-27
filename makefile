.PHONY: generation_premier  

all : generation_premier

compilation :
	$(MAKE) -C spa-crible-simple compilation

generation_premier : 
	$(MAKE) -C spa-crible-simple all

clean :
	$(MAKE) -C spa-crible-simple clean