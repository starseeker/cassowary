## $Id: Makefile.am,v 1.2 2000/03/13 00:54:07 gjb Exp $
## Process this file with automake to create Makefile.in

SUBDIRS = src

EXTRACT_DOCS = perl ../../../scwm/utilities/dev/scwmdoc -PCL

EXTRA_DIST=cassowary_scm-procedures.txt cassowary_scm-variables.txt cassowary_scm.sgml

DISTCLEANFILES = cassowary_scm-*.txt cassowary_scm.sgml

cassowary_scm.sgml cassowary_scm-procedures.txt cassowary_scm-variables.txt: ../cassowary_scm.cc
	$(EXTRACT_DOCS) -o cassowary_scm.sgml -O cassowary_scm-variables.txt $^ > cassowary_scm-procedures.txt
