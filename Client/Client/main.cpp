#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>
#include <fstream>
#include <sstream>

using namespace std;


void sockConnetc(const string &fromfile){

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        exit(1);
    }

    //создание структуры для сервера
    int port = 54000;
    string ipAdress = "127.0.0.1";
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAdress.c_str(), &hint.sin_addr);

    //соединение с сервером
    int connectRes = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if(connectRes == -1){
        exit(1);
    }

    char buf[4096];
    string userinput = fromfile;

    do{
        cout << "> ";
        int sendRes = send(sock, userinput.c_str(), userinput.size() + 1, 0);
        if(sendRes == -1){
            cout << "couldnt send to server!" << endl;
            continue;
        }

        memset(buf, 0 , 4096);
        int byteRecived = recv(sock, buf,  4096, 0);
        cout << "Server: " << string(buf, byteRecived)<< "\r\n";
    }while(true);

    close(sock);
}

std::string readFile(const std::string& fileName) {
    std::ifstream f(fileName);
    std::stringstream ss;
    if(f.is_open()){
       ss << f.rdbuf();
    }
    f.close();
    return ss.str();
}

int main()
{
    //создание сокета
    char choise;
    string filepath, filedata;
    cout << "Введите путь к текстовому файлу: ";
    cin >> filepath;
    filedata = readFile(filepath);
    filedata + "\n";
    //cout << filedata;
    cout << "Для подключения к серверу и передачи данных введите '+' : ";
    cin >> choise;
    if(choise == '+'){
        sockConnetc(filedata);
    }else{
        exit(1);
    }

    return 0;
}
