#include "main_h.h"

void GAME_WaveInit(void) {
	reserve = wave * 20;
	overtime_bell_rung = false;
	printf("reserve is %d\n", reserve);
	if (!(wave % 3)) { // reforcement round
		unsigned int tmp = rand() % 2;
		//can add federation ship or civilian ship based on chance, players should hope for civilian ships
		if (tmp)
			active_en = FEDERATION_SCOUT << (wave / 3), printf("added %d\n", FEDERATION_SCOUT << (wave / 3));
		else
			active_en = FEDERATION_FLAGSHIP << (wave / 3), printf("added %d\n", FEDERATION_FLAGSHIP << (wave / 3)); //the next bit shift will make it a civilian rancher
		enemy_counter++;
	}
	if (!(wave % 5)) { //exotic shop appears every 5 waves
		puts("exotic shop");
	}
	else if (wave > 1) { //opens the normal shop, makes sure that it opens it after you complete the first wave 
		puts("normal shop");
	}
	//update the wave counter onscreen
	char tmp_buffer[8] = "Wave:\0";
	sprintf(wave_num, "%d", wave);
	strcat(tmp_buffer, wave_num);
	wave_text = TTF_RenderText_Blended(font_1, tmp_buffer, white);
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	CreateTexture2D(wave_text, GL_RGBA, true, NULL, NULL);
}
void GAME_AddEnemies(void) {
	static float timer = 0;
	static time_withoutreserve = 0;
	clock_t time = clock() + 1000;
	time /= CLOCKS_PER_SEC;
	if ((timer <= 0 && (!(time % (9 - enemy_counter)))) && reserve > 0) {
	ADDEN:
		timer = 1000; //so that it doesn't place 500 enemys every 9 - enemy_count seconds
		unsigned int tmp = 0;
		if ((enemy_counter - 1) != 0)
			tmp = rand() % (enemy_counter - 1);
		unsigned int tmp_bitshift = FEDERATION_SCOUT << tmp;
		if (!(active_en & tmp_bitshift))
			tmp_bitshift = CIVILIAN_RANCHER << tmp;

		text_quad tmp_text_quad = { 0.9,PIXEL_TO_NDCY((float)(rand() % window_height), window_height),0.175,0.125,texture[3],0,0 }; //texture width and height not relevant... yet
		enemy* tmp_enemy = calloc(1, sizeof(enemy));
		tmp_enemy->sprite = tmp_text_quad, tmp_enemy->health = 20 * tmp_bitshift, tmp_enemy->score = 10 * tmp_bitshift * 3, tmp_enemy->id = tmp_bitshift;
		LIST_AddElement(&enemies, tmp_enemy);
		reserve -= tmp_bitshift;
		printf("reserve:%d\n", reserve);
	}
	else if (time_withoutreserve >= 100 && !(time % 1) && timer <= 0) {
		goto ADDEN;
	}
	else if (reserve <= 0) {
		time_withoutreserve = time_withoutreserve + 1;
	}

	--timer;
}
void GAME_HandleEnemies(const linkedList* const list) {
	for (int i = 0; i < list->count; ++i) {
		enemy* tmp_enemy = LIST_At(&enemies, i);
		if (tmp_enemy) {
			text_quad* sprite = &tmp_enemy->sprite;
			sprite->x = sprite->x - delta_time;
		}
	}
}
