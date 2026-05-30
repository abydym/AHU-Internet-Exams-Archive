import socket  
import tkinter as tk  
from tkinter import filedialog, messagebox, Listbox, Scrollbar  
import os  
import threading  

# 连接到服务器  
def connect_to_server():  
    global client_socket  
    ip = ip_entry.get()  
    port = int(port_entry.get())  
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
    try:  
        client_socket.connect((ip, port))  
        messagebox.showinfo("Connection", "Connected to server!")  
        list_files()  # 连接后立即列出文件  
    except Exception as e:  
        messagebox.showerror("Connection Error", str(e))  

# 上传文件  
def upload_file():  
    file_path = filedialog.askopenfilename()  
    if file_path:  
        client_socket.send(f'UPLOAD {os.path.basename(file_path)}'.encode())  
        with open(file_path, 'rb') as f:  
            data = f.read(1024)  
            while data:  
                client_socket.send(data)  
                data = f.read(1024)  
        # 上传完成后弹出提示框  
        messagebox.showinfo("Upload", "File uploaded successfully!")  
        list_files()  # 上传后更新文件列表  

# 列出服务器文件  
def list_files():  
    threading.Thread(target=_list_files).start()  # 使用线程避免阻塞  

def _list_files():  
    client_socket.send(b'LIST')  
    files = client_socket.recv(4096).decode()  
    file_list.delete(0, tk.END)  # 清空当前列表  
    for file in files.split('\n'):  
        if file:  # 确保文件名不为空  
            file_list.insert(tk.END, file)  # 添加文件到列表框  

# 下载文件  
def download_file():  
    selected_file = file_list.get(tk.ACTIVE)  
    if selected_file:  
        client_socket.send(f'DOWNLOAD {selected_file}'.encode())  
        response = client_socket.recv(1024).decode()  
        if response.startswith('EXISTS'):  
            size = int(response.split()[1])  
            with open(selected_file, 'wb') as f:  
                received = 0  
                while received < size:  
                    data = client_socket.recv(1024)  
                    f.write(data)  
                    received += len(data)  
            # 下载完成后弹出提示框  
            messagebox.showinfo("Download", "File downloaded successfully!")  
        else:  
            messagebox.showerror("Download Error", "File not found on server.")  

# 创建主窗口  
root = tk.Tk()  
root.title("File Transfer Client")  
root.geometry("400x400")  # 设置窗口大小  
root.configure(bg="#f0f0f0")  # 设置背景颜色  

# IP地址和端口输入框  
tk.Label(root, text="Server IP:", bg="#f0f0f0").pack(pady=5)  
ip_entry = tk.Entry(root)  
ip_entry.pack(pady=5)  

tk.Label(root, text="Port:", bg="#f0f0f0").pack(pady=5)  
port_entry = tk.Entry(root)  
port_entry.pack(pady=5)  

# 连接按钮  
connect_button = tk.Button(root, text="Connect", command=connect_to_server, bg="#4CAF50", fg="white")  
connect_button.pack(pady=10)  

# 上传文件按钮  
upload_button = tk.Button(root, text="Upload File", command=upload_file, bg="#2196F3", fg="white")  
upload_button.pack(pady=5)  

# 列出文件按钮  
list_button = tk.Button(root, text="List Files", command=list_files, bg="#FFC107", fg="white")  
list_button.pack(pady=5)  

# 文件列表框和滚动条  
file_list_frame = tk.Frame(root)  
file_list_frame.pack(pady=10)  

file_list = Listbox(file_list_frame, width=50, height=10)  
file_list.pack(side=tk.LEFT, fill=tk.BOTH)  

scrollbar = Scrollbar(file_list_frame)  
scrollbar.pack(side=tk.RIGHT, fill=tk.Y)  

file_list.config(yscrollcommand=scrollbar.set)  
scrollbar.config(command=file_list.yview)  

# 下载文件按钮  
download_button = tk.Button(root, text="Download File", command=download_file, bg="#FF5722", fg="white")  
download_button.pack(pady=5)  

# 运行主循环  
root.mainloop()  
