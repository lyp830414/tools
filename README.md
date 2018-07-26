This tool supply two functions: generate abi file and generate wasm file.

1. For generating abi file, you need provide the *.hpp file and try as:
	python gentool.py **.hpp
   Then, the **.abi file will be generated under current directory.

2. For generating wasm file, you need provide the *.cpp file and try as:
	python gentool.py wasm **.cpp

   Then, the **.wast and **.wasm file will be generated under current directory.

3. Restrictions for generate wasm file's cpp
        
	a. Your cpp file must #include "lib/contractcomm.hpp" at first.
	
	b. Be sure to keep "lib" folder directly under your cpp file's path.
	
	c. Your cpp file must be under the directory which contains gentool.py.
	
	d. If you move the 'lib' directory onto other path, it even still reports failure as you change the include lib path in your cpp.
