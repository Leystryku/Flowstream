#pragma once

#include "includes.h"
#include "netvars.h"

enum TF2__COND
{ 
    TFCond_Slowed = (1 << 0), //Toggled when a player is slowed down. 
    TFCond_Zoomed = (1 << 1), //Toggled when a player is zoomed. 
    TFCond_Disguising = (1 << 2), //Toggled when a Spy is disguising.  
    TFCond_Disguised = (1 << 3), //Toggled when a Spy is disguised. 
    TFCond_Cloaked = (1 << 4), //Toggled when a Spy is invisible. 
    TFCond_Ubercharged = (1 << 5), //Toggled when a player is ÜberCharged. 
    TFCond_TeleportedGlow = (1 << 6), //Toggled when someone leaves a teleporter and has glow beneath their feet. 
    TFCond_Taunting = (1 << 7), //Toggled when a player is taunting. 
    TFCond_UberchargeFading = (1 << 8), //Toggled when the ÜberCharge is fading. 
    TFCond_CloakFlicker = (1 << 9), //Toggled when a Spy is visible during cloak. 
    TFCond_Teleporting = (1 << 10), //Only activates for a brief second when the player is being teleported; not very useful. 
    TFCond_Kritzkrieged = (1 << 11), //Toggled when a player is being crit buffed by the KritzKrieg. 
    TFCond_TmpDamageBonus = (1 << 12), //Unknown what this is for. Name taken from the AlliedModders SDK. 
    TFCond_DeadRingered = (1 << 13), //Toggled when a player is taking reduced damage from the Deadringer. 
    TFCond_Bonked = (1 << 14), //Toggled when a player is under the effects of The Bonk! Atomic Punch. 
    TFCond_Stunned = (1 << 15), //Toggled when a player's speed is reduced from airblast or a Sandman ball. 
    TFCond_Buffed = (1 << 16), //Toggled when a player is within range of an activated Buff Banner. 
    TFCond_Charging = (1 << 17), //Toggled when a Demoman charges with the shield. 
    TFCond_DemoBuff = (1 << 18), //Toggled when a Demoman has heads from the Eyelander. 
    TFCond_CritCola = (1 << 19), //Toggled when the player is under the effect of The Crit-a-Cola. 
    TFCond_InHealRadius = (1 << 20), //Unused condition, name taken from AlliedModders SDK. 
    TFCond_Healing = (1 << 21), //Toggled when someone is being healed by a medic or a dispenser. 
    TFCond_OnFire = (1 << 22), //Toggled when a player is on fire. 
    TFCond_Overhealed = (1 << 23), //Toggled when a player has >100% health. 
    TFCond_Jarated = (1 << 24), //Toggled when a player is hit with a Sniper's Jarate. 
    TFCond_Bleeding = (1 << 25), //Toggled when a player is taking bleeding damage. 
    TFCond_DefenseBuffed = (1 << 26), //Toggled when a player is within range of an activated Battalion's Backup. 
    TFCond_Milked = (1 << 27), //Player was hit with a jar of Mad Milk. 
    TFCond_MegaHeal = (1 << 28), //Player is under the effect of Quick-Fix charge. 
    TFCond_RegenBuffed = (1 << 29), //Toggled when a player is within a Concheror's range. 
    TFCond_MarkedForDeath = (1 << 30), //Player is marked for death by a Fan O'War hit. Effects are similar to TFCond_Jarated. 

    TFCondEx_SpeedBuffAlly = (1 << 0), //Toggled when a player gets hit with the disciplinary action. 
    TFCondEx_HalloweenCritCandy = (1 << 1), //Only for Scream Fortress event maps that drop crit candy. 
    TFCondEx_CritCanteen = (1 << 2), //Player is getting a crit boost from a canteen.
    TFCondEx_CritHype = (1 << 4), //Soda Popper crits. 
    TFCondEx_CritOnFirstBlood = (1 << 5), //Arena first blood crit buff. 
    TFCondEx_CritOnWin = (1 << 6), //End of round crits. 
    TFCondEx_CritOnFlagCapture = (1 << 7), //CTF intelligence capture crits. 
    TFCondEx_CritOnKill = (1 << 8), //Unknown what this is for. 
    TFCondEx_RestrictToMelee = (1 << 9), //Unknown what this is for. 
    TFCondEx_Reprogrammed = (1 << 11), //MvM Bot has been reprogrammed.
    TFCondEx_PyroCrits = (1 << 12), //Player is getting crits from the Mmmph charge. 
    TFCondEx_PyroHeal = (1 << 13), //Player is being healed from the Mmmph charge. 
    TFCondEx_FocusBuff = (1 << 14), //Player is getting a focus buff.
    TFCondEx_DisguisedRemoved = (1 << 15), //Disguised remove from a bot.
    TFCondEx_MarkedForDeathSilent = (1 << 16), //MvM related.
    TFCondEx_DisguisedAsDispenser = (1 << 17), //Bot is disguised as dispenser.
    TFCondEx_Sapped = (1 << 18), //MvM bot is being sapped.
    TFCondEx_UberchargedHidden = (1 << 19), //MvM Related
    TFCondEx_UberchargedCanteen = (1 << 20), //Player is recieveing ubercharge from a canteen.
    TFCondEx_HalloweenBombHead = (1 << 21), //Player has a bomb on their head from Merasmus.
    TFCondEx_HalloweenThriller = (1 << 22), //Players are forced to dance from Merasmus.
};

