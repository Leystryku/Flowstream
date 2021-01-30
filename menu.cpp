#pragma once

#include "menu.h"
#include "d3d.h"

//This menu was actually based on someone else's
//I did some refactoring&fixing on it but that's it
//I do not know the name however, because I acquired that code ~2011 and had been using it till ~2014 or something

void Menu::Initiate()
{
	return;
	CreateTab("Aim");
	CreateTab("Gun");
	CreateTab("ESP");
	CreateTab("Misc");
	CreateTab("Chat");

	CreateCheck("Aim", "Aimbot", &M_AimEnable);
	CreateCheck("Aim", "Silent Aim", &M_Aim_Silent);
	CreateCheck("Aim", "PSilent Aim", &M_Aim_PSilent);
	CreateCheck("Aim", "Teamkill", &M_Aim_TeamKill);
	CreateCheck("Aim", "Kill Players", &M_Aim_KillPlayer);

	if ( g_AppID != 240 )
		CreateCheck("Aim", "Kill NPCs", &M_Aim_KillNPC);

	CreateCheck("Aim", "Hitscan", &M_Aim_Hitscan);
	CreateCheck("Aim", "Teamshoot", &M_Aim_TeamShoot);
	CreateCheck("Aim", "Triggerbot", &M_TriggerBot);
	CreateCheck("Aim", "Snipers", &M_Aim_Snipe);
	CreateCheck("Aim", "Ignore Ammo", &M_Aim_IgnoreAmmo);

	CreateCheck("Gun", "Auto Pistol", &M_AutoPistol);
	CreateCheck("Gun", "Alt Auto Pistol", &M_AltAutoPistol);
	CreateCheck("Gun", "Auto Reload", &M_AutoReload);
	CreateCheck("Gun", "Auto Stab", &M_AutoStab);
	CreateCheck("Gun", "Backstab Only", &M_AutoStab_BackOnly);

	if ( g_AppID == 4000 )
		CreateCheck("Gun", "Seed No Spread", &M_NoSpread);

	CreateCheck("Gun", "No Recoil", &M_NoRecoil);
	CreateCheck("Gun", "No Punch Recoil", &M_NoPunchRecoil);
	CreateCheck("Gun", "Compensate Spread", &M_CompensateSpread);

	CreateCheck("ESP", "Box ESP", &M_ESP_Box);
	CreateCheck("ESP", "Name", &M_ESP_Name);
	CreateCheck("ESP", "Health", &M_ESP_Health);

	if ( g_AppID != 240 )
		CreateCheck("ESP", "NPCs", &M_ESP_NPCs);

	CreateCheck("ESP", "Players", &M_ESP_Players);
	CreateCheck("ESP", "Weapons", &M_ESP_Weapons);
	CreateCheck("ESP", "View ESP", &M_ESP_ViewESP);

	CreateCheck("Misc", "Safe Mode", &M_SafeMode);
	CreateCheck("Misc", "Speedhack", &M_SpeedHack);
	CreateCheck("Misc", "Name Changer", &M_NameChanger);
	CreateCheck("Misc", "B-Hop", &M_Bhop);
	CreateCheck("Misc", "Auto Strafe", &M_AutoStrafe);
	CreateCheck("Misc", "Freeze Hack", &M_FreezeHack);
	CreateCheck("Misc", "Disconnect Spam", &M_DisconnectSpam);
	CreateCheck("Misc", "Anti Aim", &M_AntiAim);
	CreateCheck("Misc", "Anti Aim Proxy", &M_AntiAimProxy);
	CreateCheck("Misc", "Flashlight Spam", &M_FlashlightSpam);
	CreateCheck("Misc", "Fake Crouch", &M_FakeCrouch);
	CreateCheck("Misc", "Fake Lag", &M_FakeLag);

	if ( g_AppID == 4000 )
	{
		CreateCheck("Misc", "[GMod] Prop Kill", &M_PropKill);
		CreateCheck("Misc", "[GMod] Run Lua", &M_LuaRun);
	}

	CreateCheck("Chat", "Text Imitator", &M_TextChatImitator );
	CreateCheck("Chat", "Team-Text", &M_TextChatTeam );
	CreateCheck("Chat", "Chat Spam", &M_ChatSpam );
	CreateCheck("Chat", "Chat Jammer", &M_ChatJammer );
	CreateCheck("Chat", "Voice Spam", &M_VoiceChatSpam );

	//Create Sliders
	CreateSlider("Aim", "Aim Smoothing", 30, &S_Aim_Smooth );
	CreateSlider("Aim", "Aim FOV", 180, &S_Aim_FOV );
	//CreateSlider("Aim", "Aim Hitgroup", 7, &S_HitGroup);
	CreateSlider("Aim", "Spawnprotection", 3, &S_SpawnProtection);

	CreateSlider("Aim", "Aim Targetmode", 3, &S_TargetMode);
	CreateSlider("Aim", "Aim Prediction", 3, &S_PredictionType);
	CreateSlider("Aim", "Aim Dist", 1024, &S_Aim_Dist);

	CreateSlider("Gun", "Auto Stab Dist", 100, &S_AutoStabDist);

	CreateSlider("Misc", "Crosshair", 5, &S_CrossHair);
	CreateSlider("Misc", "Speedhack", 10, &S_SpeedHack);
	CreateSlider("Misc", "Fake Lag", 20, &S_FakeLag);

	//Create Text Boxes
	CreateTextBox("ESP", "Custom ESP", &T_CustomESP);

	CreateTextBox("Misc", "Disconnect Message", &T_DisconnectMsg);

	CreateTextBox("Chat", "Chat Spam", &T_ChatSpam);


	//M_ClientSpectate

	menu->InitializedMenu = true;

}

