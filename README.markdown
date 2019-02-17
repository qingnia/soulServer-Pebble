# 一个未完成卡牌游戏的代码备份

----------
## 简介

根据腾讯的pebble扩展出来的游戏服务器，逻辑为一个卡牌server的demo

## pebble用法

协议细分为三部分，head内包含长度，协议路由（rpc协议号）和version，thrift把protobuf包装起来，实际protobuf才是逻辑需要关心的消息体
协议层|内容
-- | --
1|head
2|thrift
3|protobuf

## 代码逻辑

逻辑代码放在example/protobuf_rpc/目录下
目录结构|作用
--|--
main.cpp|入口函数，分成两个线程，逻辑和网络
net|网络工具，协议定义在proto文件，rpcMsg.cpp是协议层处理，调用gameMgr，singleServer是原本的示例代码，做了包装
gameEntity|逻辑代码
utils|写逻辑时顺便造的或者网上找的轮子