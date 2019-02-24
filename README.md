# 基于MSP430的PC授时定时器
### 功能
- 单片机自动定时
- PC网页端获取时间，并且向单片机授时
### 硬件使用
- 欣世纪电子MSP430F149最小系统板
- LCD1206液晶模块
- 串口线
### 使用
- 使用串口线连接PC机和430开发板
- 使用IAR-EW430编译src/cpp/Main.cpp和src/cpp/Config.h 生成txt文件
- 使用烧录软件烧录txt文件 开机后单片机开始定时
- 修改src/web/py/Main.py第4行的'COM5'为你连接的串口号
- 修改src/web/js/time/js第7和18行的python路径
- 用ie浏览器打开src/web/index.html即可获取单片机当前时间，以及为单片机设定时间
- 老杨 设计前端界面
- 老王 调试TimeA定时器 键盘消抖
- [月落](github.com/yueluo-x) LCD1602驱动 开发文档
