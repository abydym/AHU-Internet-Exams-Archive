import socket  
import os  

def handle_client(client_socket):  
    while True:  
        # 接收客户端请求  
        request = client_socket.recv(1024).decode()  
        if not request:  
            break  # 如果没有请求，退出循环 

        if request == 'LIST':  
            # 列出当前目录下的文件  
            files = os.listdir('.')  
            response = '\n'.join(files)  
            client_socket.send(response.encode()) 

        elif request.startswith('UPLOAD'):  
            filename = request.split()[1]  
            with open(filename, 'wb') as f:  
                while True:  
                    data = client_socket.recv(1024)  
                    if not data:  
                        break  
                    f.write(data)  
            print(f"Uploaded: {filename}")  

        elif request.startswith('DOWNLOAD'):  
            filename = request.split()[1]  
            if os.path.exists(filename):  
                client_socket.send(b'EXISTS ' + str(os.path.getsize(filename)).encode())  
                with open(filename, 'rb') as f:  
                    data = f.read(1024)  
                    while data:  
                        client_socket.send(data)  
                        data = f.read(1024)  
                print(f"Downloaded: {filename}")  
            else:  
                client_socket.send(b'NOT FOUND') 
                 
        else:  
            break  

    client_socket.close()  

def main():  
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
    server.bind(('0.0.0.0', 55312))  
    server.listen(5)  
    print("Server listening")  

    while True:  
        client_socket, addr = server.accept()  
        print(f"Connection from {addr}")  
        handle_client(client_socket)  

if __name__ == "__main__":  
    main() 
