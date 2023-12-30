#include "TcpOutput.h"
#include <sys/socket.h>
#include <stdexcept>
#include <iostream>

using namespace std;

TcpOutput::TcpOutput(unsigned short port, unsigned short backlog) {
    this->m_port = port;
    this->m_backlog = backlog;

    this->mp_address = new sockaddr_in();
    this->mp_address->sin_family = AF_INET;
    this->mp_address->sin_port = htons(this->m_port);
    this->mp_address->sin_addr.s_addr = INADDR_ANY;

    this->mp_client_sockets = new int(this->m_backlog);
}

void TcpOutput::setup_server() {
    this->setup_socket();
    this->bind_socket();
}

void TcpOutput::listen_for_client() {
    this->listen_socket();
    this->accept_connection();
}

void TcpOutput::broadcast(short msg) {
    for (short i = 0; i < this->m_client_count; ++i) {
        this->send_to_client(
            msg,
            this->mp_client_sockets[i]
        );
    }
}

void TcpOutput::setup_socket() {
    this->m_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->m_socket_fd < 0) {
        throw runtime_error("Error opening TCP socket\n");
    }

    int opt = 1;
    int set_sock_result = setsockopt(
        this->m_socket_fd,
        SOL_SOCKET,
        SO_REUSEADDR | SO_REUSEPORT,
        &opt,
        sizeof(opt)
    );
    if (set_sock_result) {
        throw runtime_error("Error setting TCP socket: " + to_string(get_socket_error()) + "\n");
    }

    cout << "The server socket has been setup!" << endl;
}

void TcpOutput::bind_socket() {
    int bind_result = bind(this->m_socket_fd,
        (struct sockaddr*)this->mp_address,
        sizeof(*this->mp_address)
    );
    if (bind_result < 0) {
        throw runtime_error("Error binding TCP socket\n");
    }

    cout << "The server socket has been bound!" << endl;
}

void TcpOutput::listen_socket() const {
    int listen_result = listen(this->m_socket_fd, this->m_backlog);
    if (listen_result < 0) {
        throw runtime_error("Error listening TCP socket\n");
    }
}

void TcpOutput::accept_connection() {
    socklen_t addr_len = sizeof(*this->mp_address);
    int client_socket = accept(
        this->m_socket_fd,
        (struct sockaddr*)this->mp_address,
        &addr_len
    );
    if (client_socket < 0) {
        throw runtime_error("Error accepting a TCP socket\n");
    }

    this->add_client_socket(client_socket);
}

void TcpOutput::add_client_socket(int socket) {
    this->mp_client_sockets[this->m_client_count] = socket;
    ++this->m_client_count;

    cout << "Added client #" << this->m_client_count << endl;
}

void TcpOutput::send_to_client(const short &msg, const unsigned short &client_socket) {
    send(
        client_socket,
        &msg,
        sizeof(msg),
        0
    );
}

int TcpOutput::get_socket_error() {
    return errno; // WSAGetLastError() for _WIN32
}
