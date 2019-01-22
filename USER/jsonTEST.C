uint8_t yeelink_get(const char *device_id,const char *sensors_id,char *value)
{
        int ret;
        char* presult;
        char remote_server[] = "api.yeelink.net";
        char str_tmp[128] = {0};
        // 请求缓冲区和响应缓冲区
        static char http_request[DATA_BUF_SIZE] = {0};        //声明为静态变量，防止堆栈溢出
        static char http_response[DATA_BUF_SIZE] = {0};        //声明为静态变量，防止堆栈溢出
        sprintf(str_tmp,"/v1.0/device/%s/sensor/%s/datapoints",device_id,sensors_id);
        // 确定 HTTP请求首部
        // 例如POST /v1.0/device/98d19569e0474e9abf6f075b8b5876b9/1/1/datapoints/add HTTP/1.1\r\n
        sprintf( http_request , "GET %s HTTP/1.1\r\n",str_tmp);
        // 增加属性 例如 Host: api.machtalk.net\r\n
        sprintf( str_tmp , "Host:%s\r\n" , remote_server);
        strcat( http_request , str_tmp);
 
        // 增加密码 例如 APIKey: d8a605daa5f4c8a3ad086151686dce64
        sprintf( str_tmp , "U-ApiKey:%s\r\n" , "d8a605daa5f4c8a3ad086151686dce64");//需要替换为自己的APIKey
        strcat( http_request , str_tmp);
        //
        strcat( http_request , "Accept: */*\r\n");
        // 增加表单编码格式 Content-Type:application/x-www-form-urlencoded\r\n
        strcat( http_request , "Content-Type: application/x-www-form-urlencoded\r\n");
        strcat( http_request , "Connection: close\r\n");
        // HTTP首部和HTTP内容 分隔部分
        strcat( http_request , "\r\n");
         
        //将数据通过TCP发送出去
        //新建一个Socket并绑定本地端口5000
        ret = socket(SOCK_TCPS,Sn_MR_TCP,5000,0x00);
        if(ret != SOCK_TCPS){
                printf("%d:Socket Error\r\n",SOCK_TCPS);
                while(1);
        }
        //连接TCP服务器
        ret = connect(SOCK_TCPS,domain_ip,80);
        if(ret != SOCK_OK){
                printf("%d:Socket Connect Error\r\n",SOCK_TCPS);
                while(1);
        }        
        //发送请求
        ret = send(SOCK_TCPS,(unsigned char *)http_request,strlen(http_request));
        if(ret != strlen(http_request)){
                printf("%d:Socket Send Error\r\n",SOCK_TCPS);
                while(1);
        }
 
        // 获得响应
        ret = recv(SOCK_TCPS,(unsigned char *)http_response,DATA_BUF_SIZE);
        if(ret <= 0){
                printf("%d:Socket Get Error\r\n",SOCK_TCPS);
                while(1);
        }
        http_response[ret] = '\0';
        //判断是否收到HTTP OK
        presult = strstr( (const char *)http_response , (const char *)"200 OK\r\n");
        if( presult != NULL ){
                static char strTmp[DATA_BUF_SIZE]={0};//声明为静态变量，防止堆栈溢出
                sscanf(http_response,"%*[^{]{%[^}]",strTmp);
                //提取返回信息
                char timestamp[64]={0};
                char timestampTmp[64]={0};
                char valueTmp[64]={0};
                sscanf(strTmp,"%[^,],%[^,]",timestampTmp,valueTmp);
                strncpy(timestamp,strstr(timestampTmp,":")+2,strlen(strstr(timestampTmp,":"))-3);
                strncpy(value,strstr(valueTmp,":")+1,strlen(strstr(valueTmp,":"))-1);
        }else{
                printf("Http Response Error\r\n");
                printf("%s",http_response);
        }
        close(SOCK_TCPS);
        return 0;
}