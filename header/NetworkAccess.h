#ifndef NETWORKACCESS_H
#define NETWORKACCESS_H

#include"headers.h"


//настройка ssl с boost.beast + io_context
class NetworkAccess{
public:
    ~NetworkAccess()=default;

    static NetworkAccess &getInstance(){
        static NetworkAccess instance;

        return instance;
    }

    net::io_context &getIo(){return io;}
    ssl::context &getSSLContext(){return ctx;}

    NetworkAccess(const NetworkAccess &)=delete;
    NetworkAccess& operator=(const NetworkAccess &) = delete;
private:
    NetworkAccess();
    net::io_context io;
    ssl::context ctx;
};

#endif // NETWORKACCESS_H
