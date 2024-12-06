.PHONY: acquisition_trace_spa  attaque_spa acquisition_trace_cpa attaque_cpa

all : cpa

compilation :
	$(MAKE) -C spa-crible-simple compilation
	$(MAKE) -C cpa-crible-optimise compilation

acquisition_trace_spa : 
	$(MAKE) -C spa-crible-simple generation_premier

attaque_spa :
	$(MAKE) -C spa-crible-simple attaque_spa

spa : acquisition_trace_spa attaque_spa	

acquisition_trace_cpa : 
	$(MAKE) -C cpa-crible-optimise generation_premier_optimise

attaque_cpa :
	$(MAKE) -C cpa-crible-optimise attaque_cpa

cpa : acquisition_trace_cpa attaque_cpa	


clean :
	$(MAKE) -C spa-crible-simple clean
	$(MAKE) -C cpa-crible-optimise clean