int sizeoftab = 0;
void Menu::DrawMenu()
{

	if (!TabCount)
	{
		Msg("Shit!\n");
		return;
	}

	d3d->DrawGradientRect(10, 10, 300, 20, Grey, DGrey);
	d3d->DrawFilledRectangle(10,30,300,330,DGrey);
	d3d->DrawNonFilledRectangle(10,10,300,350,Outline);

	d3d->DrawText("Flowstream", 100,20,White, d3d->font1);

	sizeoftab = 291 / TabCount;
	d3d->DrawNonFilledRectangle(15,61, sizeoftab * TabCount, 293,Outline);

	for (int i = 0; i < TabCount; i++)
	{
		DrawTab(i, 15 + (i*sizeoftab), 41,sizeoftab);
	}

	int d = 0;
	for(int i = 0; i < CheckCount; i++)
	{
		if(ActiveName == Checks[i].Parent)
		{
			DrawCheckBox(i,30, 70 + (d*20));
			d++;
		}
	}

	d = 0;

	for(int i = 0; i < TextBoxCount; i++)
	{
		if(ActiveName == TextBoxes[i].Parent)
		{
			DrawTextBox(i,150, 100 + (d*40));
			d++;
		}
	}

	for(int i = 0; i < SliderCount; i++)
	{
		if(ActiveName == Sliders[i].Parent)
		{
			DrawSlider(i,150, 100 + (d*40));
			d++;
		}
	}

	if ( Cur.x == 0 && Cur.y == 0 )
		return;

	DWORD color = LightCyan;
	int x = Cur.x;
	int y = Cur.y;


	d3d->DrawFilledRectangle( x, y,11,1,color );
	d3d->DrawFilledRectangle( x+1, y+1,9,1,color );
	d3d->DrawFilledRectangle( x+2, y+2,7,1,color );
	d3d->DrawFilledRectangle( x+3, y+3,7,1,color );
	d3d->DrawFilledRectangle( x+4, y+4,8,1,color );
	d3d->DrawFilledRectangle( x+5, y+5,3,1,color );
	d3d->DrawFilledRectangle( x+6, y+6,2,1,color );


}

