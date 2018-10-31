#include <stdio.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
 
int _tmain(int argc, _TCHAR* argv[])
{
    WSADATA wsaData;
    int port = 5099;
    char buf[] = "服务器: 欢迎登录......\n";
 
    // 加载套接字
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
         printf("加载套接字失败：%d......\n", WSAGetLastError());
         return 1;
    }
 
    // socket()
    SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);
 
    // 初始化IP和端口信息
    SOCKADDR_IN addrSrv;
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(port); // 1024以上的端口号
    addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
 
    // bind()
    if (bind(sockSrv, (LPSOCKADDR)&addrSrv, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
    {
         printf("套接字绑定失败：%d......\n", WSAGetLastError());
         return 1;
    }
 
    // listen()
    if (listen(sockSrv, 10) == SOCKET_ERROR){
         printf("套接字监听失败：%d......\n", WSAGetLastError());
         return 1;
    }
 
    // 客户端信息
    SOCKADDR_IN addrClient;
    int len = sizeof(SOCKADDR);
 
    // 开始监听
    printf("服务端启动成功......开始监听...\n");
    while (1)
    {
         // 等待客户请求到来  
         SOCKET sockConn = accept(sockSrv, (SOCKADDR *)&addrClient, &len);
         if (sockConn == SOCKET_ERROR){
             printf("建立连接失败：%d......\n", WSAGetLastError());
             break;
         }
 
         printf("与客户端建立连接......IP：[%s]\n", inet_ntoa(addrClient.sin_addr));
 
         // 发送数据
         if (send(sockConn, buf, sizeof(buf), 0) == SOCKET_ERROR){
             printf("发送数据失败......\n");
             break;
         }
 
         char recvBuf[100];
         memset(recvBuf, 0, sizeof(recvBuf));
         // 接收数据
         recv(sockConn, recvBuf, sizeof(recvBuf), 0);
         printf("收到数据：%s\n", recvBuf);
 
         closesocket(sockConn);
    }
 
    // 关闭套接字
    closesocket(sockSrv);
    WSACleanup();
    system("pause");
 
    return 0;
}
