dist: *.py clean
	python setup.py install --root=dist --no-compile
	mkdir -p dist/DEBIAN
	cp -r debian/* dist/DEBIAN
	cp -r init.d dist/etc/
	dpkg -b dist vbtspa.deb
clean:
	/bin/rm -rf dist build *.deb
