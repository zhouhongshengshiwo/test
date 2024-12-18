```c++
int main()
{
    try {
        boost::asio::io_context  io_context;
        boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
        //创建一个信号集，绑定到io_context。这里监听的信号是
        //SIGINT（通常是Ctrl+C产生的中断信号）和SIGTERM（终止信号）。这意味着当程序接收到这些信号时，会触发相应的操作。
        signals.async_wait([&io_context](auto, auto) {//注册一个异步等待的回调函数。当接收到指定的信号(上面绑定的信号集)时，这个函数会被调用
            io_context.stop();
            });
        CServer s(io_context, 10086);
        io_context.run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << endl;
    }
}
```
异步等待信号集，不会阻塞进程