#pragma once
#include"server.h"
#include <memory>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include<queue>
#define MAX_LENGTH 1024*2
#define HEAD_LENGTH 2
using namespace std;
using tcp = boost::asio::ip::tcp;
class CServer;
class MsgNode
{
    friend class CSession;
public:
    MsgNode(char* msg, short max_len) :_total_len(max_len + HEAD_LENGTH), _cur_len(0) {
        _data = new char[_total_len + 1]();//����һ���ֽڴ��/0
        memcpy(_data, &max_len, HEAD_LENGTH);//�� max_len ��ֵ������ _data �Ŀ�ͷ�����ڴ洢��Ϣ�ĳ�����Ϣ��
        memcpy(_data + HEAD_LENGTH, msg, max_len);//����Ϣ���� msg ���Ƶ� _data �У�ͷ������֮���λ�á�
        _data[_total_len] = '\0';
    }

    MsgNode(short max_len) :_total_len(max_len), _cur_len(0) {
        _data = new char[_total_len + 1]();
    }

    ~MsgNode() {
        delete[] _data;
    }

    void Clear() {
        ::memset(_data, 0, _total_len);
        _cur_len = 0;
    }
private:
    short _cur_len;
    short _total_len;
    char* _data;
};

class CSession :public std::enable_shared_from_this<CSession>
{
public:
	CSession(boost::asio::io_context& ioc, CServer* server1);
	tcp::socket& Socket() {
		return _socket;
	}
	std::string& Getuuid();
	void start();
    void Close();
	void Send(char* msg, int max_length);
    std::shared_ptr<CSession>SharedSelf();
private:
	void HandleRead(const boost::system::error_code& err,size_t byte_transf,shared_ptr<CSession> _self_shared);
	void HandleWrite(const boost::system::error_code& err,shared_ptr<CSession> _self_shared);
	tcp::socket _socket;
	CServer* _server;
	std::string _uuid;
    bool _b_close;
	std::queue<shared_ptr<MsgNode>> _send_que; // ������Ϣ����
	std::mutex _send_lock;       // ������
	char _data[MAX_LENGTH];
    //�յ�����Ϣ�ṹ
    std::shared_ptr<MsgNode> _recv_msg_node;
    bool _b_head_parse;//��ʾͷ���Ƿ�������
    //�յ���ͷ���ṹ
    std::shared_ptr<MsgNode> _recv_head_node;
};
