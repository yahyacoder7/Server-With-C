#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h> // الغرض من هذه هي توفير التعريفات اللازمة لإنشاء واستخدام السوكيتات في نظام التشغيل. تحتوي هذه المكتبة على الدوال والأنواع المستخدمة للتعامل مع الشبكات، مثل إنشاء السوكيت، الربط، الاستماع، والاتصال.

#include <netinet/in.h> // هذه المكتبة تحتوي على التعريفات اللازمة للتعامل مع عناوين الإنترنت (IP) والبروتوكولات المتعلقة بها. توفر أنواع البيانات والهياكل المستخدمة لتمثيل عناوين IP، المنافذ، والبروتوكولات المختلفة مثل TCP وUDP.

#include <arpa/inet.h>

int main(void)
{
    // The First Step: Create TCP Socket

    int socket_fd_number;              // File Descriptor للسوكيت
    struct sockaddr_in server_address; // هيكل يحتوي على IP + Port

    socket_fd_number = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd_number < 0)
    {
        perror("|Socket Failed| 🔴");
        close(socket_fd_number);
        exit(EXIT_FAILURE);
    }

    printf("|Socket Created| 🟢 \n|The FD Number is: %d|🔵\n", socket_fd_number);

    // The Second Step: Bind the Socket to an IP + Port
    memset(&server_address, 0, sizeof(server_address)); // تنظيف الهيكل
    server_address.sin_family = AF_INET;                // IPv4
    server_address.sin_addr.s_addr = INADDR_ANY;            // أي عنوان IP متاح
    server_address.sin_port = htons(2222); // Port 2222 (Network Byte Order)

    int bind_result = bind(socket_fd_number, (struct sockaddr *)&server_address, sizeof(server_address));
    if (bind_result < 0)
    {
        perror("|Bind Failed| 🔴");
        close(socket_fd_number);
        exit(EXIT_FAILURE);
    }
    printf("|Binding Socket to IP + Port| 🟢 \n|IP: %s|🔵\n|Port: %d|🔵\n", inet_ntoa(server_address.sin_addr), ntohs(server_address.sin_port));

    int listen_result = listen(socket_fd_number, 3); // Backlog =3
    if (listen_result < 0)
    {
        perror("|Listen Failed| 🔴");
        close(socket_fd_number);
        exit(EXIT_FAILURE);
    }
    printf("|Server is Listening| 🟢 \n|Port: %d|🔵\n", ntohs(server_address.sin_port));

    int clinet_fd;                                         // File Descriptor للعميل
    struct sockaddr_in client_address;                     // هيكل يحتوي على IP + Port للعميل
    socklen_t client_address_len = sizeof(client_address); // طول هيكل عنوان العميل
    while (1)
    {
        int client_socket = accept(socket_fd_number, (struct sockaddr *)&client_address, &client_address_len);
        if (client_socket < 0)
        {
            perror("|Accept Failed| 🔴");
            continue; // استمر في قبول الاتصالات الأخرى حتى لو فشل هذا الاتصال
        }
        printf("|Client Connected| 🟢 \n|Client IP: %s: %d|🔵\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

        // هنا يمكنك إضافة الكود للتعامل مع العميل (مثل قراءة البيانات، إرسال رد، إلخ)
        char buffer[1024] = {0};
        read(client_socket, buffer, sizeof(buffer));
        printf("Client said: %s\n", buffer);

        send(client_socket, "Hello from server\n", 18, 0);
        close(client_socket); // إغلاق اتصال العميل بعد التعامل معه
    }

    return 0;
}