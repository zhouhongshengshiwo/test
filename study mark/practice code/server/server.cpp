#include "server.h"

#include"boost/asio.hpp"

void CServer::ClearSession(std::string uuid) {
    _sessions.erase(uuid);
}
CServer::CServer(boost::asio::io_context& io_context, short port):_ioc(io_context),_acceptor(io_context,tcp::endpoint(tcp::v4(),port))
{		
	std::cout << "server start success,on port" << port << std::endl;
	StartAccept();
}

void CServer::StartAccept() {
    shared_ptr<CSession> new_session = make_shared<CSession>(_ioc, this);//后面的智能指针保证startAccept结束new_session不被销毁,
    //new_session生命周期与异步回调函数一致,保证异步回调执行完成
    _acceptor.async_accept(new_session->Socket(), std::bind(&CServer::HandleAccept, this, new_session, placeholders::_1));
    //bind绑定之后，new_session的智能指针引用计数加一,保证new_session生命周期延长
    //如果将智能指针作为参数绑定给函数对象，那么智能指针就以值的方式被新函数对象使用，那么智能指针的生命周期将和新生成的函数对象一致
}

void CServer::HandleAccept(shared_ptr<CSession> new_session, const boost::system::error_code& error) {
    if (!error) {
        new_session->start();
        _sessions.insert(make_pair(new_session->Getuuid(), new_session));
    }
    else {
        cout << "session accept failed, error is " << error.what() << endl;
    }

    StartAccept();
}
