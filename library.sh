### Create object file
g++ -fPIC -c Threadpool.cpp -pthread -std=c++11

### Create shared object file
g++ -shared -Wl,-soname,libcurieux.so.1 -o libcurieux.so.1.0 Threadpool.o

### Create symbolic links
ln -sf libcurieux.so.1.0 libcurieux.so.1
ln -sf libcurieux.so.1.0 libcurieux.so

### Update ldconfig
ldconfig -n .

### Add library path
LD_LIBRARY_PATH=/usr/local/lib
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/IIITH/OS/Project/Threadpool\ final
export LD_LIBRARY_PATH

### Compile executable program
g++ -o pooltest threadpool_test.cpp -pthread -L. -lcurieux -std=c++11

### Run program
./pooltest
