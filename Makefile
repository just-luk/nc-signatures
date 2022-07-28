coding:
	cd build && ../install.sh && cmake --build . 

install:
	cd build && ../install.sh && cmake --build . && cd .. && ./install.sh
