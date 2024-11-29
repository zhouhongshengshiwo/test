#pragma once
#include"boost/asio.hpp"
#include "Session.h"
#include<map>
#include <memory>
#include<iostream>
using namespace std;
class CSession;
class CServer
{
public:
    CServer(boost::asio::io_context& io_context, short port);
    void ClearSession(std::string uuid);
private:
    void HandleAccept(shared_ptr<CSession> new_session, const boost::system::error_code& error);
    void StartAccept();
    boost::asio::io_context& _ioc;
    short _port;
    boost::asio::ip::tcp::acceptor _acceptor;
    std::map<std::string, shared_ptr<CSession>> _sessions;
};