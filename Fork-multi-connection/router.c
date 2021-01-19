#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <unistd.h> // for close

int main()
{
    char server_message[256] = "已連接";
    char buf[256];

    // 建立socket
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0); //TCP連線

    // 指定socket的ip
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;   //通訊協定走internet
    server_address.sin_port = htons(9488); //指定port
    server_address.sin_addr.s_addr = INADDR_ANY;

    // 綁定socket 到特定ip/port
    bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));

    //監聽
    listen(server_socket, 5);

    int client_socket;
    while (1)
    {
        client_socket = accept(server_socket, NULL, NULL);
        //當連接失敗
        if (client_socket < 0)
        {
            perror("----accept----fail\n"); //印錯誤訊息
            close(server_socket);
            exit(0); //關閉程式
        }

        pid_t pid = fork();
        //子程式
        if (pid == 0)
        {
            //子程式已經獲得server_socket,所以可以先關掉
            close(server_socket);
            while (1)
            {
                ssize_t s = read(client_socket, buf, sizeof(buf) - 1);
                if (s > 0)
                {
                    buf[s] = 0;
                    printf("子程式： %s \n", buf);
                    write(client_socket, buf, strlen(buf));
                }
                else if (s == 0)
                {
                    printf("client quit. \n");
                    break;
                }
                else
                    break;
            }
            exit(0);
        }
        // 父程式
        else if (pid > 0)
        {
            close(client_socket); //子程式已經獲得server_socket,所以可以先關掉
            if (fork() > 0)
            {
                exit(0);
            }
        }

        // 傳送資料
        send(client_socket, server_message, sizeof(server_message), 0);
    }

    // 關閉socket
    close(server_socket);
    return 0;
}

/*常用command
**sudo lsof -i:port 查佔用port
*/