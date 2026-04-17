#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h> // الغرض من هذه هي توفير التعريفات اللازمة لإنشاء واستخدام السوكيتات في نظام التشغيل. تحتوي هذه المكتبة على الدوال والأنواع المستخدمة للتعامل مع الشبكات، مثل إنشاء السوكيت، الربط، الاستماع، والاتصال.

#include <netinet/in.h> // هذه المكتبة تحتوي على التعريفات اللازمة للتعامل مع عناوين الإنترنت (IP) والبروتوكولات المتعلقة بها. توفر أنواع البيانات والهياكل المستخدمة لتمثيل عناوين IP، المنافذ، والبروتوكولات المختلفة مثل TCP وUDP.

#include <arpa/inet.h>

int main(void)
{
    int socket_fd_number;

    struct sockaddr_in server_address;
    socket_fd_number = socket(AF_INET, SOCK_STREAM, 0);
if (socket_fd_number<0)
{
    perror("|Socket Failed| 🔴");
    exit(EXIT_FAILURE);
}

    printf("|Socket Created| 🟢 \n|The FD Number is: %d|🔵\n", socket_fd_number);
    return 0;
}