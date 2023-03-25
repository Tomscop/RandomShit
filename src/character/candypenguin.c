/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "candypenguin.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//CandyPenguin character structure
enum
{
  CandyPenguin_ArcMain_Idle0,
  CandyPenguin_ArcMain_Left0,
  CandyPenguin_ArcMain_Left1,
  CandyPenguin_ArcMain_Down0,
  CandyPenguin_ArcMain_Up0,
  CandyPenguin_ArcMain_Up1,
  CandyPenguin_ArcMain_Right0,
  CandyPenguin_ArcMain_Right1,
  CandyPenguin_ArcMain_IdleP0,
  CandyPenguin_ArcMain_LeftP0,
  CandyPenguin_ArcMain_DownP0,
  CandyPenguin_ArcMain_UpP0,
  CandyPenguin_ArcMain_RightP0,
	
	CandyPenguin_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[CandyPenguin_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_CandyPenguin;

//CandyPenguin character definitions
static const CharFrame char_candypenguin_frame[] = {
  //Candy
  {CandyPenguin_ArcMain_Idle0, {  0,  0, 61,152}, {160,156}}, //0 idle 1
  {CandyPenguin_ArcMain_Idle0, { 61,  0, 62,155}, {160,159}}, //1 idle 2
  {CandyPenguin_ArcMain_Idle0, {123,  0, 63,156}, {160,160}}, //2 idle 3
  {CandyPenguin_ArcMain_Idle0, {186,  0, 63,156}, {160,160}}, //3 idle 4

  {CandyPenguin_ArcMain_Left0, {  0,  0,113,151}, {184,155}}, //4 left 1
  {CandyPenguin_ArcMain_Left0, {113,  0,113,150}, {183,153}}, //5 left 2
  {CandyPenguin_ArcMain_Left1, {  0,  0,114,149}, {183,153}}, //6 left 3
  {CandyPenguin_ArcMain_Left1, {114,  0,115,149}, {183,152}}, //7 left 4

  {CandyPenguin_ArcMain_Down0, {  0,  0,117,122}, {161,127}}, //8 down 1
  {CandyPenguin_ArcMain_Down0, {117,  0,116,124}, {160,129}}, //9 down 2
  {CandyPenguin_ArcMain_Down0, {  0,125,115,125}, {160,130}}, //10 down 3
  {CandyPenguin_ArcMain_Down0, {115,125,114,127}, {160,132}}, //11 down 4

  {CandyPenguin_ArcMain_Up0, {  0,  0,103,172}, {182,176}}, //12 up 1
  {CandyPenguin_ArcMain_Up0, {103,  0,103,170}, {183,175}}, //13 up 2
  {CandyPenguin_ArcMain_Up1, {  0,  0,104,168}, {184,173}}, //14 up 3
  {CandyPenguin_ArcMain_Up1, {104,  0,104,169}, {185,173}}, //15 up 4

  {CandyPenguin_ArcMain_Right0, {  0,  0,108,146}, {170,150}}, //16 right 1
  {CandyPenguin_ArcMain_Right0, {108,  0,107,146}, {169,150}}, //17 right 2
  {CandyPenguin_ArcMain_Right1, {  0,  0,107,146}, {169,150}}, //18 right 3
  {CandyPenguin_ArcMain_Right1, {107,  0,107,146}, {169,150}}, //19 right 4
  
  //Penguin
  {CandyPenguin_ArcMain_IdleP0, {  0,  0, 58, 86}, {160,156}}, //20 idlep 1
  {CandyPenguin_ArcMain_IdleP0, { 58,  0, 58, 89}, {159,159}}, //21 idlep 2
  {CandyPenguin_ArcMain_IdleP0, {116,  0, 57, 89}, {159,159}}, //22 idlep 3
  {CandyPenguin_ArcMain_IdleP0, {173,  0, 58, 90}, {159,160}}, //23 idlep 4
  {CandyPenguin_ArcMain_IdleP0, {  0, 90, 58, 89}, {159,159}}, //24 idlep 5
  {CandyPenguin_ArcMain_IdleP0, { 58, 90, 57, 89}, {159,159}}, //25 idlep 6
  {CandyPenguin_ArcMain_IdleP0, {115, 90, 58, 90}, {159,159}}, //26 idlep 7

  {CandyPenguin_ArcMain_LeftP0, {  0,  0, 77, 88}, {170,160}}, //27 leftp 1
  {CandyPenguin_ArcMain_LeftP0, { 77,  0, 72, 88}, {167,160}}, //28 leftp 2
  {CandyPenguin_ArcMain_LeftP0, {149,  0, 70, 87}, {166,159}}, //29 leftp 3
  {CandyPenguin_ArcMain_LeftP0, {  0, 88, 69, 87}, {165,158}}, //30 leftp 4

  {CandyPenguin_ArcMain_DownP0, {  0,  0, 78, 88}, {169,157}}, //31 downp 1
  {CandyPenguin_ArcMain_DownP0, { 78,  0, 72, 90}, {166,160}}, //32 downp 2
  {CandyPenguin_ArcMain_DownP0, {150,  0, 70, 90}, {165,160}}, //33 downp 3
  {CandyPenguin_ArcMain_DownP0, {  0, 90, 71, 89}, {165,160}}, //34 downp 4
  {CandyPenguin_ArcMain_DownP0, { 71, 90, 70, 89}, {165,160}}, //35 downp 5

  {CandyPenguin_ArcMain_UpP0, {  0,  0, 55, 95}, {157,165}}, //36 upp 1
  {CandyPenguin_ArcMain_UpP0, { 55,  0, 55, 93}, {157,163}}, //37 upp 2
  {CandyPenguin_ArcMain_UpP0, {110,  0, 55, 91}, {158,162}}, //38 upp 3
  {CandyPenguin_ArcMain_UpP0, {165,  0, 56, 91}, {158,161}}, //39 upp 4
  {CandyPenguin_ArcMain_UpP0, {  0, 95, 56, 91}, {158,161}}, //40 upp 5
  {CandyPenguin_ArcMain_UpP0, { 56, 95, 56, 91}, {158,161}}, //41 upp 6

  {CandyPenguin_ArcMain_RightP0, {  0,  0, 79, 85}, {165,155}}, //42 rightp 1
  {CandyPenguin_ArcMain_RightP0, { 79,  0, 80, 85}, {168,154}}, //43 rightp 2
  {CandyPenguin_ArcMain_RightP0, {159,  0, 80, 85}, {170,154}}, //44 rightp 3
  {CandyPenguin_ArcMain_RightP0, {  0, 85, 80, 85}, {170,155}}, //45 rightp 4
  {CandyPenguin_ArcMain_RightP0, { 80, 85, 80, 86}, {170,155}}, //46 rightp 5
};

//Candy
static const Animation char_candypenguin_anim[CharAnim_Max] = {
	{1, (const u8[]){ 0, 0, 0, 1, 1, 1, 2, 2, 3, 3, ASCR_BACK, 1}}, //CharAnim_Idle
	{2, (const u8[]){ 4, 5, 6, 7, 7, ASCR_BACK, 1}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){ 8, 9, 10, 11, 11, ASCR_BACK, 1}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){ 12, 13, 14, 15, 15, ASCR_BACK, 1}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){ 16, 17, 18, 19, 19, ASCR_BACK, 1}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
};
//Penguin
static const Animation char_candypenguin_anim2[CharAnim_Max] = {
	{2, (const u8[]){ 20, 21, 22, 23, 24, 25, 26, ASCR_BACK, 1}}, //CharAnim_Idle
	{2, (const u8[]){ 27, 28, 29, 30, 30, ASCR_BACK, 1}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){ 31, 32, 33, 34, 35, 34, 35, ASCR_BACK, 1}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){ 36, 37, 38, 39, 40, 41, 40, ASCR_BACK, 1}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){ 42, 43, 44, 45, 46, 45, 46, ASCR_BACK, 1}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
};

