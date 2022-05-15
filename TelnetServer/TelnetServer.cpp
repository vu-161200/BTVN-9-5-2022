// TelnetServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <winsock2.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)
//const MAX_CLIENT = 64;
int numClient = 0;
SOCKET clients[64];
char buf[256];

DWORD WINAPI ClientThread(LPVOID lpParam)
{
    SOCKET client = *(SOCKET*)lpParam;
    int matched = 0;
    char buffer[1024];
    //Khai bao user password
        char user[1024];
        char password[1024];

    while (matched != 1) {
        char* loichao = (char*)"\nVui long nhap user va password theo dinh dang user password:";
        send(client, loichao, strlen(loichao), 0);
        
        recv(client, buffer, sizeof(buffer), 0);
        // Bo khoang trang o cuoi buffer
        while (buffer[strlen(buffer) - 1] == '\r' || buffer[strlen(buffer) - 1] == '\n') {
            buffer[strlen(buffer) - 1] = 0;
        }
        
        sscanf(buffer, "%s%s", user, password);
        //So sanh user va password voi tung tai khoan trong file accountClients.txt
        FILE* f = fopen("C:\\Temp\\accountClients.txt", "rt");
        while (!feof(f)) {
            char line[1024];
            fgets(line, sizeof(line), f);
            char u[1024];
            char p[1024];
            sscanf(line, "%s%s", u, p);
            if (strcmp(user, u) == 0 && strcmp(password, p) == 0) {
                matched = 1;
                break;
            }

        }
        fclose(f);
       if(matched == 0)
       {
        char* Loi = (char*)"\nDang nhap loi:";
        send(client, Loi, strlen(Loi), 0);
       }
       else {
           clients[numClient] = client;
           numClient++;
       }
    }
    while (1) {
        char* Thanhcong = (char*)"\nNhap lenh can thuc hien:";
        send(client, Thanhcong, strlen(Thanhcong), 0);
        //memset(buffer, 0, sizeof(buffer));
        int ret = recv(client, buffer, sizeof(buffer), 0);
        buffer[ret - 1] = 0;
        sprintf(buffer + strlen(buffer), "%s", ">C:\\Temp\\output.txt");
        system(buffer);

        // Doc file va gui lai ket qua cho client
        FILE* f = fopen("C:\\Temp\\output.txt", "rb");
        char *data;
        
        fseek(f, 0, SEEK_END);
        int lSize = ftell(f);
        rewind(f);

        data = (char*) malloc(sizeof(char) * lSize);
        fread(data, 1, lSize, f);

        fclose(f);

        send(client, data, lSize, 0);

        free(data);
    }
   closesocket(client);
}

int main()
{
    // Khoi tao thu vien
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    // Tao socket
    SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // Khai bao dia chi server
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(8000);
    // Gan cau truc dia chi voi socket
    bind(listener, (SOCKADDR*)&addr, sizeof(addr));
    // Chuyen sang trang thai cho ket noi
    listen(listener, 5);
    while (1)
    {
        SOCKET client = accept(listener, NULL, NULL);
        printf("Client moi ket noi: %d\n", client);
        CreateThread(0, 0, ClientThread, &client, 0, 0);
    }
}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