void Menu::MenuNavigation()
{

	if (!TabCount)
	{
		return;
	}

	if( GetAsyncKeyState(VK_F10)&1 )
	{
		IsOpen = !IsOpen;
			
		if ( !IsOpen )
		{

			for(int i = 0; i < TextBoxCount; i++)
				TextBoxes[i].Focused = false;
		}

	}

	if ( GetAsyncKeyState(VK_F12)&1 )
	{
		M_FreezeHack = !M_FreezeHack;
	}

	if ( GetAsyncKeyState(VK_F11)&1 )
	{
		M_SpeedHack = !M_SpeedHack;
	}

	if ( !IsOpen )
		return;


	bool ActiveCheck = false;
	for (int i = 0; i < TabCount; i++)
	{
		if(MouseOver(Tabs[i].x, Tabs[i].y, Tabs[i].x + sizeoftab, Tabs[i].y + 20) == 2)
			SwitchTab(i);

		if(Tabs[i].Active)
		{
			ActiveName = Tabs[i].Name;
			ActiveCheck = true;
		}
	}

	if(!ActiveCheck)
		Tabs[0].Active = true;

	if (CheckCount)
	{

		for(int i = 0; i < CheckCount; i++)
		{
			if ( ActiveName != Checks[i].Parent )
				continue;
		
			if((MouseOver(Checks[i].x, Checks[i].y, Checks[i].x + 10 , Checks[i].y + 10) == 3) )
				*Checks[i].Variable = 0;

			if((MouseOver(Checks[i].x, Checks[i].y, Checks[i].x + 10 , Checks[i].y + 10) == 2) )
				*Checks[i].Variable = 1;
		}

	}

	if (SliderCount)
	{
		for (int i = 0; i < SliderCount; i++)
		{

			if (ActiveName != Sliders[i].Parent)
				continue;

			if (((MouseOver(Sliders[i].x, Sliders[i].y - 3, Sliders[i].x + 160, Sliders[i].y + 6) == 2)))
			{
				//Calculate value based on click position.
				int Value1 = (Cur.x - Sliders[i].x);
				float Value2 = ((float)Value1 / (float)120) * Sliders[i].MaxValue;

				if ((int)Value2 > Sliders[i].MaxValue)
					Value2 = (float)Sliders[i].MaxValue;

				if ((int)Value2 < 0)
					Value2 = 0.0f;


				Sliders[i].Value = (int)Value2;
				*Sliders[i].Variable = Sliders[i].Value;
			}
		}
	}

	if (TextBoxCount)
	{
		for (int i = 0; i < TextBoxCount; i++)
		{

			if (ActiveName != TextBoxes[i].Parent)
				continue;

			if ((MouseOver(TextBoxes[i].x, TextBoxes[i].y, TextBoxes[i].x + 100, TextBoxes[i].y + 20) == 2))
				TextBoxes[i].Focused = 1;

			if ((MouseOver(TextBoxes[i].x, TextBoxes[i].y, TextBoxes[i].x + 100, TextBoxes[i].y + 20) == 3))
				TextBoxes[i].Focused = 0;

			if (TextBoxes[i].Focused)
				Menu::ThinkTextBox(i);

		}
	}

}

void Menu::CreateTab(char* Text)
{
	Tabs[TabCount].Name = Text;
	Tabs[TabCount].Active = false;
	TabCount++;
}

void Menu::CreateCheck(char* Parent, char* Text, int* Variable )
{
	Checks[CheckCount].Parent = Parent;
	Checks[CheckCount].Name = Text;
	Checks[CheckCount].Variable = Variable;
	CheckCount++;
}

void Menu::CreateSlider(char* Parent, char* Text, int MaxValue, int* Variable)
{
	Sliders[SliderCount].Parent = Parent;
	Sliders[SliderCount].Name = Text;
	Sliders[SliderCount].MaxValue = MaxValue;
	Sliders[SliderCount].Variable = Variable;
	SliderCount++;
}

void Menu::CreateTextBox(char* Parent, char* Text, std::string *Variable )
{
	TextBoxes[TextBoxCount].Parent = Parent;
	TextBoxes[TextBoxCount].Name = Text;
	TextBoxes[TextBoxCount].Variable = Variable;
	TextBoxCount++;
}