enum TF2__WEAPONS_SCOUT
{
	//Primary
	WPN_Scattergun = 13,
	WPN_NewScattergun = 200,
	WPN_FAN = 45,
	WPN_Shortstop = 220,
	WPN_SodaPopper = 448,
	WPN_BabyFaceBlaster = 772,
	WPN_BotScattergunS = 799,
	WPN_BotScattergunG = 808,
	WPN_BotScattergunR = 888,
	WPN_BotScattergunB = 897,
	WPN_BotScattergunC = 906,
	WPN_BotScattergunD = 915,
	WPN_BotScattergunES = 964,
	WPN_BotScattergunEG = 973,
	//Secondary
	WPN_ScoutPistol = 23,
	WPN_NewPistol = 209,
	WPN_Bonk = 46,
	WPN_CritCola = 163,
	WPN_Lugermorph = 160,
	WPN_Milk = 222,
	WPN_Lugermorph2 = 294,
	WPN_Winger = 449,
	WPN_PocketPistol = 773,
	//Melee
	WPN_Bat = 0,
	WPN_NewBat = 190,
	WPN_Sandman = 44,
	WPN_Fish = 221,
	WPN_Cane = 317,
	WPN_BostonBasher = 325,
	WPN_SunStick = 349,
	WPN_FanOWar = 355,
	WPN_RuneBlade = 452,
	WPN_Saxxy = 423,
	WPN_Atomizer = 450,
	WPN_ConscientiousObjector = 474,
	WPN_UnarmedCombat = 572,
	WPN_WrapAssassin = 648,
	WPN_FestiveBat = 660,
	WPN_FestiveScattergun = 669,
	WPN_FlyingGuillotine1 = 812,
	WPN_FlyingGuillotine2 = 833,
	WPN_FreedomStaff = 880,
	WPN_BatOuttaHell = 939,
	WPN_FestiveFish = 999,
};

enum TF2__WEAPONS_SOLDIER
{
	//Primary
	WPN_RocketLauncher = 18,
	WPN_NewRocketLauncher = 205,
	WPN_DirectHit = 127,
	WPN_BlackBox = 228,
	WPN_RocketJump = 237,
	WPN_LibertyLauncher = 414,
	WPN_CowMangler = 441,
	WPN_Original = 513,
	WPN_FestiveRocketLauncher = 658,
	WPN_BeggersBazooka = 730,
	WPN_BotRocketlauncherS = 800,
	WPN_BotRocketlauncherG = 809,
	WPN_BotRocketlauncherR = 889,
	WPN_BotRocketlauncherB = 898,
	WPN_BotRocketlauncherC = 907,
	WPN_BotRocketlauncherD = 916,
	WPN_BotRocketlauncherES = 965,
	WPN_BotRocketlauncherEG = 974,
	//Secondary
	WPN_SoldierShotgun = 10,
	WPN_NewShotgun = 199,
	WPN_BuffBanner = 129,
	WPN_BattalionBackup = 226,
	WPN_Concheror = 354,
	WPN_ReserveShooter = 415,
	WPN_RighteousBison = 442,
	WPN_FestiveBuffBanner = 1001,
	//Melee
	WPN_Shovel = 6,
	WPN_NewShovel = 196,
	WPN_Equalizer = 128,
	WPN_PainTrain = 154,
	WPN_Katana = 357,
	WPN_MarketGardener = 416,
	WPN_DisciplinaryAction = 447,
	WPN_EscapePlan = 775,
};

