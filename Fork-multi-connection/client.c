#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <unistd.h> // for close

int main()
{
    // 建立socket
    int network_socket;
    network_socket = socket(AF_INET, SOCK_STREAM, 0); //TCP連線

    // 指定socket的ip
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;   //通訊協定走internet
    server_address.sin_port = htons(9488); //指定port
    server_address.sin_addr.s_addr = INADDR_ANY;

    /** 
     * @brief 建立連線
     * @param socket
     * @param 經轉換過型別的地址
     * @param 地址的大小
     * @return 狀態，0為正常連線，負數則為連線錯誤
    */
    int connection_status = connect(network_socket, (struct sockaddr *)&server_address, sizeof(server_address));

    //當連線錯誤時
    if (connection_status == -1)
    {
        printf("There's something wrong of connection! \n\n");
        return 0;
    }

    char buf[1024];
    while (1)
    {
        printf("client > ");
        fflush(stdout);
        ssize_t sread = read(0, buf, sizeof(buf) - 1);
        if (sread > 0)
        {
            buf[sread - 1] = 0;
            write(network_socket, buf, strlen(buf));
            printf("From Server > %s\n", buf);
            read(network_socket, buf, sizeof(buf) - 1);
        }
    }
    //關閉socket 連接
    close(network_socket);
    return 0;
}