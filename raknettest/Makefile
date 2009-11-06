
PREFIX=/home/user-pool/users/jw7082/lib


main: main.cpp
	g++ -o test main.cpp -I/home/user-pool/users/jw7082/lib/usr/include -I/home/user-pool/users/jw7082/lib/usr/include/raknet -L/home/user-pool/users/jw7082/lib/usr/lib -lraknet -lpthread

run:
	export LD_LIBRARY_PATH="${PREFIX}/usr/lib/"; ./test
