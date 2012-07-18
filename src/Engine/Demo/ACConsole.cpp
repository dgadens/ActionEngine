#include "ACConsole.h"

std::string ACConsole::COMMAND0 = "CREATEHOST";
std::string ACConsole::COMMAND1 = "OPEN";
std::string ACConsole::COMMAND2 = "CHAT";
std::string ACConsole::COMMAND3 = "ADDMODEL";
std::string ACConsole::SYMBOLS = ":,.;+-=";

void ACConsole::Clear()
{
	GenericTools::ClearCharArray(Text, 256);

	mIndex = 2; 
	Text[0] = '>'; 
	Text[1] = ' '; 

	mpSpriteFont->SetText(Text, 256);
};

void ACConsole::SetCharacter(char c)
{
	int ascCode = c<0?c+128:c;

	if (mIndex<256)
	{
		switch (ascCode)
		{
			case 8:  //backspace
			{
				if (mIndex>2)
				{
					for (int i = mIndex; i < 256; i++)
						Text[i-1] = Text[i];

					--mIndex;
				}
			};
			break;
			case 127:  //delete
			{
				
			};
			break;
			case 13:  //enter processa o comando pra ver oq fazer
			{
				ProcessCommand();
			};
			break;
			default:
			{
				if (ascCode > 31 && ascCode< 255) //pega so os caracteres
				{
					Text[mIndex++] = c;
				}
			};
		};

		mpSpriteFont->SetText(Text, 256);
	}
};

void ACConsole::Render()
{
	mpSpriteBatch->BeginRender();
	mpSpriteBatch->Render(mpSpriteFont,&Vector2(-Globals::MainWndWidth/2,Globals::MainWndHeight/2-5),&Vector2(1,1),0,&Vector4(1,1,1,1));
	mpSpriteBatch->EndRender();
};

void ACConsole::ProcessCommand()
{
	//extrai os tokens
	std::vector<std::vector<char>> tokens;
	std::vector<char> token;
	int i = 2;
	while (true)
	{
		if (Text[i] != 32) //space
		{
			//se chego no final sai fora
			if (Text[i]=='\0')
			{
				tokens.push_back(token);
				token.clear();
				break;
			}

			token.push_back(Text[i]);
		}
		else
		{
			tokens.push_back(token);
			token.clear();
		}
		++i;
	}

	//pega o primeiro token
	std::string command( tokens[0].begin(), tokens[0].end() );

	if (command=="CREATEHOST")
	{
		std::string port( tokens[1].begin(), tokens[1].end() );
		Globals::pNetworkDevice->Init(Globals::MainWnd, NetModeType::NMT_Server, GenericTools::StringToInt(port), "", MAX_PKG_SIZE, MAX_CLIENTS, true);
		Globals::NetMode = NetModeType::NMT_Server;
		Globals::NetID = 0;
	} else if (command=="OPEN")
	{
		std::string ip( tokens[1].begin(), tokens[1].end() );
		std::string port( tokens[2].begin(), tokens[2].end() );
		Globals::pNetworkDevice->Init(Globals::MainWnd, NetModeType::NMT_Client, GenericTools::StringToInt(port), ip.c_str(), MAX_PKG_SIZE, MAX_CLIENTS, true);
		Globals::NetMode = NetModeType::NMT_Client;
	} else if (command=="CHAT")
	{
		ACPACKAGE pack;
		pack.pData = Text; 
		pack.nSender = Globals::NetID;
		pack.nType = MSG_CHAT;
		pack.nLength = sizeof(char) * 256;

		if (Globals::NetMode == NetModeType::NMT_Server)
			Globals::pNetworkDevice->SendToClients(&pack);
		else
			Globals::pNetworkDevice->SendToServer(&pack);

	} else if (command=="ADDMODEL")
	{
		std::string modelName( tokens[1].begin(), tokens[1].end() );
		modelName.append(".amt");
		ACModel* model = new ACModel(mpGDevice, mpCManager);
		model->Load(modelName);
		model->SetAbsoluteScale(50,50,50);
		model->SetAbsolutePosition(0,0,0);
	}

	Clear();
};