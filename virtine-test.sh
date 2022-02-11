#!/bin/bash


export LD_LIBRARY_PATH=$(pwd):$LD_LIBRARY_PATH
export OPENSSL_ia32cap="~0x200000200000000"

function valgrind_test() {
	valgrind --tool=callgrind --log-file="callgrind.out" apps/openssl speed -elapsed -evp aes-128-cbc
	gprof2dot --format=callgrind --output=out.dot callgrind.out
	dot -Tpng out.dot -o graph.png
}

# valgrind_test


apps/openssl speed -elapsed -evp aes-128-cbc
exit


FILE=test.txt
MODE=aes-128-cbc
IV=00000000000000000000000000000000
PW=0

echo "Encrypt:"
apps/openssl $MODE -e -a -salt -in $FILE -out $FILE.enc -k $PW -iter 1 -iv $IV

echo "Decrypt:"
apps/openssl $MODE -d -a -salt -in $FILE.enc -out $FILE.dec -k $PW -iter 1 -iv $IV

echo "Printing diffs"
md5sum $FILE $FILE.dec
