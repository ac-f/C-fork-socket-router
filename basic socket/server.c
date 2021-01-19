#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <unistd.h> // for close

int main()
{
    char server_message[256] = "已連接";

    // 建立socket
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0); //TCP連線

    // 指定socket的ip
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;   //通訊協定走internet
    server_address.sin_port = htons(9487); //指定port
    server_address.sin_addr.s_addr = INADDR_ANY;

    // 綁定socket 到特定ip/port
    bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));

    //監聽
    listen(server_socket, 5);

    int client_socket;
    client_socket = accept(server_socket, NULL, NULL);

    // 傳送資料
    send(client_socket, server_message, sizeof(server_message), 0);

    // 關閉socket
    close(server_socket);
    return 0;
}