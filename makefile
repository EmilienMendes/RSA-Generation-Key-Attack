.PHONY: acquisition  attaque_spa

all : acquisition attaque_spa

compilation :
	$(MAKE) -C spa-crible-simple compilation

acquisition : 
	$(MAKE) -C spa-crible-simple generation_premier

attaque_spa :
	$(MAKE) -C spa-crible-simple attaque_spa
	

clean :
	$(MAKE) -C spa-crible-simple clean