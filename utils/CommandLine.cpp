//
// Created by xcbosa on 2023/1/31.
//

#include "CommandLine.h"
#include "strop.h"
#include <unistd.h>

namespace xc {
    namespace utils {

        static CommandLineCommand *registeredCommands[1024];
        static int registeredCommandsId(0);

        CommandLineCommand::CommandLineCommand(string name, string usage, string message) {
            this->commandName = name;
            this->commandUsage = usage;
            this->message = message;
            assert(registeredCommandsId < 1024);
            registeredCommands[registeredCommandsId++] = this;
            cout << "[CommandLineCommand] Registered " << commandName << endl;
        }

        CommandLineStringCommand::CommandLineStringCommand(string commandName, string commandUsage, string message, int argCnt, function<void (vector<string>)> handler):
                CommandLineCommand(commandName, commandUsage, message) {
            this->argCnt = argCnt;
            this->handler = handler;
        }

        void CommandLineStringCommand::evaluate(string userInputLine) const {
            auto list = split(userInputLine, " ");
            vector<string> result;
            for (int i = 1; i < list.size(); i++) {
                string copied = list[i];
                trim(copied);
                result.push_back(copied);
            }
            if (result.size() != this->argCnt) {
                cerr << "Usage: " << this->commandUsage << endl;
            } else {
                this->handler(result);
            }
        }

        void CommandLineWorker::processCommand(string cmd) {
            auto list = split(cmd, " ");
            string titleUppercase = uppercase(list[0]);
            if (titleUppercase == "HELP") {
                cout << "Frp-WebUI Command List" << endl;
                for (int i = 0; i < registeredCommandsId; i++) {
                    cout << registeredCommands[i]->message << " : " << registeredCommands[i]->commandUsage << endl;
                }
            } else {
                bool founded = false;
                for (int i = 0; i < registeredCommandsId; i++) {
                    auto command = registeredCommands[i];
                    if (uppercase(command->commandName) == titleUppercase) {
                        command->evaluate(cmd);
                        founded = true;
                        break;
                    }
                }
                if (!founded) {
                    cerr << "Command " << list[0] << " not founded, type help to view help." << endl;
                }
            }
        }

        void CommandLineWorker::workerLoop() {
            char cinReadBuff[1024];
            while (true) {
                bzero(cinReadBuff, sizeof(cinReadBuff));
                cin.getline(cinReadBuff, sizeof(cinReadBuff));
                if (cin.eof() || cin.bad() || cin.fail()) {
                    cout << "Frp-WebUI interactive command disabled during stdin is closed." << endl;
                    return;
                }
                string str(cinReadBuff);
                if (str.empty()) continue;
                this->processCommand(str);
                cin.clear();
                usleep(1000 * 10);
            }
        }

    } // xc
} // utils