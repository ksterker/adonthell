EXTRA_DIST = reference.cfg mainpage.dxt 

doc:
	cd $(srcdir) && doxygen reference.cfg
	# cd $(srcdir) && xsltproc xml/combine.xslt xml/index.xml > adonthell-api.xml
	# cd $(srcdir) && python doxy2swig.py adonthell-api.xml adonthell-api.i
