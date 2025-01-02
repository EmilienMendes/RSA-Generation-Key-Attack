.PHONY: spa cpa 

all : spa cpa

compilation :
	$(MAKE) -C spa-crible-simple compilation_cpa
	$(MAKE) -C cpa-crible-optimise compilation_spa

spa : 
	$(MAKE) -C spa-crible-simple spa

cpa : 
	$(MAKE) -C cpa-crible-optimise cpa


clean :
	$(MAKE) -C spa-crible-simple clean
	$(MAKE) -C cpa-crible-optimise clean