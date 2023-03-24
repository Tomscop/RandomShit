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
	{XA_2, XA_LENGTH(16641)}, //XA_UnlikelyRivalsVloo
	{XA_2, XA_LENGTH(12268)}, //XA_BiteVloo
	//3.XA
	{XA_3, XA_LENGTH(12000)}, //XA_GuyVloo
	{XA_3, XA_LENGTH(16167)}, //XA_MidnightVloo
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
	"\\MUSIC\\2.XA;1", //XA_2
	"\\MUSIC\\3.XA;1", //XA_3
	"\\MUSIC\\4.XA;1", //XA_4
	"\\MUSIC\\5.XA;1", //XA_5
	"\\MUSIC\\6.XA;1", //XA_6
	"\\MUSIC\\7.XA;1", //XA_7
	"\\MUSIC\\8.XA;1", //XA_8
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
	//2.XA
	{"unlikelyrivalsvloo", true}, //XA_UnlikelyRivalsVloo
	{"bitevloo", true},   //XA_BiteVloo
	//3.XA
	{"guyvloo", true}, //XA_GuyVloo
	{"midnight", true},   //XA_MidnightVloo
	//4.XA
	{"amusialullaby", true}, //XA_AmusiaLullaby
	{"deathtolllullaby", true},   //XA_DeathTollLullaby
	//5.XA
	{"shitnolullaby", true}, //XA_ShitnoLullaby
	{"brimstonelullaby", true},   //XA_BrimstoneLullaby
	//6.XA
	{"yieldmf", true}, //XA_YieldMF
	{"checksummf", true},   //XA_ChecksumMF
	//7.XA
	{"dissonance", true}, //XA_DissonanceMF
	{"pickupmf", true},   //XA_PickUpMF
	//8.XA
	{"dakotamf", true}, //XA_DakotaMF
	{"warforgedmf", true},   //XA_WarforgedMF
	
	{NULL, false}
};
