# const库
```cpp
#pragma once
#include <boost/beast/http.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <memory>
#include <iostream>
#include <unordered_map>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
//#include "Singleton.h"
#include <assert.h>
#include <queue>
#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/exception.h>
#include <iostream>
#include <functional>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <string>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

enum ErrorCodes {
	Success = 0,
	Error_Json = 1001,  //Json解析错误
	RPCFailed = 1002,  //RPC请求错误
	VarifyExpired = 1003, //验证码过期
	VarifyCodeErr = 1004, //验证码错误
	UserExist = 1005,       //用户已经存在
	PasswdErr = 1006,    //密码错误
	EmailNotMatch = 1007,  //邮箱不匹配
	PasswdUpFailed = 1008,  //更新密码失败
	PasswdInvalid = 1009,   //密码更新失败
	TokenInvalid = 1010,   //Token失效
	UidInvalid = 1011,  //uid无效
};


// Defer
class Defer {
public:
	// 接受一个lambda表达式或者函数指针
	Defer(std::function<void()> func) : func_(func) {}

	// 析构函数中执行传入的函数
	~Defer() {
		func_();
	}

private:
	std::function<void()> func_;
};
enum{MAX_LENGTH=1024};
#define CODEPREFIX  "code_"
```
# 同步读取客户端
```cpp
#include <iostream>
#define MAX_LENGTH 1024
#include"boost/asio.hpp"
#include<string>
using tcp = boost::asio::ip::tcp;
using address = boost::asio::ip::address;
int main() {
	try {
		boost::asio::io_context ioc;
		tcp::socket sock(ioc);
		tcp::endpoint  remote_ep(address::from_string("127.0.0.1"), 10086);
		boost::system::error_code err;
		sock.connect(remote_ep, err);
		if (err) {
			std::cout << "connect failed, code is " << err.value() << " error msg is " << err.message();
			return 0;
		}
		std::cout << "pl inout message" << std::endl;
		char request[MAX_LENGTH];
		std::cin.getline(request, MAX_LENGTH);
		size_t request_length = strlen(request);
		boost::asio::write(sock, boost::asio::buffer(request, request_length));
		char reply[MAX_LENGTH];
		size_t reply_length = boost::asio::read(sock,
			boost::asio::buffer(reply, request_length));
		std::cout << "Reply is: ";
		std::cout.write(reply, reply_length);
		std::cout << "\n";
	}

	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}
return 0;
}
```
# 异步读取服务端

# #server.cpp

```cpp
#include "server.h"

#include"boost/asio.hpp"


server::server(boost::asio::io_context& io_context, unsigned short port):_ioc(io_context),_acceptor(io_context,tcp::endpoint(tcp::v4(),port))
{		
	start_accept();
}

void server::start_accept()
{		
	Session* session = new Session(_ioc);
	_acceptor.async_accept(session->Socket(), std::bind(&server::handaccept, this, session, std::placeholders::_2));
	
}

void server::handaccept(Session* session,boost::system::error_code err)
{
	if (!err) {
		session->start();
	}
	else {
		delete session;
	}
	start_accept();
}
```
##server.h
```cpp
#pragma once
#include"boost/asio.hpp"
#include"const.h"
#include "Session.h"

class server {
public:
	server(boost::asio::io_context& io_context, unsigned short port);
private:
	void start_accept();
	void handaccept(Session* session, boost::system::error_code err);
	boost::asio::io_context& _ioc;
	tcp::acceptor _acceptor;
};
```
# #Session.cpp
```cpp
#include "session.h"
#include"const.h"

void Session::start() {
	memset(_data, 0, max_length);
	_socket.async_read_some(boost::asio::buffer(_data, max_length),
		std::bind(&Session::handread, this, std::placeholders::_1,
			std::placeholders::_2)
	);
}

void Session::handread(boost::system::error_code err, size_t byte_transf)
{
	if (!err) {
		std::cout << "receive data is" << _data << std::endl;
		boost::asio::async_write_some(boost::asio::buffer(_data, max_length), std::bind(&Session::handwrite, this, std::placeholders::_1,
			std::placeholders::_2)
		);
	}
	else
	{
		delete this;
	}
}

void Session::handwrite(boost::system::error_code err, size_t byte_transf)
{
	if (!err) {
		memset(_data, 0, max_length);
		_socket.async_read_some(boost::asio::buffer(_data, max_length), std::bind(&Session::handread, this, std::placeholders::_1,
			std::placeholders::_2)
		);
	}
	else
	{
		delete this;
	}
}
```
# #Session.h
```cpp
#pragma once
#include"const.h"

class Session {
public:
	Session(boost::asio::io_context& ioc):_socket(ioc){}
	tcp::socket& Socket() {
		return _socket;
	}
	void start();
private:
	void handread(boost::system::error_code err,size_t byte_transf);
	void handwrite(boost::system::error_code err, size_t byte_transf);
	tcp::socket _socket;
	enum {max_length=1024};
	char _data[max_length];

};
```
# #myserver.cpp
```cpp


#include <iostream>
#include"const.h"
#include"server.h"
#include"Session.h"
int main()
{
	try {
		boost::asio::io_context ioc;
		server s1(ioc, 3333);
		ioc.run();
	}
	catch (std::exception& e) {
		std::cout << "exception is" << e.what() << std::endl;
		
	}
}
```



