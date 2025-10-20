import socket

SERVER_HOST = "localhost"
SERVER_PORT = 8080

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

try:
    client_socket.connect((SERVER_HOST,SERVER_PORT))
    print(f"connected to {SERVER_HOST}:{SERVER_PORT}")

    while True:
        message = input("the message? ")
        cmd = f"GET / HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\nMessage:{message}\r\n".encode()
        client_socket.send(cmd)
        response = ""

        while True:
            data = client_socket.recv(512)

            if len(data) < 1:
                break
            else:
                print(data.decode('utf-8'))
        print(response)

except ConnectionRefusedError:
    print("the connection was refused.make sure the server is running")

finally:
    client_socket.close()
