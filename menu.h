
#pragma once
#include "includes.h"


class Slider
{
public:
	char* Name;
	char* Parent;
	int MaxValue;
	int Value;
	int* Variable;
	int x;
	int y;
	int pos;
};

class Tab
{
public:
	bool Active;
	char* Name;
	int x;
	int y;
};

class TextBox
{
public:
	char* Name;
	char* Parent;
	std::string *Variable;
	bool Focused;
	int cursor=0;
	int x;
	int y;
};

class Check
{
public:
	char* Name;
	char* Parent;
	int* Variable;
	int x;
	int y;
};


class Menu
{
public:

	Check Checks[100];
	Tab Tabs[10];
	TextBox TextBoxes[100];
	Slider Sliders[100];

	int M_LuaRun, M_SafeMode, M_FakeCrouch, M_FakeLag, M_AntiAimProxy, M_AimEnable, M_Aim_TeamKill, M_Aim_Silent, M_Aim_Hitscan, M_Aim_PSilent, M_Aim_TeamShoot, M_TriggerBot, M_AutoStab, M_AutoStab_BackOnly, M_AutoPistol, M_AltAutoPistol, M_NoSpread, M_CompensateSpread, M_NoRecoil, M_NoPunchRecoil, M_AutoReload, M_ESP_Box,  M_ESP_Name, M_ESP_Health, M_ESP_Weapons, M_NameChanger, M_Bhop, M_FreezeHack, M_DisconnectSpam, M_AutoStrafe, M_VoiceChatSpam, M_SpeedHack, M_TextChatImitator, M_TextChatTeam, M_ChatSpam, M_ChatJammer, M_Aim_KillNPC, M_ESP_NPCs, M_ESP_Players, M_ESP_ViewESP, M_Aim_KillPlayer, M_Aim_IgnoreAmmo, M_Aim_Snipe, M_AntiAim, M_PropKill, M_FlashlightSpam, M_ToughenUp;	

	int S_SpawnProtection, S_PredictionType, S_FakeLag, S_Aim_Smooth, S_Aim_FOV, S_CrossHair, S_SpeedHack, S_HitGroup, S_TargetMode, S_AutoStabDist, S_Aim_Dist;

	std::string T_CustomESP, T_ChatSpam, T_VoiceChatSpam, T_DisconnectMsg;

	int TabCount, CheckCount, SliderCount, TextBoxCount;
	char* ActiveName;
	char textCharBuf[999];
	POINT Cur;
	bool InitializedMenu;
	bool IsOpen;
	

	void InitMenu();
	void DrawMenu();
	void MenuNavigation();
	void CreateCheck(char* Parent, char* Text, int* Variable);
	void CreateTab(char* Text);
	void CreateSlider(char* Parent, char* Text, int MaxValue, int* Variable);
	void CreateTextBox(char* Parent, char* Text, std::string *Variable );
	void DrawSlider(int index, int x, int y);
	void DrawTab(int index, int x, int y, int width);
	void DrawCheckBox(int index, int x, int y);
	void DrawTextBox(int index, int x, int y);
	void ThinkTextBox(int index);
	void Initiate( );
	int MouseOver(int x, int y, int x2, int y2);
	void SwitchTab(int index);

};

extern Menu *menu;