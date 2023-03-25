/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "blank.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//Blank character structure
enum
{
  Blank_ArcMain_Idle0,
  Blank_ArcMain_Idle1,
  Blank_ArcMain_Left0,
  Blank_ArcMain_Left1,
  Blank_ArcMain_Left2,
  Blank_ArcMain_Down0,
  Blank_ArcMain_Down1,
  Blank_ArcMain_Down2,
  Blank_ArcMain_Up0,
  Blank_ArcMain_Up1,
  Blank_ArcMain_Up2,
  Blank_ArcMain_Up3,
  Blank_ArcMain_Up4,
  Blank_ArcMain_Right0,
  Blank_ArcMain_Right1,
  Blank_ArcMain_Right2,
	
	Blank_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[Blank_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_Blank;

//Blank character definitions
static const CharFrame char_blank_frame[] = {
  {Blank_ArcMain_Idle0, {  0,  0,109,118}, {159,158}}, //0 idle 1
  {Blank_ArcMain_Idle0, {109,  0,109,119}, {159,158}}, //1 idle 2
  {Blank_ArcMain_Idle0, {  0,119,109,118}, {159,158}}, //2 idle 3
  {Blank_ArcMain_Idle0, {109,119,109,119}, {159,158}}, //3 idle 4
  {Blank_ArcMain_Idle1, {  0,  0,109,121}, {159,160}}, //4 idle 5
  {Blank_ArcMain_Idle1, {109,  0,110,120}, {160,160}}, //5 idle 6
  {Blank_ArcMain_Idle1, {  0,121,109,120}, {160,160}}, //6 idle 7

  {Blank_ArcMain_Left0, {  0,  0,147,120}, {162,160}}, //7 left 1
  {Blank_ArcMain_Left0, {  0,120,150,119}, {161,159}}, //8 left 2
  {Blank_ArcMain_Left1, {  0,  0,149,120}, {160,160}}, //9 left 3
  {Blank_ArcMain_Left1, {  0,120,151,120}, {161,160}}, //10 left 4
  {Blank_ArcMain_Left2, {  0,  0,151,120}, {161,160}}, //11 left 5

  {Blank_ArcMain_Down0, {  0,  0,162,126}, {198,158}}, //12 down 1
  {Blank_ArcMain_Down0, {  0,127,163,127}, {200,159}}, //13 down 2
  {Blank_ArcMain_Down1, {  0,  0,163,125}, {200,159}}, //14 down 3
  {Blank_ArcMain_Down1, {  0,125,164,125}, {201,160}}, //15 down 4
  {Blank_ArcMain_Down2, {  0,  0,164,125}, {201,160}}, //16 down 5

  {Blank_ArcMain_Up0, {  0,  0,140,129}, {185,165}}, //17 up 1
  {Blank_ArcMain_Up1, {  0,  0,138,128}, {186,163}}, //18 up 2
  {Blank_ArcMain_Up2, {  0,  0,137,131}, {186,164}}, //19 up 3
  {Blank_ArcMain_Up3, {  0,  0,137,129}, {186,163}}, //20 up 4
  {Blank_ArcMain_Up4, {  0,  0,137,129}, {186,163}}, //21 up 5

  {Blank_ArcMain_Right0, {  0,  0,118,136}, {161,173}}, //22 right 1
  {Blank_ArcMain_Right0, {118,  0,120,134}, {165,172}}, //23 right 2
  {Blank_ArcMain_Right1, {  0,  0,122,133}, {167,172}}, //24 right 3
  {Blank_ArcMain_Right1, {122,  0,123,133}, {168,172}}, //25 right 4
  {Blank_ArcMain_Right2, {  0,  0,123,133}, {168,171}}, //26 right 5
};

static const Animation char_blank_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0, 1, 2, 3, 4, 5, 6, ASCR_BACK, 1}}, //CharAnim_Idle
	{2, (const u8[]){ 7, 8, 9, 10, 11, 10, 11, ASCR_BACK, 1}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){ 12, 13, 14, 15, 16, 15, 16, ASCR_BACK, 1}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){ 17, 18, 19, 20, 21, 20, 21, ASCR_BACK, 1}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){ 22, 23, 24, 25, 26, 25, 26, ASCR_BACK, 1}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
};

//Blank character functions
void Char_Blank_SetFrame(void *user, u8 frame)
{
	Char_Blank *this = (Char_Blank*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_blank_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_Blank_Tick(Character *character)
{
	Char_Blank *this = (Char_Blank*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_Blank_SetFrame);
	if (stage.song_step >= 2032)
		Character_Draw(character, &this->tex, &char_blank_frame[this->frame]);
}

void Char_Blank_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_Blank_Free(Character *character)
{
	Char_Blank *this = (Char_Blank*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_Blank_New(fixed_t x, fixed_t y)
{
	//Allocate blank object
	Char_Blank *this = Mem_Alloc(sizeof(Char_Blank));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_Blank_New] Failed to allocate blank object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_Blank_Tick;
	this->character.set_anim = Char_Blank_SetAnim;
	this->character.free = Char_Blank_Free;
	
	Animatable_Init(&this->character.animatable, char_blank_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 3;

	//health bar color
	this->character.health_bar = 0xFF989483;
	
	this->character.focus_x = FIXED_DEC(65,1);
	this->character.focus_y = FIXED_DEC(-115,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	this->character.size = FIXED_DEC(1136,1000);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\BLANK.ARC;1");
	
	const char **pathp = (const char *[]){
  "idle0.tim",
  "idle1.tim",
  "left0.tim",
  "left1.tim",
  "left2.tim",
  "down0.tim",
  "down1.tim",
  "down2.tim",
  "up0.tim",
  "up1.tim",
  "up2.tim",
  "up3.tim",
  "up4.tim",
  "right0.tim",
  "right1.tim",
  "right2.tim",
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
