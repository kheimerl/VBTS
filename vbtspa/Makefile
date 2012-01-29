dist: *.py clean
	python setup.py install --root=dist --no-compile
	mkdir -p dist/DEBIAN
	mkdir -p dist/var/log/asterisk/bm_interests
	cp -r debian/* dist/DEBIAN
	cp -r init.d dist/etc/
	dpkg -b dist vbtspa.deb
clean:
	/bin/rm -rf dist build *.deb
