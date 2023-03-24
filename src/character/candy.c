/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "candy.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//Candy character structure
enum
{
  Candy_ArcMain_Idle0,
  Candy_ArcMain_Left0,
  Candy_ArcMain_Left1,
  Candy_ArcMain_Down0,
  Candy_ArcMain_Up0,
  Candy_ArcMain_Up1,
  Candy_ArcMain_Right0,
  Candy_ArcMain_Right1,
	
	Candy_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[Candy_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_Candy;

//Candy character definitions
static const CharFrame char_candy_frame[] = {
  {Candy_ArcMain_Idle0, {  0,  0, 61,152}, {160,155}}, //0 idle 1
  {Candy_ArcMain_Idle0, { 61,  0, 62,155}, {160,158}}, //1 idle 2
  {Candy_ArcMain_Idle0, {123,  0, 63,156}, {160,160}}, //2 idle 3
  {Candy_ArcMain_Idle0, {186,  0, 63,156}, {160,160}}, //3 idle 4

  {Candy_ArcMain_Left0, {  0,  0,113,151}, {184,155}}, //4 left 1
  {Candy_ArcMain_Left0, {113,  0,113,150}, {183,153}}, //5 left 2
  {Candy_ArcMain_Left1, {  0,  0,114,149}, {183,153}}, //6 left 3
  {Candy_ArcMain_Left1, {114,  0,115,149}, {183,152}}, //7 left 4

  {Candy_ArcMain_Down0, {  0,  0,117,122}, {161,127}}, //8 down 1
  {Candy_ArcMain_Down0, {117,  0,116,124}, {160,129}}, //9 down 2
  {Candy_ArcMain_Down0, {  0,125,115,125}, {160,130}}, //10 down 3
  {Candy_ArcMain_Down0, {115,125,114,127}, {160,132}}, //11 down 4

  {Candy_ArcMain_Up0, {  0,  0,103,172}, {182,176}}, //12 up 1
  {Candy_ArcMain_Up0, {103,  0,103,170}, {183,175}}, //13 up 2
  {Candy_ArcMain_Up1, {  0,  0,104,168}, {184,173}}, //14 up 3
  {Candy_ArcMain_Up1, {104,  0,104,169}, {185,173}}, //15 up 4

  {Candy_ArcMain_Right0, {  0,  0,108,146}, {170,150}}, //16 right 1
  {Candy_ArcMain_Right0, {108,  0,107,146}, {169,150}}, //17 right 2
  {Candy_ArcMain_Right1, {  0,  0,107,146}, {169,150}}, //18 right 3
  {Candy_ArcMain_Right1, {107,  0,107,146}, {169,150}}, //19 right 4
};

static const Animation char_candy_anim[CharAnim_Max] = {
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

//Candy character functions
void Char_Candy_SetFrame(void *user, u8 frame)
{
	Char_Candy *this = (Char_Candy*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_candy_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_Candy_Tick(Character *character)
{
	Char_Candy *this = (Char_Candy*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_Candy_SetFrame);
	Character_Draw(character, &this->tex, &char_candy_frame[this->frame]);
}

void Char_Candy_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_Candy_Free(Character *character)
{
	Char_Candy *this = (Char_Candy*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_Candy_New(fixed_t x, fixed_t y)
{
	//Allocate candy object
	Char_Candy *this = Mem_Alloc(sizeof(Char_Candy));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_Candy_New] Failed to allocate candy object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_Candy_Tick;
	this->character.set_anim = Char_Candy_SetAnim;
	this->character.free = Char_Candy_Free;
	
	Animatable_Init(&this->character.animatable, char_candy_anim);
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
	this->arc_main = IO_Read("\\CHAR\\CANDY.ARC;1");
	
	const char **pathp = (const char *[]){
  "idle0.tim",
  "left0.tim",
  "left1.tim",
  "down0.tim",
  "up0.tim",
  "up1.tim",
  "right0.tim",
  "right1.tim",
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
