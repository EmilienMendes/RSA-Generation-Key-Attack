.PHONY: acquisition_trace_spa  attaque_spa

all : spa

compilation :
	$(MAKE) -C spa-crible-simple compilation

acquisition_trace_spa : 
	$(MAKE) -C spa-crible-simple generation_premier

attaque_spa :
	$(MAKE) -C spa-crible-simple attaque_spa

spa : acquisition_trace_spa attaque_spa	

clean :
	$(MAKE) -C spa-crible-simple clean