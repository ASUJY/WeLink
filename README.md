# WeLink
基于 C++ / Qt 开发的桌面即时通讯客户端  
![Language](https://img.shields.io/badge/language-C++-blue.svg)
![Qt](https://img.shields.io/badge/Qt-6.9-green.svg)
![Build](https://img.shields.io/badge/build-CMake-orange.svg)
![SQLite](https://img.shields.io/badge/db-SQLite-lightblue.svg)

<br/>

# 快速开始
- 下载项目中的 `WeLink-Window64.zip`，解压之后直接点击执行WeLink.exe，进行登录注册。 
- `WeLink-MacOSArm64.zip`是MacOS版本


<br/>

# 模块说明
|   模块名称    |                                              模块用途                                              |
| :-----------: | :------------------------------------------------------------------------------------------------: |
|      app      |       程序入口 + 业务调度中枢，串联网络、数据库、界面三大模块，处理登录、聊天整体业务逻辑。        |
|    DBMagr     | 统一封装所有 SQLite 数据库操作，隔离业务代码与原生 QtSql API，提供对数据的增删改查，全局单例调用。 |
|    common     |                                  全局常量、通用工具函数、基础封装                                  |
|     model     |                              定义项目所有数据实体结构体，统一数据格式                              |
|      net      |           封装 Qt TcpSocket，实现客户端与 IM 服务端双向通信，负责消息收发、字节流处理。            |
| ui_components |                         全部界面组件，自定义 UI 控件：气泡、输入框、窗口等                         |

# 已实现功能
> 需要服务端配合的功能，实际使用还取决于服务端是否在线并遵循相同通信协议。

[登录模块](doc/login.md)  
[注册模块](doc/register.md)  
[主窗口与左侧导航模块](doc/mainwindow.md)  
[聊天模块](doc/chat.md)  
[联系人与好友申请模块](doc/contacts.md)  
[网络通信模块](doc/network.md)  
[数据库模块](doc/db.md)  

# 开发计划
- [x] 联系人列表中的数据保存到本地数据库中
  - [x] 新的朋友列表中的数据保存到本地数据库中
  - [x] 联系人列表中的数据保存到本地数据库中
  - [x] 登录的时候从数据库中读取联系人列表数据到界面中
- [x] 修复添加好友窗口重复打开会导致软件崩溃的bug 
- [x] 如果当前聊天窗口是朋友A的聊天窗口，则朋友A发送消息则直接显示到聊天窗口中，否则，则在聊天列表的头像中显示未读消息数量
- [x] 优化聊天窗口中的聊天信息，考虑是直接用model/view模式来重构还是继续使用QListWidget，但是每次打开窗口不重新刷新所有数据
  - [ ] 使用model/view模式来重构聊天消息
  - [ ] 新增每条聊天消息都会显示聊天消息时间的功能
- [x] 聊天面板中的搜索框新增输入内容限制长度，点击x按钮清除内容功能
- [x] 聊天面板中，修改列表滚动时，滚动条的样式
- [x] 聊天面板中，增加显示效果，当鼠标在列表上悬浮的时候会显示不同颜色，选中会显示不同颜色 
- [ ] 考虑主界面中是否使用QMainWindow原生的标题栏，但是需要修改标题栏的颜色以及去掉标题栏的图标
- [ ] 登录的时候获取服务器返回的好友申请信息，联系人列表信息（在新设备的时候）
- [ ] 聊天消息长度有限制，怎么解决？
- [x] 主界面由自定义关闭按钮，最小化按钮，最大化按钮改为使用QMainWindow自带标题栏

# 功能展示
![](doc/images/0.png)

![](doc/images/1.png)

![](doc/images/2.png)

![](doc/images/3.png)

![](doc/images/4.png)

![](doc/images/5.png)


