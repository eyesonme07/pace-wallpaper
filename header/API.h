#ifndef API_H
#define API_H

#include"headers.h"
#include"NetworkAccess.h"
#include<QDir>
#include<QDirIterator>

//соединение с API
class F1API{
public:
    F1API();
    virtual ~F1API() = default;
    virtual std::string request() = 0;
protected:
    void connect();
    void disconnect();

    std::string doGet(const std::string &target);
private:
    beast::flat_buffer _dataBuffer;
    const std::string host = "api.ocblacktop.com";
    const std::string port = "443";
    tcp::resolver resolver;
    beast::ssl_stream<beast::tcp_stream> stream;
};

#endif // API_H