//CandyPenguin character functions
void Char_CandyPenguin_SetFrame(void *user, u8 frame)
{
	Char_CandyPenguin *this = (Char_CandyPenguin*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_candypenguin_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_CandyPenguin_Tick(Character *character)
{
	Char_CandyPenguin *this = (Char_CandyPenguin*)character;
	
	//Switch
	if ((stage.stage_id == StageId_BiteVreen) && (stage.song_step == 1024))
	{
		this->character.health_bar = 0xFFFFFFFF;
		this->character.health_i = 2;
		Animatable_Init(&this->character.animatable, char_candypenguin_anim2);
	}
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_CandyPenguin_SetFrame);
	if (stage.song_step <= 2031)
		Character_Draw(character, &this->tex, &char_candypenguin_frame[this->frame]);
}

void Char_CandyPenguin_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_CandyPenguin_Free(Character *character)
{
	Char_CandyPenguin *this = (Char_CandyPenguin*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_CandyPenguin_New(fixed_t x, fixed_t y)
{
	//Allocate candypenguin object
	Char_CandyPenguin *this = Mem_Alloc(sizeof(Char_CandyPenguin));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_CandyPenguin_New] Failed to allocate candypenguin object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_CandyPenguin_Tick;
	this->character.set_anim = Char_CandyPenguin_SetAnim;
	this->character.free = Char_CandyPenguin_Free;
	
	Animatable_Init(&this->character.animatable, char_candypenguin_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 1;

	//health bar color
	this->character.health_bar = 0xFF39469C;
	
	this->character.focus_x = FIXED_DEC(65,1);
	this->character.focus_y = FIXED_DEC(-115,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	this->character.size = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\CANDYPEN.ARC;1");
	
	const char **pathp = (const char *[]){
  "idle0.tim",
  "left0.tim",
  "left1.tim",
  "down0.tim",
  "up0.tim",
  "up1.tim",
  "right0.tim",
  "right1.tim",
  "idlep0.tim",
  "leftp0.tim",
  "downp0.tim",
  "upp0.tim",
  "rightp0.tim",
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
