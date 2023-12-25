#include "TcpInput.h"
#include <sys/socket.h>
#include <stdexcept>
#include <netdb.h>
#include <cstring>
#include <iostream>
#include <unistd.h>

using namespace std;

TcpInput::TcpInput(const string hostname, unsigned short port) {
    this->m_hostname = hostname;
    this->m_port = port;
}

TcpInput::~TcpInput() {
    this->close_connection();
}

void TcpInput::open_connection(unsigned int samp_rate) {
    this->connect_socket();
    this->adjust_connection(samp_rate);
}

short int TcpInput::read() {
    short sample;

    int result = sf_read_short(this->mp_input_file, &sample, 1);
    if (result == 0) {
        throw runtime_error("Connection is interrupted");
    }

    return sample;
}

void TcpInput::close_connection() {
    int socket_fd_status = close(this->m_socket_fd);
    int input_file_status = sf_close(this->mp_input_file);

    cout << "TCP connection closed, " <<
         "socket_fd: " << socket_fd_status << ", " <<
         "input_file: " << input_file_status << endl;
}

void TcpInput::connect_socket() {
    this->m_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->m_socket_fd < 0) {
        throw runtime_error("Error opening TCP socket\n");
    }

    struct hostent *server;
    server = gethostbyname(this->m_hostname.c_str());
    if (server == nullptr) {
        throw runtime_error("Entered TCP host was not found\n");
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(this->m_port);
    bcopy((char *)server->h_addr_list[0], (char *)&server_addr.sin_addr.s_addr, server->h_length);

    int status = connect(this->m_socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (status < 0) {
        throw runtime_error("Error opening TCP socket\n");
    }

    cout << "TCP connection established, status: " << status << endl;
}

void TcpInput::adjust_connection(unsigned int samp_rate) {
    auto *in_file_info = new SF_INFO {};
    in_file_info->samplerate = samp_rate;
    in_file_info->channels = 1;
    in_file_info->seekable = 0;
    in_file_info->format = SF_FORMAT_RAW | SF_FORMAT_PCM_16 | SF_ENDIAN_LITTLE;

    this->mp_input_file = sf_open_fd(this->m_socket_fd, SFM_READ, in_file_info, 0);
    if (this->mp_input_file == nullptr) {
        string error_msg = "Error opening TCP input file with with libsndfile, error: \n";
        error_msg += sf_strerror(NULL);
        throw runtime_error(error_msg);
    }
}
