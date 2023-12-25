#ifndef TCP_MIDDLEWARE_TCPINPUT_H
#define TCP_MIDDLEWARE_TCPINPUT_H

#include <string>
#include <sndfile.h>

using namespace std;

class TcpInput {
public:
    TcpInput(string hostname, unsigned short port);

    ~TcpInput();

    void open_connection(unsigned int samp_rate);

    short read();

    void close_connection();

private:
    string m_hostname;
    unsigned short m_port;

    int m_socket_fd;
    SNDFILE *mp_input_file;

    void connect_socket();
    void adjust_connection(unsigned int samp_rate);
};

#endif
