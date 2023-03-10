# FRPC-WebUI-5MB
在5MB的内存 / 硬盘下，实现FRPC的管理界面。此项目实现了最简单的多线程HTTP服务器与单线程请求处理服务器，并在实现中尽可能的降低内存消耗。

此项目也包含一个非常有趣的C++模版引擎，可以去看看Controller目录。

![screen-shot](https://github.com/XCBOSA/frp-webui-5MB/blob/6879d14d89ef75c38c19e25954408eec5faf5433/screen-shot.jpeg)

## 支持的操作系统：
**Linux**（已在Ubuntu 22.04、PVE 7.3中测试）。如果您使用PVE，出于安全考虑推荐您将此项目部署在LXC容器中。

**macOS** (已在macOS Ventura中测试，支持x86_64和arm64)。

**Windows** (需要MinGW-w64环境)。未经测试。

## 这个项目可以：
1. 使用Web界面来配置frpc，您只需填写重要信息，支持TCP/UDP端口转发（不区分TCP/UDP，每个端口只能同时开放TCP/UDP）。
2. 管理frpc的生命周期，为每一个配置文件开启一个frpc进程，并在配置文件修改时自动更新，在配置文件删除时终止进程。
3. 您可为不同用户指定不同的配置文件，配置文件中包含了允许使用的端口范围和允许使用的用户列表。

## 这个项目不可以：
1. 仅转发TCP或UDP其中之一，我觉得这样做没有意义，当然欢迎PR来加入此选项。
2. 转发HTTP，这个项目目前仅支持TCP/UDP。

## 安装
如果您想修改任何配置，请修改 `webuiconf.h` 中的硬编码值，然后重新编译项目。我不认为使用配置文件是更友好的，我认为对于一个简单到不能再简单的小工具项目，使用复杂的配置文件只会增加项目的上手成本。
### 使用二进制文件
1. 下载frpc的最新版本可执行文件，并将frpc可执行文件拷贝到 `/usr/local/bin/frpc` 或 `/usr/bin/frpc` 。
2. 从 GitHub Release 中下载适用于您的平台的二进制文件，放置到任何地方，例如，您下载到 `~/fpw` 。
3. 执行 `chmod +x fpw`，赋予其可执行权限。
4. 执行 `sudo ./fpw install`，将此项目安装到 `/usr/local/bin` 中，并自动配置服务和开机启动。
5. 到目前为止，您已经成功的安装了FRPCWebUI。您可以删除下载的fpw文件，然后参考下一节配置账号与配置文件。
### 使用源代码编译
1. 下载frpc的最新版本可执行文件，并将frpc可执行文件拷贝到 `/usr/local/bin/frpc` 或 `/usr/bin/frpc` 。
2. 克隆此项目到您的服务器上
    ``` sh
    git clone https://github.com/XCBOSA/frp-webui-5MB.git
    cd frp-webui-5MB
    ```
3. 您可以修改webuiconf.h文件中的硬编码配置值，如果不需要修改请继续。注意：强烈建议您修改 `userPasswordSalt` 的值，这将保护用户的密码安全。
4. 编译
    ``` sh
    cmake .
    make -j64
    cp FRPCWebUI /usr/local/bin/fpw
    ```
5. 如果您想使用systemd，请执行 `fpw install` 来写入服务配置文件并配置自启动；否则，您可以使用任何工具，使得fpw开机启动。

### 配置账号与配置文件

配置文件是frpc程序使用的ini文件。此项目在frpc需要的配置条目基础上增加了一些用户权限、端口范围相关内容。此项目会为每个配置文件维护一个frpc进程。

1. 添加用户

    ```
    fpw reg <用户名> <密码>
    ```

2. 添加配置文件

    ```
    fpw frp <配置文件名字> <frps服务器地址> <frps服务器端口> <frps服务器Token>
    ```

3. 为用户指定其可以使用的配置文件

    ```
    fpw assign <用户名> <配置文件名字>
    ```

4. 在浏览器中登陆 服务器地址:8192 ，开始配置端口吧。
