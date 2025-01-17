.PHONY: spa cpa suppression_stat

all : spa cpa

compilation :
	$(MAKE) -C spa-crible-simple compilation_cpa
	$(MAKE) -C cpa-crible-optimise compilation_spa

spa : 
	$(MAKE) -C spa-crible-simple spa

cpa : 
	$(MAKE) -C cpa-crible-optimise cpa

suppression_stat :
	$(MAKE) -C spa-crible-simple suppression_stat
	$(MAKE) -C cpa-crible-optimise suppression_stat
	

clean :
	$(MAKE) -C spa-crible-simple clean
	$(MAKE) -C cpa-crible-optimise clean