enum TF2__WEAPONS_PYRO
{
	//Primary
	WPN_Flamethrower = 21,
	WPN_NewFlamethrower = 208,
	WPN_Backburner = 40,
	WPN_Degreaser = 215,
	WPN_FestiveFlamethrower = 659,
	WPN_Phlogistinator = 594,
	WPN_Rainblower = 741,
	WPN_BotFlamethrowerS = 798,
	WPN_BotFlamethrowerG = 807,
	WPN_BotFlamethrowerR = 887,
	WPN_BotFlamethrowerB = 896,
	WPN_BotFlamethrowerC = 905,
	WPN_BotFlamethrowerD = 914,
	WPN_BotFlamethrowerES = 963,
	WPN_BotFlamethrowerEG = 972,
	//Secondary
	WPN_PyroShotgun = 12,
	WPN_Flaregun = 39,
	WPN_Detonator = 351,
	WPN_ManMelter = 595,
	WPN_ScorchShot = 740,
	//Melee
	WPN_Fireaxe = 2,
	WPN_NewAxe = 192,
	WPN_Axtingusher = 38,
	WPN_HomeWrecker = 153,
	WPN_PowerJack = 214,
	WPN_Backscratcher = 326,
	WPN_VolcanoFragment = 348,
	WPN_Maul = 466,
	WPN_Mailbox = 457,
	WPN_ThirdDegree = 593,
	WPN_Lollychop = 739,
	WPN_NeonAnnihilator1 = 813,
	WPN_NeonAnnihilator2 = 834,
	WPN_FestiveAxtwingisher = 1000,
};

enum TF2__WEAPONS__DEMOMAN
{
	//Primary
	WPN_GrenadeLauncher = 19,
	WPN_NewGrenadeLauncher = 206,
	WPN_LochNLoad = 308,
	WPM_LoooseCannon = 996,
	WPN_FestiveGrenadeLauncher = 1007,
	//Secondary
	WPN_StickyLauncher = 20,
	WPN_NewStickyLauncher = 207,
	WPN_ScottishResistance = 130,
	WPN_StickyJumper = 265,
	WPN_FestiveStickyLauncher = 661,
	WPN_BotStickyS = 797,
	WPN_BotStickyG = 806,
	WPN_BotStickyR = 886,
	WPN_BotStickyB = 895,
	WPN_BotStickyC = 904,
	WPN_BotStickyD = 913,
	WPN_BotStickyES = 962,
	WPN_BotStickyEG = 971,
	//Melee
	WPN_Bottle = 1,
	WPN_NewBottle = 191,
	WPN_Sword = 132,
	WPN_ScottsSkullctter = 172,
	WPN_Fryingpan = 264,
	WPN_Headless = 266,
	WPN_Ullapool = 307,
	WPN_Claidheamhmor = 327,
	WPN_PersainPersuader = 404,
	WPN_Golfclub = 482,
	WPN_ScottishHandshake = 609,
};

enum TF2__WEAPONS__HEAVY
{
	//Primary
	WPN_Minigun = 15,
	WPN_NewMinigun = 202,
	WPN_Natascha =  41,
	WPN_IronCurtain = 298,
	WPN_BrassBeast = 312,
	WPN_Tomislav = 424,
	WPN_FestiveMinigun = 654,
	WPN_HuoLongHeatmaker1 = 811,
	WPN_HuoLongHeatmaker2 = 832,
	WPN_BotMinigunS = 793,
	WPN_BotMinigunG = 802,
	WPN_BotMinigunR = 882,
	WPN_BotMinigunB = 891,
	WPN_BotMinigunC = 900,
	WPN_BotMinigunD = 909,
	WPN_Deflector = 850,
	WPN_BotMinigunES = 958,
	WPN_BotMinigunEG = 967,
	//Secondary
	WPN_HeavyShotgun = 11,
	WPN_Sandvich = 42,
	WPN_CandyBar = 159,
	WPN_Steak = 311,
	WPN_Fishcake = 433,
	WPN_FamilyBuisness = 425,
	WPN_RobotSandvich = 863,
	WPN_FestiveSandvich = 1002,
	//Melee
	WPN_Fists = 5,
	WPN_NewFists = 195,
	WPN_KGB = 43,
	WPN_GRU = 239,
	WPN_WarriorSpirit = 310,
	WPN_FistsOfSteel = 331,
	WPN_EvictionNotice = 426,
	WPN_ApocoFists = 587,
	WPN_HolidayPunch = 656,
};

enum TF2__WEAPONS__ENGINEER
{
	//Primary
	WPN_EngineerShotgun = 9,
	WPN_FrontierJustice = 141,
	WPN_Widowmaker = 527,
	WPN_Pomson = 588,
	WPN_RescueRanger = 997,
	WPN_FestiveFrontierJustice = 1004,
	//Secondary
	WPN_EngineerPistol = 22,
	WPN_Wrangler = 140,
	WPN_ShortCircut = 528,
	//Melee
	WPN_Wrench = 7,
	WPN_NewWrench = 197,
	WPN_Goldenwrench = 169,
	WPN_SouthernHospitality = 155,
	WPN_Gunslinger = 142,
	WPN_Jag = 329,
	WPN_FestiveWrench = 662,
	WPN_EurekaEffect = 589,
	WPN_BotWrenchS = 795,
	WPN_BotWrenchG = 804,
	WPN_BotWrenchR = 884,
	WPN_BotWrenchB = 893,
	WPN_BotWrenchC = 902,
	WPN_BotWrenchD = 911,
	WPN_BotWrenchES = 960,
	WPN_BotWrenchEG = 969,
	//Misc
	WPN_Builder = 25,
	WPN_Builder2 = 737,
	WPN_Destructor = 26,
	WPN_Toolbox = 28,
};

