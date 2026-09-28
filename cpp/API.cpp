#include"../header/API.h"

//инициализация
F1API::F1API():
    resolver(NetworkAccess::getInstance().getIo()),
    stream(NetworkAccess::getInstance().getIo(),
             NetworkAccess::getInstance().getSSLContext())
{

}


void F1API::connect()
{
    auto const results = resolver.resolve(host,port);

    if (!SSL_set_tlsext_host_name(stream.native_handle(), host.c_str())) {
        throw beast::system_error(
            beast::error_code(static_cast<int>(::ERR_get_error()),
                              net::error::get_ssl_category()));
    }

    beast::get_lowest_layer(stream).connect(results);
    stream.handshake(ssl::stream_base::client);
}

void F1API::disconnect()
{
    beast::error_code ec;
    beast::get_lowest_layer(stream).socket().shutdown(tcp::socket::shutdown_both, ec);
    beast::get_lowest_layer(stream).socket().close(ec);
}

std::string F1API::doGet(const std::string &target)
{
    try {
        http::request<http::string_body> req{http::verb::get, target, 11};
        req.set(http::field::host, host);
        req.set(http::field::user_agent, "F1_Live_Wallpaper/1.0");

        req.set(http::field::accept,"application/json");
        req.set("X-API-Key",settings().getApiKey());

        http::write(stream, req);

        beast::flat_buffer buffer;
        http::response<http::string_body> res;
        http::read(stream, buffer, res);
        return res.body();

    } catch (const std::exception &ex) {
        qDebug()<<"ERROR "<<ex.what();
    } catch(...){
        qDebug()<<"ERROR ...";
    }
    return "";

}
NetworkAccess::NetworkAccess() : ctx(ssl::context::tlsv12_client)
{
    QFile file(":/source/cacert.pem");

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Не удалось открыть cacert.pem из ресурсов";
        return;
    }

    QByteArray pem = file.readAll();
    file.close();


    boost::system::error_code ec;
    ctx.add_certificate_authority(
        boost::asio::buffer(pem.constData(), pem.size()), ec);

    if (ec) {
        qWarning() << "Ошибка загрузки сертификатов:" << QString::fromStdString(ec.message());
        return;
    }
    ctx.set_verify_mode(ssl::verify_peer);
}
