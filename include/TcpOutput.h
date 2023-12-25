#ifndef TCP_MIDDLEWARE_TCPOUTPUT_H
#define TCP_MIDDLEWARE_TCPOUTPUT_H

#include <netdb.h>

class TcpOutput {
public:
    TcpOutput(unsigned short port, unsigned short backlog = 2);

    void setup_server();
    void listen_for_client();

    void broadcast(short msg);
private:
    unsigned short m_port;
    unsigned short m_backlog;

    int m_socket_fd;
    sockaddr_in *mp_address;

    int *mp_client_sockets;
    unsigned short m_client_count = 0;

    void setup_socket();
    void bind_socket();
    void listen_socket() const;

    void accept_connection();
    void add_client_socket(int socket);

    void send_to_client(const short &msg, const unsigned short &client_socket);

#if defined(_WIN32)
#define get_socket_error() (WSAGetLastError())
#else
#define get_socket_error() (errno)
#endif
};

#endif
