#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>
#include <fstream>


using namespace std;

int main()
{
    //создание сокета
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if(listening == -1){
        cerr << "Cant create socket!";
        return - 1;
    }

    //привязка сокета к ip/port
    sockaddr_in hintServ;
    hintServ.sin_family = AF_INET;
    hintServ.sin_port = htons(54000);
    inet_pton(AF_INET, "0.0.0.0", &hintServ.sin_addr);

    if(bind(listening, (sockaddr*)&hintServ, sizeof(hintServ)) == -1){
        cerr << "Cant bint to IP/port";
        return -2;
    }

    //маркеруем сокет
    if(listen(listening, SOMAXCONN) == -1){
        cerr << "Cant listen socket";
        return -3;
    }

    //accept a call
    sockaddr_in client;
    socklen_t clientSize = sizeof (client);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];

    int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
    if(clientSocket == -1){
        cerr << "Proplem with client connecting!";
        return -4;
    }


    //close the listening socket
    close(listening);

    memset(host,0, NI_MAXHOST);
    memset(svc, 0, NI_MAXSERV);

    int result = getnameinfo((sockaddr*)&client, clientSize, host, NI_MAXHOST, svc, NI_MAXSERV, 0);

    if(result){
        cout << host << " connected on " << svc << endl;
    }else{
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host << " connected on " << ntohs(client.sin_port) << endl;
    }

    //возвращаем сообщение
    string tofile;
    ofstream in("Testfile.txt");
    char buff[4096];
    while(true){
        memset(buff, 0, 4096);
        int byteRecev = recv(clientSocket, buff, 4096, 0);
        if(byteRecev == -1){
            cerr << "Connection issue!";
            break;
        }
        if(byteRecev == 0){
            cout << "Client disconnected!" << endl;
            break;
        }
        tofile += string(buff, 0, byteRecev);
        //display
        //cout << "Recived: " << string(buff, 0, byteRecev) << endl;
        in << tofile;
        in.close();
        //resend
        //send(clientSocket, buff, byteRecev + 1, 0);

    }

    close(clientSocket);

    return 0;
}
