#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/socket_example"

int main() {
    int client_socket;
    struct sockaddr_un server_address;

    // Создаем Unix domain socket
    client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Ошибка создания сокета");
        exit(EXIT_FAILURE);
    }

    // Устанавливаем параметры серверного адреса
    memset(&server_address, 0, sizeof(server_address));
    server_address.sun_family = AF_UNIX;
    strcpy(server_address.sun_path, SOCKET_PATH);

    // Подключаемся к серверу
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Ошибка подключения к серверу");
        exit(EXIT_FAILURE);
    }

    // Отправляем текст на сервер
    char text[] = "Hello, Server!";
    printf("Отправлено на сервер: %s\n", text);
    send(client_socket, text, strlen(text), 0);

    // Получаем данные от сервера
    char buffer[1024];
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    buffer[bytes_received] = '\0';

    printf("Получено от сервера: %s\n", buffer);

    // Закрываем соединение
    close(client_socket);

    return 0;
}