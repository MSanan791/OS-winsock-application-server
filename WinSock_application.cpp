#include <iostream>
#include<WS2tcpip.h>
#include<sstream>

#pragma comment (lib,"ws2_32.lib")



using namespace std;



void main()
{
    // initialize winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    int wsOK = WSAStartup(ver, &wsData);

    if (wsOK != 0) {
        cerr << "Cant initialize winsock! Quitting" << endl;
        return;
    }


    //Create a socket
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == INVALID_SOCKET) {
        cerr << "Cant't create Socket! Quitting" << endl;
        return;
    }


    // Bind the Socket to an Ip Address and Port
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    hint.sin_addr.S_un.S_addr = INADDR_ANY;

    bind(listening, (sockaddr*)&hint, sizeof(hint));

    // Tell winSOck Socket is for listening
    listen(listening, SOMAXCONN);


    fd_set master;

    FD_ZERO(&master);

    FD_SET(listening, &master);


    cout << "Server initialised...\n";
    cout << "awaiting connection\n";


    while (true)
    {
        fd_set copy = master;

        int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

      

        for (int i = 0; i < socketCount; i++) {
            SOCKET sock = copy.fd_array[i];
            if (sock == listening)
            {
                //accept new connection
                SOCKET client = accept(listening, nullptr, nullptr);
                cout << "New Connection detected\n";


                // add a new message to the list of connected clients
                FD_SET(client, &master);



                // send  a welcome message to the connected client
                string welcomeMsg = "Welcome to the chat!!!\r\n";
                send(client, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);



              
            }
            else
            {
                char buf[4096];
                ZeroMemory(buf, 4096);

                int bytesIn = recv(sock, buf, 4096, 0);
                if (bytesIn <= 0)
                {
                    // Drop the client
                    closesocket(sock);
                    FD_CLR(sock, &master);

                }
                else
                {
                    //send message out  send message to  other clients , and definitely not a listening socket

                    for (int i = 0; i < master.fd_count; i++)
                    {
                        SOCKET outSock = master.fd_array[i];
                        if (outSock != listening && outSock != sock)
                        {
                            ostringstream ss;
                            ss << "Socket #" << sock <<":" << buf << "\r\n";
                            string strOut = ss.str();
                            cout << strOut << endl;

                            send(outSock, strOut.c_str(), strOut.size()+1, 0);
                        }
                    }
                }

                //accept a new message
                

            }
        }


    }


    //Shutdown winSock
    WSACleanup();

    system("pause");

}

//// wait for connection
  //sockaddr_in client;
  //int clientSize = sizeof(client);

  //SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

  //char host[NI_MAXHOST]; // Client Remote name
  //char service[NI_MAXHOST];// Service (port) the client is connected on

  //ZeroMemory(host, NI_MAXHOST);// same as memset(host,0,NI_MAXHOST)
  //ZeroMemory(service, NI_MAXHOST);

  //if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
  //{
  //    cout << host << "Connected on port " << service << endl;
  //}
  //else
  //{
  //    inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
  //    cout << host << "connected on port" << ntohs(client.sin_port) << endl;
  //}


  //// close listening socket
  //closesocket(listening);



  //// while loop: accept and echo message back to client.
  //char buf[4096];
  //while (true) {
  //    ZeroMemory(buf, 4096);
  //    //wait for client to send data
  //    int bytesRecieved = recv(clientSocket, buf, 4096, 0);
  //    if (bytesRecieved == SOCKET_ERROR)
  //    {
  //        cerr << "Error in recv(). Quitting" << endl;
  //    }
  //    if(bytesRecieved == 0) {
  //        cout << "Clients disconnected" << endl;
  //        break;
  //    }
  //    cout << string(buf, 0, bytesRecieved);
  //    //echo message back to client
  //    send(clientSocket, buf, bytesRecieved + 1, 0);

  //}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