void Menu::DrawTab(int index, int x, int y, int width)
{
	Tabs[index].x = x;
	Tabs[index].y = y;

	if(Tabs[index].Active)
	{
		d3d->DrawGradientRect(x, y, width, 20, Grey, DGrey);
		d3d->DrawNonFilledRectangle(x,y,width,20,Outline);
		d3d->DrawFilledRectangle(x + 1 ,y + 1 ,width - 1 ,3 ,Red);
		d3d->DrawFilledRectangle(x+1,y+19, width - 1, 2,DGrey);
	}
	else
	{
		d3d->DrawFilledRectangle(x+1,y+19, width - 1, 2,DGrey);
		d3d->DrawNonFilledRectangle(x,y,width,20,Outline);
	}

	d3d->DrawText(Tabs[index].Name, x + 10, y + 5, White, d3d->font1);
}

void Menu::DrawCheckBox(int index, int x, int y)
{
	Checks[index].x = x;
	Checks[index].y = y;
	d3d->DrawNonFilledRectangle(x,y,10,10,Outline);
	d3d->DrawText(Checks[index].Name, x + 22, y, White, d3d->font1);

	if(*Checks[index].Variable == 1)
		d3d->DrawFilledRectangle(x + 1, y + 1, 9, 9, Red);
}

void Menu::DrawSlider(int index, int x, int y)
{
	Sliders[index].x = x;
	Sliders[index].y = y;
	d3d->DrawLine(x,y,x+120,y,3, Outline);
	d3d->DrawText(Sliders[index].Name, x + 5, y - 15, White, d3d->font1);
	char lol[256];
	sprintf_s(lol, "%i", Sliders[index].Value);
	d3d->DrawText(lol, x + 55, y + 5, White, d3d->font1);

	//Actual Slider
	float fpos = ((float)Sliders[index].Value/(float)Sliders[index].MaxValue)*((float)120);
	float pos = (float)x + (float)fpos;
	Sliders[index].pos = (int)pos;

	d3d->DrawFilledRectangle(Sliders[index].pos - 10, y - 3, 20, 5, Grey);
	d3d->DrawNonFilledRectangle(Sliders[index].pos - 10, y - 3, 20, 5, White);
}

void Menu::DrawTextBox(int index, int x, int y)
{
	TextBoxes[index].x = x;
	TextBoxes[index].y = y;
	d3d->DrawText(TextBoxes[index].Name, x + 5, y - 15, White, d3d->font1);

	d3d->DrawText(const_cast<char *>(TextBoxes[index].Variable->c_str()), x + 5, y + 5, White, d3d->font1);

	if ( TextBoxes[index].Focused )
		d3d->DrawNonFilledRectangle(x,y,100,20,Red);
	else
		d3d->DrawNonFilledRectangle(x,y,100,20,White);

}

bool upperMode = false;
void Menu::ThinkTextBox(int index)
{
	
	char* curChar = textCharBuf;

	if ( curChar == NULL )
		return;

	//char* newcurChar = _strdup(curChar);

	//curChar = newcurChar;

	if( strstr(curChar, "\b") )
	{

		if(TextBoxes[index].cursor > 0)
		{
			TextBoxes[index].Variable->erase( TextBoxes[index].cursor - 1, TextBoxes[index].cursor );

			TextBoxes[index].cursor = TextBoxes[index].cursor - 1;

		}

		textCharBuf[0] = '\0';
		strcpy_s(textCharBuf, sizeof(textCharBuf), "{_}");

		return;
	}

	if ( strstr(curChar, "{_}") )
		return;

	TextBoxes[index].Variable->append( curChar );

	TextBoxes[index].cursor = TextBoxes[index].cursor + 1;

	textCharBuf[0] = '\0';
	strcpy_s(textCharBuf, sizeof(textCharBuf), "{_}");

}

int Menu::MouseOver(int x, int y, int x2, int y2)
{
	if(Cur.x > x && Cur.x < x2 && Cur.y > y && Cur.y < y2)
	{
		if(GetAsyncKeyState( VK_LBUTTON ) & 0x8000)
			return 2;

		if(GetAsyncKeyState( VK_RBUTTON ) & 0x8000)
			return 3;

		return 1;
	}
	return 0;
}

void Menu::SwitchTab(int index)
{
	for (int i = 0; i < TabCount; i++)
	{
		if(i == index)
			Tabs[i].Active = true;
		else
			Tabs[i].Active = false;
	}
}

Menu *menu = NULL;
