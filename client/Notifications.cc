//
// Created by shawn on 23-8-7.
//
#include "Notifications.h"
#include "../utils/proto.h"
#include "../utils/TCP.h"
#include "../utils/IO.h"
#include <iostream>
#include <thread>

using namespace std;

//实时通知
void announce(string UID) {
    int announce_fd = Socket();
    //直接连服务器，不断给服务器发送实时通知处理请求
    Connect(announce_fd, IP, PORT);
    string buf;
    int num;
    while (true) {
        //read::_for(chrono::seconds(3));

        sendMsg(announce_fd, NOTIFY);

        sendMsg(announce_fd, UID);

        recvMsg(announce_fd, buf);
        if (buf == REQUEST_NOTIFICATION) {
            cout << "您收到一条好友添加申请" << endl;
        }

        recvMsg(announce_fd, buf);
        if (buf != "NO") {
            cout << "收到一条来自" << buf << "的消息" << endl;
        }

        recvMsg(announce_fd, buf);
        num = stoi(buf);
        for (int i = 0; i < num; ++i) {

            recvMsg(announce_fd, buf);
            cout << "您已经被" << buf << "删除" << endl;
        }

        recvMsg(announce_fd, buf);
        num = stoi(buf);
        for (int i = 0; i < num; i++) {
            recvMsg(announce_fd, buf);

            cout << "您已经被设为" << buf << "的管理员" << endl;
        }

        recvMsg(announce_fd, buf);
        num = stoi(buf);
        for (int i = 0; i < num; i++) {
            recvMsg(announce_fd, buf);
            cout << "您已被取消" << buf << "的管理权限" << endl;
        }

        recvMsg(announce_fd, buf);
        num = stoi(buf);
        for (int i = 0; i < num; i++) {
            recvMsg(announce_fd, buf);
            cout << "收到" << buf << "发送的文件" << endl;
        }
    }
}
//现在开的线程全部不使用引用
//私聊群聊，接收对方发送的消息
void chatReceived(int fd, string UID) {
    Message message;
    string json_msg;
    while (true) {

        recvMsg(fd, json_msg);
        if (json_msg == EXIT) {
            break;
        }
        message.json_parse(json_msg);
        //私发
        if (message.getGroupName() == "1") {
            if (message.getUidFrom() == UID) {
                cout << message.getUsername() << ": " << message.getContent() << endl;
            } else {
                cout << "\t\t\t\t\t\t\t\t" << RED << "收到一条来自" << message.getUsername() << "的一条消息" << RESET
                     << endl;
            }
            //sm bug之前写成return了
            continue;
        }
        //群发
        if (message.getUidFrom() == UID) {
            cout << message.getUsername() << ": " << message.getContent() << endl;
        } else {
            cout << "\033[1m\033[31m"
                 << "           "
                 << "收到一条来自" << message.getUsername() << "的一条消息"
                 << "\033[0m" << endl;
            //cout << "收到一条来自" << message.getUsername() << "的一条消息" << endl;
        }

    }
}