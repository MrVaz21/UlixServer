import socket

def send_ulix_request(request: str) -> str:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect(('localhost', 8080))
        s.send(request.encode())
        return s.recv(4096).decode()

if __name__ == "__main__":
    req = "method/post|datatype/html|code/utf-8|password/1111\r\n<html><body><h1>Hello Ulix!</h1></body></html>"
    print(send_ulix_request(req))
