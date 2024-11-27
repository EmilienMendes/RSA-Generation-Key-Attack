.PHONY: acquisition  

all : acquisition

compilation :
	$(MAKE) -C spa-crible-simple compilation

acquisition : 
	$(MAKE) -C spa-crible-simple all

clean :
	$(MAKE) -C spa-crible-simple clean