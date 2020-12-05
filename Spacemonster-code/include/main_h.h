#ifndef INCLUDED_MAIN_H
#define INCLUDED_MAIN_H
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <glad/glad.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <cglm.h>
#include <lua/luaconf.h>
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
#endif

//window size stuff
#define WINDOW_WIDTH_START 1280
#define WINDOW_HEIGHT_START 720
extern unsigned int window_width;
extern unsigned int window_height;

//actually useful macros
#define NDC_TO_PIXELX(x,w) ((((float)w * (x + 1))) / 2)
#define NDC_TO_PIXELY(y,h) (h - (((float)h * (y + 1))) / 2)
#define PIXEL_TO_NDCX(x,w) ((x/(float)w * 2) - 1)
#define PIXEL_TO_NDCY(y,h) (((((float)h - y)/(float)h * 2) - 1))
#define QUAD_TEXTURE_RECT_TEXT_CAL(str) (0.03 * strlen(str))
#define ANIMATESHEET(quad,start,frame) (source_rect.x = start * source_rect.w + frame * source_rect.w)

//enemy flags
//federation - hard
#define FEDERATION_SCOUT 1
#define FEDERATION_FIGHTER 2
#define FEDERATION_CARGO 4
#define FEDERATION_MISSILE_FIGHTER 8
#define FEDERATION_PLANET_ARTY 16
#define FEDERATION_CRUISER 32
#define FEDERATION_FLAGSHIP 64
//civilian - easyish
#define CIVILIAN_RANCHER 128
#define CIVILIAN_CARGO 256 //gives less money and score than federation cargo but easier, and cargo ships have a chance of dropping an item that can be found in the shop
#define CIVILIAN_LAZER 512 //this ship is actually a little harder than the missile fighter in my opinion
#define CIVILIAN_BOMBER 1024 //easy to dodge but can one hit you if it tags you
#define CIVILIAN_MOBSTER 2048
#define CIVILIAN_MOBBOSS 4096 //alternative to the federation flagship, is a little easier

//samething, but they declare the w and h of the ships
//TODO: Do that
//lua state, for lua
extern lua_State* L;
//lua prototypes and stuff
extern void error(lua_State* L, const char* fmt, ...);
//misc
extern float delta_time;
extern SDL_bool overtime_bell_rung;
extern SDL_Surface* wave_text, * overtime_text;
extern TTF_Font* font_1;
extern SDL_Color white;
extern int reserve;
extern char wave_num[2];
extern unsigned int wave, score, cash, active_en, enemy_counter;

//after you beat the civilian or federation flagship you move on to the next galaxy
//linked list stuff
typedef struct listNode listNode;
extern struct listNode {
	listNode* next, * prev;
	void* data;

};
typedef struct linkedList linkedList;
extern struct linkedList {
	listNode* head;
	int count;
};
//game function declarations
extern void GAME_WaveInit(void);
extern void GAME_AddEnemies(void);
extern void GAME_HandleEnemies(const linkedList* const list);

//shape abstractions
typedef struct text_quad text_quad;
typedef struct text_quad_v text_quad_v;
typedef struct quad quad;
typedef struct quad_v quad_v;
//warning, the quads that don't have _v in them have to do math every frame to find out where the rest of the points have to go, could cause slow down if many rectangles do this per frame.
struct text_quad {
	float x, y, w, h;
	GLint textid;
	int texw, texh;
};

struct text_quad_v {
	float vertexes[32];
	GLint textid;
};

struct quad {
	float x, y, w, h;
};

struct quad_v {
	float vertexes[32];
};
//shape abstraction rendering fucntions
extern void RENDER_TexturedQuad(text_quad target, float r, float g, float b, SDL_bool reverse_rendering);
extern void RENDER_TexturedQuadSheet(text_quad target, quad source_rect, float r, float g, float b, SDL_bool normalized, SDL_bool reverse_rendering);
extern void RENDER_Quad(quad target, float r, float g, float b);
extern void RENDER_List(const linkedList* const list);

//shaders
extern GLint shader_texturedobj;
extern GLint shader_colored;
//texture list
extern GLint texture[6];

//enemys
typedef struct personMale personMale;
typedef struct enemy enemy;
struct personMale {
	float vertexes[32];
	int health;
};
//linked list for the enemies
extern linkedList enemies;
struct enemy {
	text_quad sprite;
	unsigned int health, score, id;
};