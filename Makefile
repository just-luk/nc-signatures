.ONESHELL:

kodo_install:
	cd kodo
	./waf configure --fifi_path=../fifi
	./waf build
	./waf install

run_kodo_example:
	clang++ -o ./kodo/examples/rlnc/encode_recode_decode_simple ./kodo/examples/rlnc/encode_recode_decode_simple.cpp -I ./kodo/kodo_install/include -L ./kodo/kodo_install/lib -Wl,-rpath ./kodo/kodo_install/lib  -l kodo
	./kodo/examples/rlnc/encode_recode_decode_simple

kodo_test: kodo_install run_kodo_example

fifi_install:
	cd fifi
	./waf configure
	./waf build
	./waf install

run_fifi_example:
	clang++ -o ./fifi/examples/basic_arithmetic/basic_arithmetic ./fifi/examples/basic_arithmetic/basic_arithmetic.cpp -I ./fifi/fifi_install/include -L ./fifi/fifi_install/lib -Wl,-rpath ./fifi/fifi_install/lib  -l fifi
	./fifi/examples/basic_arithmetic/basic_arithmetic

fifi_test: fifi_install run_fifi_example

test: fifi_install kodo_install 
	clang++ -o foo test.cpp -I ~/.local/include -L ~/.local/lib -Wl,-rpath ~/.local/lib  -l pbc -l gmp -l kodo -l fifi && ./foo monalisa.png
	

test_no_install:
	clang++ -o foo test.cpp -I ~/.local/include -L ~/.local/lib -Wl,-rpath ~/.local/lib  -l pbc -l gmp -l kodo -l fifi && ./foo monalisa.png
