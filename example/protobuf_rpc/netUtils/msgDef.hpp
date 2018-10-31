struct Massage
{
    int nID;
    char strMsg[64];
};

void sendMsg()
{
   // 结构体消息
    Massage stMsg;
    memset(stMsg.strMsg, 0, sizeof(stMsg.strMsg));
    stMsg.nID = 1001;
    strcpy(stMsg.strMsg, "Struct string");
    // ...
    sendto(sockClient, (char*)&stMsg, sizeof(stMsg) + 1, 0, (SOCKADDR*)&addrClnt, nLen);
}