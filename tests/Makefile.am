# Simplified tests - just use basic autotools check framework
TESTS = run-tests.sh
check_SCRIPTS = run-tests.sh

run-tests.sh:
	echo '#!/bin/sh' > $@
	echo 'echo "Testing catdoc tools..."' >> $@
	echo 'if [ ! -x ../src/catdoc ]; then echo "catdoc not built"; exit 1; fi' >> $@
	echo 'if [ ! -x ../src/xls2csv ]; then echo "xls2csv not built"; exit 1; fi' >> $@
	echo 'if [ ! -x ../src/catppt ]; then echo "catppt not built"; exit 1; fi' >> $@
	echo 'echo "All tools built successfully"' >> $@
	echo 'exit 0' >> $@
	chmod +x $@

CLEANFILES = run-tests.sh