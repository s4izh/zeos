entrega:
	cd project && make clean
	tar -cvzf zeos.tar.gz project

test_entrega:
	mkdir test
	cp zeos.tar.gz test
	cd test && tar -xvzf zeos.tar.gz
	cd test/project && make emul
	rm -rf test
