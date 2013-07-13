
.PHONY: all
all : lib c python

.PHONY: lib
lib :
	cd src/c ; ./make-lib.sh

.PHONY: c
# this should rely on lib, but doesn't.
c :
	cd src/c ; ./make-c.sh

.PHONY: python
python : lib
	cd src/python ; ./make-python.sh

.PHONY: clean
clean :
	# go through all languages and call clean
	# this for simplicity
	-cd src ; rm */*.so *.so ./python/*pyc
