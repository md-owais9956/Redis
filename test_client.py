
import socket

HOST = "127.0.0.1"
PORT = 6379


def encode_command(*args):
    """Encode a command using the RESP array format."""
    result = f"*{len(args)}\r\n"

    for arg in args:
        arg = str(arg)
        result += f"${len(arg.encode())}\r\n{arg}\r\n"

    return result.encode()


def read_response(sock):
    """Read one complete basic RESP response."""
    first = sock.recv(1)

    if not first:
        raise ConnectionError("Server closed the connection")

    line = first + read_line(sock)

    if first == b"$":
        length = int(line[1:].strip())

        if length == -1:
            return "(nil)"

        data = b""
        while len(data) < length + 2:
            chunk = sock.recv(length + 2 - len(data))
            if not chunk:
                raise ConnectionError("Incomplete bulk response")
            data += chunk

        return data[:length].decode(errors="replace")

    if first == b"*":
        return line.decode(errors="replace").strip()

    return line.decode(errors="replace").strip()


def read_line(sock):
    data = b""

    while not data.endswith(b"\r\n"):
        chunk = sock.recv(1)
        if not chunk:
            raise ConnectionError("Incomplete response")
        data += chunk

    return data


def command(sock, *args):
    sock.sendall(encode_command(*args))
    response = read_response(sock)
    print(f"> {' '.join(args)}")
    print(response)
    print()


with socket.create_connection((HOST, PORT), timeout=5) as client:
    print("Connected to Mini Redis!\n")

    command(client, "PING")
    command(client, "SET", "name", "Owais")
    command(client, "GET", "name")
    command(client, "DEL", "name")
    command(client, "GET", "name")

    command(client, "GET", "missing")
    command(client, "HELLO")
    command(client, "SET", "onlykey")
