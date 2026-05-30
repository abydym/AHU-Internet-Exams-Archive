rm client
rm server
rm receivedFile.tif
g++ client.cpp -o client `pkg-config opencv4 --libs --cflags`
g++ server.cpp -o server `pkg-config opencv4 --libs --cflags`
