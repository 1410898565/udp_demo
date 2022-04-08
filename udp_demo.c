
void udp_testTask(void *p)
{
    int nRet = -1;
    int socketFd = -1;

    struct in_addr addr 		= {0};
    struct sockaddr_in ser_addr = {0}; //是用于指定对方(目的主机)信息
    struct sockaddr_in loc_addr = {0}; //可以用来指定一些本地的信息，比如指定端口进行通信，而不是让系统随机分配
    socklen_t ser_addr_len = 0,
	socklen_t loc_addr_len = 0;
		
    char sendBuff[32] = {0};

    //配置服务器信息
    bzero(&ser_addr, sizeof(ser_addr));
    ser_addr.sin_family 		= AF_INET;                                //设置为IPV4通信
    ser_addr.sin_addr.s_addr	= inet_addr("192.168.0.151");                //设置目的ip
    ser_addr.sin_port 			= htons(1235);            //设置目的端口去链接服务器
    ser_addr_len 				= sizeof(ser_addr);

    socketFd = socket(AF_INET, SOCK_DGRAM, 0);                        //设置UDP报文传输    0表示默认    SOCK_DGRAM 默认使用UDP
    //其中第三位 0 是调用方式标志位，设置socket通方式，比如非阻塞
    if(socketFd<0)
    {    	
	    ESP_LOGI(TAG, "socket create failure");
        goto iExit;
    }

    while(1)
    {

		memset(sendBuff, 0, sizeof(sendBuff));
		memcpy(sendBuff, "hello", 5);
		
        sendto(socketFd, sendBuff, sizeof(sendBuff), 0, (struct sockaddr*)&ser_addr, ser_addr_len);

		memset(sendBuff, 0, sizeof(sendBuff));
        nRet = recvfrom(socketFd, sendBuff, sizeof(sendBuff),0,(struct sockaddr*)&loc_addr,&loc_addr_len);
        if (nRet==-1)
        {			
			ESP_LOGI(TAG, "receive data failure");
            continue;
        }
        addr.s_addr = loc_addr.sin_addr.s_addr;
		
		ESP_LOGI(TAG, "Receive info: %s from %s %d\n", sendBuff,inet_ntoa(addr),loc_addr.sin_port);
    }

	if(socketFd>0)
	    close(sk);

iExit:
	//不会走到这里，走到这里需要删除task
	vTaskDelete(NULL);
    return;
}
