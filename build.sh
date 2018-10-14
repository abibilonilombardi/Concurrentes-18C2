rm *.bin -f
rm *.log -f
rm FIFOPUERTO_* -f
rm fifopuerto* -f
rm lock* -f


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
