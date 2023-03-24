#define XA_LENGTH(x) (((u64)(x) * 75) / 100 * IO_SECT_SIZE) //Centiseconds to sectors in bytes (w)

typedef struct
{
	XA_File file;
	u32 length;
} XA_TrackDef;

static const XA_TrackDef xa_tracks[] = {
	//MENU.XA
	{XA_Menu, XA_LENGTH(11300)}, //XA_GettinFreaky
	{XA_Menu, XA_LENGTH(3800)},  //XA_GameOver
	//1.XA
	{XA_1, XA_LENGTH(18510)}, //XA_BiteVreen
	{XA_1, XA_LENGTH(14400)}, //XA_RestlessVreen
	//2.XA
	{XA_2, XA_LENGTH()}, //XA_UnlikelyRivalsVloo
	{XA_2, XA_LENGTH()}, //XA_BiteVloo
	//3.XA
	{XA_3, XA_LENGTH()}, //XA_GuyVloo
	{XA_3, XA_LENGTH()}, //XA_MidnightVloo
	//4.XA
	{XA_4, XA_LENGTH(21783)}, //XA_AmusiaLullaby
	{XA_4, XA_LENGTH(19452)}, //XA_DeathTollLullaby
	//5.XA
	{XA_5, XA_LENGTH(26000)}, //XA_ShitnoLullaby
	{XA_5, XA_LENGTH(38741)}, //XA_BrimstoneLullaby
	//6.XA
	{XA_6, XA_LENGTH(16434)}, //XA_YieldMF
	{XA_6, XA_LENGTH(22484)}, //XA_ChecksumMF
	//7.XA
	{XA_7, XA_LENGTH(19511)}, //XA_DissonanceMF
	{XA_7, XA_LENGTH(20400)}, //XA_PickUpMF
	//8.XA
	{XA_8, XA_LENGTH(18251)}, //XA_DakotaMF
	{XA_8, XA_LENGTH(23383)}, //XA_WarforgedMF
};

static const char *xa_paths[] = {
	"\\MUSIC\\MENU.XA;1",   //XA_Menu
	"\\MUSIC\\1.XA;1", //XA_1
	NULL,
};

typedef struct
{
	const char *name;
	boolean vocal;
} XA_Mp3;

static const XA_Mp3 xa_mp3s[] = {
	//MENU.XA
	{"freaky", false},   //XA_GettinFreaky
	{"gameover", false}, //XA_GameOver
	//1.XA
	{"bitevreen", true}, //XA_BiteVreen
	{"restlessvreen", true},   //XA_RestlessVreen
	
	{NULL, false}
};
