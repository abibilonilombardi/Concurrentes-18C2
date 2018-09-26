if [ -d "/build" ]
then
	# if build directory exists
	# clean build directory contents
	rm -rf build/*	
fi

mkdir -p ./build

cd build

# generate Makefile and run 'make' command.
cmake ../src && make
