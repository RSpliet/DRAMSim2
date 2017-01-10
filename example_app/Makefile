
#tell the linker the rpath so that we don't have to muck with LD_LIBRARY_PATH, etc
all: dramsim_big dramsim_test

dramsim_big: dramsim_big.cpp
	$(CXX) -g -o dramsim_big dramsim_big.cpp -I../ -L../ -ldramsim -Wl,-rpath=../
dramsim_test: dramsim_test.cpp
	$(CXX) -g -o dramsim_test dramsim_test.cpp -I../ -L../ -ldramsim -Wl,-rpath=../

clean: 
	rm dramsim_test
	rm dramsim_big
