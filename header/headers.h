#ifndef HEADERS_H
#define HEADERS_H

#include<iostream>
#include<string>
#include<QDebug>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/context.hpp>
#include<QFile>
#include<QTimer>
#include<vector>
#include"settings.h"
#include<optional>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace ssl = net::ssl;
using tcp = net::ip::tcp;

#endif // HEADERS_H
