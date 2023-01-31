//
// Created by xcbosa on 2023/1/31.
//

#pragma once
#include "utils-private.h"

namespace xc {
    namespace utils {

        class CommandLineWorker;

        class CommandLineCommand {
            friend CommandLineWorker;
        public:
            CommandLineCommand(string name, string usage, string message);
            virtual void evaluate(string userInputLine) const = 0;
        protected:
            string commandName;
            string commandUsage;
            string message;
        };

        class CommandLineStringCommand: public CommandLineCommand{
        public:
            CommandLineStringCommand(string commandName, string commandUsage, string message, int argCnt, function<void (vector<string>)> handler);
            void evaluate(string userInputLine) const override;
        private:
            int argCnt;
            function<void (vector<string>)> handler;
        };

        typedef CommandLineStringCommand strcmd;

        class CommandLineWorker {
        public:
            void workerLoop();
        };

    } // xc
} // utils
