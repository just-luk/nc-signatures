main:
	clang++ -o main.exe main.cpp -DMCL_USE_VINT -I ~/.local/include/ -L ~/.local/lib -Wl,-rpath ~/.local/lib  -l mcl
	./main.exe


coding:
	clang++ -o kodr/coding.exe kodr/coding.cpp kodr/encoder.cpp kodr/recoder.cpp kodr/decoder.cpp kodr/matrix.cpp kodr/data.cpp kodr/decoder_state.cpp -DMCL_USE_VINT -I ~/.local/include/  -I ./kodr -L ~/.local/lib -Wl,-rpath ~/.local/lib  -l mcl
	./kodr/coding.exe > out.txt