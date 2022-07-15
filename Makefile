coding:
	clang++-12 -O3 -o coding.exe coding.cpp kodr/encoder.cpp kodr/recoder.cpp kodr/decoder.cpp kodr/matrix.cpp kodr/data.cpp kodr/decoder_state.cpp -DMCL_USE_VINT -I ~/.local/include/  -I ./kodr -L ~/.local/lib -Wl,-rpath ~/.local/lib  -l mcl

coding_debug:
	clang++-12 -o kodr/coding.exe kodr/coding.cpp kodr/encoder.cpp kodr/recoder.cpp kodr/decoder.cpp kodr/matrix.cpp kodr/data.cpp kodr/decoder_state.cpp -DMCL_USE_VINT -I ~/.local/include/  -I ./kodr -L ~/.local/lib -Wl,-rpath ~/.local/lib  -l mcl