enum TF2__WEAPONS_MEDIC
{
	//Primary
	WPN_SyringeGun = 17,
	WPN_NewSyringeGun = 204,
	WPN_Blutsauger = 36,
	WPN_Crossbow = 305,
	WPN_Overdose = 412,
	//Secondary
	WPN_Medigun = 29,
	WPN_NewMedigun = 211,
	WPN_Kritzkrieg = 35,
	WPN_QuickFix = 411,
	WPN_FestiveMedigun = 663,
	WPN_MedigunS = 796,
	WPN_MedigunG = 805,
	WPN_MedigunR = 885,
	WPN_MedigunB = 894,
	WPN_MedigunC = 903,
	WPN_MedigunD = 912,
	WPN_Vaccinator = 998,
	WPN_MedigunES = 961,
	WPN_MedigunEG = 970,
	//Melee
	WPN_Bonesaw = 8,
	WPN_NewBonesaw = 198,
	WPN_Ubersaw = 37,
	WPN_Vitasaw = 173,
	WPN_Amputator = 304,
	WPN_Solemnvow = 413,
	WPN_FestiveUbersaw = 1003,
};

enum TF2__WEAPONS_SNIPER
{
	//Primary
	WPN_SniperRifle = 14,
	WPN_NewSniperRifle = 201,
	WPN_Huntsman = 56,
	WPN_SydneySleeper = 230,
	WPN_Bazaarbargain = 402,
	WPN_Machina = 526,
	WPN_FestiveSniperRifle = 664,
	WPN_HitmanHeatmaker = 752,
	WPN_BotSniperRifleS = 792,
	WPN_BotSniperRifleG = 801,
	WPN_BotSniperRifleR = 881,
	WPN_BotSniperRifleB = 890,
	WPN_BotSniperRifleC = 899,
	WPN_BotSniperRifleD = 908,
	WPN_AWP = 851,
	WPN_BotSniperRifleES = 957,
	WPN_BotSniperRifleEG = 966,
	WPN_FestiveHuntsman = 1005,
	//Secondary
	WPN_SMG = 16,
	WPN_NewSMG = 203,
	WPN_Jarate = 58,
	WPN_DarwinDangerShield = 231,
	WPN_CleanersCarbine = 751,
	//Melee
	WPN_Kukri = 3,
	WPN_NewKukri = 193,
	WPN_TribalmansShiv = 171,
	WPN_Bushwacka = 232,
	WPN_Shahanshah = 401,
	//Hats
	HAT_Snaggletooth = 229,
};

enum TF2__WEAPONS_SPY
{
	//Primary
	WPN_Revolver = 24,
	WPN_NewRevolver = 210,
	WPN_Ambassador =  61,
	WPN_BigKill = 161,
	WPN_Letranger = 224,
	WPN_Enforcer = 460,
	WPN_Diamondback = 525,
	WPN_FestiveAmbassador = 1006,
	//Melee
	WPN_Knife = 4,
	WPN_NewKnife = 194,
	WPN_EternalReward = 225,
	WPN_DisguiseKit = 27,
	WPN_Kunai = 356,
	WPN_BigEarner = 461,
	WPN_WangaPrick = 574,
	WPN_SharpDresser = 638,
	WPN_Spycicle = 649,
	WPN_FestiveKnife = 665,
	WPN_BlackRose = 727,
	WPN_BotKnifeS = 794,
	WPN_BotKnifeG = 803,
	WPN_BotKnifeR = 883,
	WPN_BotKnifeB = 892,
	WPN_BotKnifeC = 901,
	WPN_BotKnifeD = 910,
	WPN_BotKnifeES = 959,
	WPN_BotKnifeEG = 968,
	//Watches
	WPN_InivisWatch = 30,
	WPN_DeadRinger = 59,
	WPN_CloakAndDagger = 60,
	WPN_TimePiece = 297,
	//Sapper
	WPN_Sapper = 735,
	WPN_Sapper2 = 736,
	WPN_RedTape1 = 810,
	WPN_RedTape2 = 831,
};


float TF2__flGetSpreadFactor( void );

float TF2__flGetWeaponSpread( C_BaseCombatWeapon* pWeapon );