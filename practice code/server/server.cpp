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
    shared_ptr<CSession> new_session = make_shared<CSession>(_ioc, this);//���������ָ�뱣֤startAccept����new_session��������,
    //new_session�����������첽�ص�����һ��,��֤�첽�ص�ִ�����
    _acceptor.async_accept(new_session->Socket(), std::bind(&CServer::HandleAccept, this, new_session, placeholders::_1));
    //bind��֮��new_session������ָ�����ü�����һ,��֤new_session���������ӳ�
    //���������ָ����Ϊ�����󶨸�����������ô����ָ�����ֵ�ķ�ʽ���º�������ʹ�ã���ô����ָ����������ڽ��������ɵĺ�������һ��
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
