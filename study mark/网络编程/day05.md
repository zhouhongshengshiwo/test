Protocol Buffers（简称 Protobuf）是一种轻便高效的序列化数据结构的协议，由 Google 开发。它可以用于将结构化数据序列化到二进制格式，并广泛用于数据存储、通信协议、配置文件等领域。 我们的逻辑是有类等抽象数据构成的，而tcp是面向字节流的，我们需要将类结构序列化为字符串来传输
# proto文件如何定义
.proto 文件用于定义数据结构和服务。以下是一些基本的定义要素和示例：

基本结构
一个 .proto 文件的基本结构通常包括：

    语法声明（syntax）：指定使用的 protobuf 语法版本。
    消息定义（message）：定义数据结构。
    字段定义（field）：为消息类型中的字段提供名称和类型。
    枚举定义（enum）：定义一组命名的常量值。
    服务定义（service）：定义 RPC 服务及其方法。
    示例
```cpp
// 指定使用的语法版本
syntax = "proto3";

// 定义一个消息
message Person {
    string name = 1;       // 字段名为 name，类型为 string，序号为 1
    int32 id = 2;         // 字段名为 id，类型为 int32，序号为 2
    string email = 3;     // 字段名为 email，类型为 string，序号为 3

    // 嵌套消息
    message PhoneNumber {
        string number = 1;  // 手机号
        PhoneType type = 2; // 类型
    }

    repeated PhoneNumber phone = 4; // 可以有多个电话号码

    // 定义枚举类型
    enum PhoneType {
        MOBILE = 0;  // 默认值
        HOME = 1;
        WORK = 2;
    }
}

// 定义一个服务
service AddressBook {
    rpc AddPerson(Person) returns (Person); // 定义一个 RPC 方法
}
```
# 关键点
字段类型：常见的字段类型包括 string、int32、float、bool 等。
字段序号：每个字段有一个唯一的序号，用于标识字段。这些序号在序列化和反序列化过程中非常重要。
重复字段（repeated）：表示字段可以出现多次，类似于数组。
嵌套消息：消息可以包含其他消息作为字段。
编译 .proto 文件
定义完 .proto 文件后，你可以使用 protoc 命令行工具编译它：
```cpp
protoc --cpp_out=. example.proto  // 生成 C++ 代码
protoc --python_out=. example.proto // 生成 Python 代码
```
我们修改服务器接收数据和发送数据的逻辑 当服务器收到数据后，完成切包处理后，将信息反序列化为具体要使用的结构,打印相关的信息，然后再发送给客户端
```cpp
    MsgData msgdata;//存储接收到的消息数据
    std::string receive_data;
    msgdata.ParseFromString(std::string(_recv_msg_node->_data, _recv_msg_node->_total_len));//解析后，msgdata 对象中现在包含了接收到的消息的信息。
    std::cout << "recevie msg id  is " << msgdata.id() << " msg data is " << msgdata.data() << endl;//输出接收到的消息
    std::string return_str = "server has received msg, msg data is " + msgdata.data();//建一个字符串 return_str，用于返回给客户端
    MsgData msgreturn;//用于返回给客户端
    msgreturn.set_id(msgdata.id());
    msgreturn.set_data(return_str);//并设置其 ID 和数据为刚构造的 return_str
    msgreturn.SerializeToString(&return_str);//将 msgreturn 对象序列化为字节流.并存储在 return_str
    Send(return_str);
```

序列化是将对象转换为字节流的过程
反序列化是将字节流转换回对象的过程

同样的道理，客户端在发送的时候也利用protobuf进行消息的序列化，然后发给服务器
```cpp
    MsgData msgdata;
    msgdata.set_id(1001);
    msgdata.set_data("hello world");
    std::string request;
    msgdata.SerializeToString(&request);
```