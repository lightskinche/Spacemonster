#include "main_h.h"

void RENDER_List(linkedList* const list) {
	for (int i = 0; i < list->count; ++i) {
		enemy* tmp_enemy = LIST_At(&enemies, i);
		if (tmp_enemy) {
			text_quad* sprite = &tmp_enemy->sprite;
			RENDER_TexturedQuad(tmp_enemy->sprite, 1, 1, 1, true);
			if (tmp_enemy->sprite.x < -1) {//remove any enimies that are out of bounds
				LIST_RemoveAt(list, i, 1);
				continue;
			}
		}
	}
}
void RENDER_TexturedQuad(text_quad target, float r, float g, float b, SDL_bool reverse_rendering) {
	float x = target.x, y = target.y, w = target.w, h = target.h;
	if (reverse_rendering) {
		float tmp_vertexes[32] = { x + w,y,0,r,g,b,0,0, x,y,0,r,g,b,1,0, x,y - h,0,r,g,b,1,1, x + w,y - h,0,r,g,b,0,1 };
		glBufferSubData(GL_ARRAY_BUFFER, NULL, 32 * sizeof(float), tmp_vertexes);
		glBindTexture(GL_TEXTURE_2D, target.textid);
		glUseProgram(shader_texturedobj);
		glDrawArrays(GL_QUADS, 0, 4);
	}
	else {
		float tmp_vertexes[32] = { x,y,0,r,g,b,0,0, x + w,y,0,r,g,b,1,0, x + w,y - h,0,r,g,b,1,1, x,y - h,0,r,g,b,0,1 };
		glBufferSubData(GL_ARRAY_BUFFER, NULL, 32 * sizeof(float), tmp_vertexes);
		glBindTexture(GL_TEXTURE_2D, target.textid);
		glUseProgram(shader_texturedobj);
		glDrawArrays(GL_QUADS, 0, 4);
	}
}
void RENDER_TexturedQuadSheet(text_quad target, quad source_rect, float r, float g, float b, SDL_bool normalized, SDL_bool reverse_rendering) {
	float x = target.x, y = target.y, w = target.w, h = target.h;
	float textx, textw;
	float texty, texth;
	if (normalized)
		textx = source_rect.x, texty = source_rect.y, textw = source_rect.w, texth = source_rect.h;
	else
		textx = source_rect.x / target.texw, texty = source_rect.y / target.texh, textw = source_rect.w / target.texw, texth = source_rect.h / target.texh;

	if (reverse_rendering) {
		float tmp_vertexes[32] = { x + w,y,0,r,g,b,textx,texty, x,y,0,r,g,b,textx + textw,texty, x,y - h,0,r,g,b,textx + textw,textx + texth, x + w,y - h,0,r,g,b,textx,textx + texth };
		glBufferSubData(GL_ARRAY_BUFFER, NULL, 32 * sizeof(float), tmp_vertexes);
		glBindTexture(GL_TEXTURE_2D, target.textid);
		glUseProgram(shader_texturedobj);
		glDrawArrays(GL_QUADS, 0, 4);
	}
	else {
		//this is so unreadable, definitly clean this up later
		float tmp_vertexes[32] = { x,y,0,r,g,b,textx,texty, x + w,y,0,r,g,b,textx + textw,source_rect.y, x + w,y - h,0,r,g,b,textx + textw,texty + texth, x,y - h,0,r,g,b,textx,texty + texth };
		glBufferSubData(GL_ARRAY_BUFFER, NULL, 32 * sizeof(float), tmp_vertexes);
		glBindTexture(GL_TEXTURE_2D, target.textid);
		glUseProgram(shader_texturedobj);
		glDrawArrays(GL_QUADS, 0, 4);
	}
}
void RENDER_Quad(quad target, float r, float g, float b) {
	float x = target.x, y = target.y, w = target.w, h = target.h;
	float tmp_vertexes[32] = { x,y,0,r,g,b,0,0, x + w,y,0,r,g,b,1,0, x + w,y - h,0,r,g,b,1,1, x,y - h,0,r,g,b,0,1 };
	glBufferSubData(GL_ARRAY_BUFFER, NULL, 32 * sizeof(float), tmp_vertexes);
	glUseProgram(shader_colored);
	glDrawArrays(GL_QUADS, 0, 4);
}
