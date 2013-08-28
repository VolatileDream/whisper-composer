
.PHONY: all
all : cpp c python

.PHONY: cpp
cpp :
	cd src/cpp ; make all

.PHONY: c
c : cpp
	cd src/c ; make all

.PHONY: python
python : c
	cd src/python ; ./make-python.sh

.PHONY: clean
clean :
	rm lib/*
