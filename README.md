This tool supply two functions: generate abi file and generate wasm file.

1. For generating abi file, you need provide the *.hpp file and try as:
	python gentool.py **.hpp
   Then, the **.abi file will be generated under current directory.

2. For generating wasm file, you need provide the *.cpp file and try as:
	python gentool.py wasm **.cpp

   Then, the **.wast and **.wasm file will be generated under current directory.
