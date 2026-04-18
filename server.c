#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h> // الغرض من هذه هي توفير التعريفات اللازمة لإنشاء واستخدام السوكيتات في نظام التشغيل. تحتوي هذه المكتبة على الدوال والأنواع المستخدمة للتعامل مع الشبكات، مثل إنشاء السوكيت، الربط، الاستماع، والاتصال.

#include <netinet/in.h> // هذه المكتبة تحتوي على التعريفات اللازمة للتعامل مع عناوين الإنترنت (IP) والبروتوكولات المتعلقة بها. توفر أنواع البيانات والهياكل المستخدمة لتمثيل عناوين IP، المنافذ، والبروتوكولات المختلفة مثل TCP وUDP.

#include <arpa/inet.h>


void SEND_HTML_FILE(int client_socket);
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
    int opt = 1;

    setsockopt(socket_fd_number, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // The Second Step: Bind the Socket to an IP + Port
    memset(&server_address, 0, sizeof(server_address));            // تنظيف الهيكل
    server_address.sin_family = AF_INET;                           // IPv4
    server_address.sin_addr.s_addr = inet_addr("192.168.198.149"); // أي عنوان IP متاح
    server_address.sin_port = htons(2222);                         // Port 2222 (Network Byte Order)

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
        sleep(20); // تأخير لمدة 20 ثانية قبل قبول الاتصالات الجديدة (لأغراض الاختبار)

        if (client_socket < 0)
        {
            perror("|Accept Failed| 🔴");
            continue; // استمر في قبول الاتصالات الأخرى حتى لو فشل هذا الاتصال
        }
        printf("|Client Connected| 🟢 \n|Client IP: %s: %d|🔵\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

        char buffer[1024] = {0};
        char response[1024] = {0};
        strcpy(response, "Welcome user to my own server in c language from scratch! 👋\n");
        read(client_socket, buffer, sizeof(buffer));
        printf("Client Says: %s\n", buffer);
        if (strstr(buffer, "GET / HTTP/1.1") != NULL)
        {
            SEND_HTML_FILE(client_socket);
        }
        else if (strstr(buffer, "Y:w") != NULL)
        {
            write(client_socket, response, strlen(response));
        }
        else
        {
            write(client_socket, "HTTP/1.1 404 Not Found\r\n\r\n", 26);
        }

        close(client_socket); // إغلاق اتصال العميل بعد التعامل معه
    }

    return 0;
}
void SEND_HTML_FILE(int client_socket)
{

    // الخطوة الاولى: فتح ملف HTML
    FILE *html_file = fopen("welcome.html", "r");
    if (html_file == NULL)
    {
        write(client_socket, "HTTP/1.1 404 Not Found\r\n\r\n", 26);
        return;
    }
    // الخطوة الثانية: قراءة محتوى الملف
    char html_content[1024]; // اي مايعادل 1KB
    fread(html_content, sizeof(char), sizeof(html_content) - 1, html_file);

    fseek(html_file, 0, SEEK_END);
    long file_size = ftell(html_file);
    html_content[file_size] = '\0'; // تأكد من أن المحتوى ينتهي بمحرف نهاية السلسلة

    // الخطوة الثالثة: إرسال استجابة HTTP مع المحتوى
    char header[512];
    snprintf(header, sizeof(header),
             "HTTP/1.1 200 OK\r\n"
             "Content-Length: %ld\r\n"
             "Content-Type: text/html; charset=utf-8\r\n"
             "connection: close\r\n\r\n",
             file_size);
    write(client_socket, header, strlen(header));
    write(client_socket, html_content, file_size);

    // الخطوة الرابعة: إغلاق الملف
    fclose(html_file);
}
