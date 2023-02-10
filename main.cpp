#include <iostream>
#include <sys/stat.h>

#include "httpserver/http-server.h"
#include "utils/utils.h"
#include "processor/processor.h"
#include "fs.hpp"
#include "thirdparty/sha256.hpp"
#include "frp.h"

using namespace xc;
using namespace xc::httpserver;
using namespace xc::processor;

const static strcmd reg("reg", "reg <User> <Pwd>", "注册用户", 2, [] (auto args) {
    INI ini;
    ini.getMust("info")->set("password", sha256(args[1] + conf::userPasswordSalt));
    saveTextFile(conf::getUserDataDir() + "/" + args[0], ini.getINIString());
    cout << "成功创建用户 " << args[0] << endl;
});

const static strcmd createFrp("frp", "frp <Name> <Server-Addr> <Server-Port> <Token>", "创建Frpc配置文件", 4, [] (auto args) {
    frp::addProfile(args[0], args[1], args[2], args[3]);
    cout << "成功创建Frpc配置文件 " << args[0] << endl;
});

const static strcmd assign("assign", "assign <UserName> <Profile>", "使用户有权访问配置文件", 2, [] (auto args) {
    frp::ProfileInfo profile(args[1]);
    profile.addUser(args[0]);
    profile.save();
    cout << "成功将配置文件 " << args[1] << " 绑定到用户 " << args[0] << endl;
});

int call_shell(string cmd) {
    cout << "$ " << cmd << endl;
    ::system(cmd.c_str());
}

const static strcmd install("install", "install", "安装", 0, [] (auto args) {
    uid_t uid = getuid();
    if (setuid(0)) {
        cerr << "安装失败，无法获取root权限。" << endl;
        return;
    }

    char selfPath[1024];
    int len;
    if ((len = readlink("/proc/self/exe", selfPath, sizeof(selfPath))) <= 0) {
        cerr << "安装失败，无法获取自身位置。" << endl;
        return;
    }
    selfPath[len] = 0;
    string programPath(selfPath);

    replace_all(programPath, " ", "\\ ");
    string cpCmd = "cp " + programPath + " /usr/local/bin/fpw";
    if (call_shell(cpCmd)) {
        cerr << "安装失败，无法将自身拷贝到 /usr/local/bin/fpw。" << endl;
        return;
    }

    cout << "$ vi /usr/lib/systemd/system/fpw.service" << endl;

    INIFile systemd("/usr/lib/systemd/system/fpw.service");
    systemd.getMust("Unit")->set("Description", "FRPCWebUI Daemon");
    systemd.getMust("Unit")->set("After", "network.target");
    systemd.getMust("Service")->set("Type", "Simple");
    systemd.getMust("Service")->set("User", "root");
    systemd.getMust("Service")->set("Restart", "on-failure");
    systemd.getMust("Service")->set("RestartSec", "1s");
    systemd.getMust("Service")->set("ExecStart", "fpw");
    systemd.getMust("Service")->set("LimitNOFILE", "1048576");
    systemd.getMust("Install")->set("WantedBy", "multi-user.target");
    systemd.save();

    if (call_shell("systemctl daemon-reload")) {
        cerr << "安装失败，无法调用systemctl。" << endl;
        return;
    }
    if (call_shell("systemctl enable fpw")) {
        cerr << "安装失败，无法调用systemctl。" << endl;
        return;
    }
    if (call_shell("systemctl start fpw")) {
        cerr << "安装失败，无法调用systemctl。" << endl;
        return;
    }

    setuid(uid);
    cout << "安装成功。" << endl;
});

int main(int argc, char **argv) {
    std::cout << "FRPC WebUI HelloWorld!" << std::endl;
    CommandLineWorker cmdLine;

    conf::getRootDir();
    conf::getUserDataDir();
    conf::getFrpcDir();

    if (argc > 1) {
        ostringstream cmd;
        for (int i = 1; i < argc; i++) {
            cmd << argv[i];
            cmd << " ";
        }
        cmdLine.processCommand(cmd.str());
        return 0;
    }

    ostringstream oss;
    oss << "ps -ef | grep " << argv[0] << " | grep -v grep";
    ::FILE *psStdoutFd = popen(oss.str().c_str(), "r");
    ostringstream w_oss;
    while (true) {
        int ch = ::fgetc(psStdoutFd);
        if (ch == -1) break;
        w_oss << (char) ch;
    }
    ::pclose(psStdoutFd);
    if (utils::split(w_oss.str(), "\n").size() > 1) {
        cerr << "FrpcWebUI只能运行一个进程，您已启动了多个。" << endl;
        return -1;
    }

    if (fs::contentsOfDirectory(conf::getUserDataDir()).empty()) {
        cout << "看起来您没有设置任何一个账号，您可以输入 reg <账号> <密码> 来注册。" << endl;
    }

    HTTPServer server(8192);
    thread([&server] { server.serverLoop(); }).detach();

    RequestProcessWorker worker;
    thread([&worker] { worker.workerLoop(); }).detach();

    thread([] { frp::frpDaemon(); }).detach();

    cmdLine.workerLoop();

    while (true) {
        sleep(1);
    }
}
