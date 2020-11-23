#include "main_h.h"

//Globals
SDL_Window* window;
SDL_GLContext glcontext;
SDL_Surface* test_font_surface;
unsigned int window_width = WINDOW_WIDTH_START, window_height = WINDOW_HEIGHT_START;
unsigned int wave = 1, score = 0, cash, active_en = FEDERATION_SCOUT, enemy_counter = 1;
int reserve = 0;
linkedList enemies;
unsigned int texturesizeswh[2];
//openGL stuff
GLint texture[5];
GLint VAO;
GLint VERTEXES_VBO;
//audio
Mix_Chunk* overtime_bell_audio;
//text renderering
SDL_Color white = { 255,255,255,255 };
TTF_Font* font_1;
SDL_Surface* wave_text;
SDL_Surface* overtime_text;
text_quad wave_text_quad = { 0,1,0,0.1 };
text_quad overtime_quad = { 0,0.8,0,0.1 };
char wave_num[2];
SDL_bool overtime_bell_rung = false;
//shaders
GLint shader_texturedobj;
GLint shader_colored;
//player stuff
text_quad player_quad;
quad source_rect_nothing = { 0,0,1,1 };
float player_wasd_speed = 1.35;
//syncing
float delta_time = 0;
float start;
//misc
float background_vertexes[32] = { -1,1,0,1,1,1,0,0, 1,1,0,1,1,1,1,0, 1,-1,0,1,1,1,1,1, -1,-1,0,1,1,1,0,1 };
SDL_Cursor* mouse_opened;
SDL_Cursor* mouse_closed;
//Prototypes
GLint CompileShader(char* shader_fname, GLenum type);
void SetTextureBoundedParams(GLenum sampler_target, GLenum filter, GLfloat repeat_type);
void GAME_WaveInit(void);
void GAME_AddEnemies(void);
void RENDER_List(const linkedList* const list);
void CreateTexture2D(SDL_Surface* tmp_surface, GLenum format, SDL_bool free_surface, int* w, int* h);
void Init_GL(void);

int main(void) {
	//Inititlze SDL stuff
	//TODO: rename some globals from "test" and choose better var names, also get controller support and haptic support before you start adding items and stuff
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_HAPTIC | SDL_INIT_JOYSTICK)) {
		printf("Couldn't inilize audio, %s", SDL_GetError());
	}
	if (TTF_Init() == -1) {
		puts("Failed to load SDL_TTF font library");
	}
	if ((Mix_Init(MIX_INIT_MP3) & MIX_INIT_MP3) != MIX_INIT_MP3) {
		printf("Failed to initilize mix, %s\n", Mix_GetError());
	}
	//set up fonts, load them and turn them into surfaces here
	font_1 = TTF_OpenFont("resources/font_1.ttf", 50);
	wave_text = TTF_RenderText_Blended(font_1, "Wave:01", white);
	overtime_text = TTF_RenderText_Blended(font_1, "OVERTIME", white);
	wave_text_quad.w = QUAD_TEXTURE_RECT_TEXT_CAL("Wave:01"), wave_text_quad.x = 0 - (QUAD_TEXTURE_RECT_TEXT_CAL("Wave:01") / 2);
	overtime_quad.w = QUAD_TEXTURE_RECT_TEXT_CAL("OVERTIME"), overtime_quad.x = 0 - (QUAD_TEXTURE_RECT_TEXT_CAL("OVERTIME") / 2);
	//set up audio
	if(Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 2048) < 0) {
		printf("Error opening audio device, %s\n", Mix_GetError());
	}
	printf("amount of channels: %d\n", Mix_AllocateChannels(32));
	//???
	int number_decoders = Mix_GetNumChunkDecoders();
	for (int i = 0; i < number_decoders; ++i)
		printf("Sample chunk decoder %d is for %s\n", i, Mix_GetChunkDecoder(i));
	//set up any audio loading here
	overtime_bell_audio = Mix_LoadWAV("resources/overtime_bell.wav");
	Mix_VolumeChunk(overtime_bell_audio, 64);
	//print out the current hardware format
	int freq, channels;
	Uint16 format;
	if (Mix_QuerySpec(&freq, &format, &channels)) {
		printf("Frequerncy:%d, audio format:%x, number of channels:%d\n", freq, format, channels);
	}
	else
	{
		printf("Something bad happened...%s\n", Mix_GetError());
	}
	window = SDL_CreateWindow("SpaceMonster", 0, 100, WINDOW_WIDTH_START, WINDOW_HEIGHT_START, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!window) {
		puts("Window creation failed!");
	}
    glcontext = SDL_GL_CreateContext(window);
	if (!glcontext) {
		puts("SDL/OPENGL thing failed!");
	}
	if (!gladLoadGL()) {
		puts("Failed to load openGL");
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glViewport(0, 0, WINDOW_WIDTH_START, WINDOW_HEIGHT_START);
	glOrtho(0, WINDOW_WIDTH_START, 0, WINDOW_HEIGHT_START, 1.0, -1.0);
	mat4 ortho;
	glm_ortho(0, WINDOW_WIDTH_START, 0, WINDOW_HEIGHT_START, 1.0, -1.0, ortho);
	if (glGetError()) {
		printf("Failed to initilize openGL, %x", glGetError());
	}
	shader_texturedobj = glCreateProgram();
	shader_colored = glCreateProgram();
	{
		GLint shader_vertex;
		GLint shader_fragment;
		shader_vertex = CompileShader("vertex1.txt", GL_VERTEX_SHADER);
		shader_fragment = CompileShader("fragment1.txt", GL_FRAGMENT_SHADER);
		glAttachShader(shader_texturedobj, shader_vertex);
		glAttachShader(shader_texturedobj, shader_fragment);
		glDeleteShader(shader_vertex);
		glDeleteShader(shader_fragment);
	}        
	{
		GLint shader_vertex;
		GLint shader_fragment;
		shader_vertex = CompileShader("vertex2.txt", GL_VERTEX_SHADER);
		shader_fragment = CompileShader("fragment2.txt", GL_FRAGMENT_SHADER);
		glAttachShader(shader_colored, shader_vertex);
		glAttachShader(shader_colored, shader_fragment);
		glDeleteShader(shader_vertex);
		glDeleteShader(shader_fragment);
	}
	glLinkProgram(shader_texturedobj);
	glLinkProgram(shader_colored);
	if (glGetError()) {
		printf("Failed to initilize shaders, %x", glGetError());
	}
	//setting global light
	{
		glUseProgram(shader_colored);
		GLint loc = glGetUniformLocation(shader_colored, "global_light");
		glUniform3f(loc, 1, 1, 1);
		glUseProgram(shader_texturedobj);
		loc = glGetUniformLocation(shader_texturedobj, "global_light");
		glUniform3f(loc, 1, 1, 1);
		if (glGetError()) {
			printf("Failed to initilize shader uniforms, %x", glGetError());
		}
		//unless you need it for somereason
		//GLint tex0 = glGetUniformLocation(shader_texturedobj, "Texture0");
		//GLint tex1 = glGetUniformLocation(shader_texturedobj, "Texture1");
		//glUniform1i(tex0, 0);
		//glUniform1i(tex1, 1);
	}
	glUseProgram(shader_texturedobj);
	Init_GL();
	GAME_WaveInit();
	//other misc texture init
	player_quad.x = -1.0, player_quad.y = 1.0, player_quad.w = 0.1, player_quad.h = 0.2;
	player_quad.textid = texture[0];
	wave_text_quad.textid = texture[3];
	wave_num[0] = '0', wave_num[1] = '1';
	overtime_quad.textid = texture[4];
	glClearColor(0, 0, 0, 1);
	//Stuff that needs to be outside of the while loop
	Uint8* key_input = SDL_GetKeyboardState(NULL);
	SDL_Event event_handle;
	while (1) {
		start = clock();
		if (SDL_PollEvent(&event_handle)) {
			if (event_handle.type == SDL_WINDOWEVENT) {
				if (event_handle.window.event == SDL_WINDOWEVENT_CLOSE) {
					goto SHUTDOWN;
				}
				if (event_handle.window.event == SDL_WINDOWEVENT_RESIZED) {
					glViewport(0, 0, event_handle.window.data1, event_handle.window.data2);
					glOrtho(0, event_handle.window.data1, 0, event_handle.window.data2, 1.0, -1.0);
					glm_ortho(0, event_handle.window.data1, 0, event_handle.window.data2, 1.0, -1.0, ortho);
					window_width = event_handle.window.data1, window_height = event_handle.window.data2;
				}
			}
		}

		glClear(GL_COLOR_BUFFER_BIT);
		//cycle the background
		background_vertexes[6] = background_vertexes[6] + 0.5 * delta_time, background_vertexes[14] = background_vertexes[14] + 0.5 * delta_time;
		background_vertexes[22] = background_vertexes[22] + 0.5 * delta_time, background_vertexes[30] = background_vertexes[30] + 0.5 * delta_time;
		if (background_vertexes[6] > 1)
			background_vertexes[6] = 0, background_vertexes[14] = 1, background_vertexes[22] = 1, background_vertexes[30] = 0;
		glBufferSubData(GL_ARRAY_BUFFER, NULL, 32 * sizeof(float), background_vertexes);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glDrawArrays(GL_QUADS, 0, 4);
		//input processing
		if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
			SDL_SetCursor(mouse_closed);
		}
		else {
			SDL_SetCursor(mouse_opened);
		}
		if (key_input[SDL_SCANCODE_UP]) {
			++score;
		}
		if (key_input[SDL_SCANCODE_DOWN]) {
			printf("score is now: %d\n", score);
		}
		if (key_input[SDL_SCANCODE_D]) {
			player_quad.x = player_quad.x + player_wasd_speed * delta_time;
		}
		if (key_input[SDL_SCANCODE_A]) {
			player_quad.x = player_quad.x - player_wasd_speed * delta_time;
		}
		if (key_input[SDL_SCANCODE_S]) {
			player_quad.y = player_quad.y - player_wasd_speed * delta_time;
		}
		if (key_input[SDL_SCANCODE_W]) {
			player_quad.y = player_quad.y + player_wasd_speed * delta_time;
		}
		//make sure player is not outside of the window
		if (player_quad.x < -1) {
			player_quad.x = -1;

		}
		else if (player_quad.x + player_quad.w > 1) {
			player_quad.x = 0.9;

		}
		if (player_quad.y > 1) {
			player_quad.y = 1;

		}
		else if (player_quad.y < -1 + player_quad.h) {
			player_quad.y = -0.8;

		}
		//check if wave should increase
		if (score == (1000 * wave))
			++wave, GAME_WaveInit(), printf("score requirement: %d\n", 1000 * wave);
		GAME_AddEnemies();
		RENDER_TexturedQuad(player_quad,1,1,1, false);
		RENDER_List(&enemies);
		RENDER_TexturedQuad(wave_text_quad, 1, 0.1, 0.1, false);
		if (reserve < 0) {
			RENDER_TexturedQuad(overtime_quad, 1, 0.9, 0.1, false);
			if (!overtime_bell_rung) {
				Mix_PlayChannel(-1, overtime_bell_audio, 0);
				overtime_bell_rung = true;
			}
		}
		if (glGetError()) {
			printf("Failed to do something, %x", glGetError());
		}
		SDL_GL_SwapWindow(window);
		delta_time = clock() - start;
		delta_time /= CLOCKS_PER_SEC;
		if (key_input[SDL_SCANCODE_F]) {
			printf("time:%f\n", delta_time);
		}
	}	
SHUTDOWN:
	return 0;
}

GLint CompileShader(char* shader_fname, GLenum type) {
	char* shader_data;
	GLint shader_obj;
	size_t size;
	int linebreak_amount = 0;
	{
		FILE* fp; 
		fp = fopen(shader_fname, "r");
		fseek(fp, SEEK_SET, SEEK_END);
		size = ftell(fp);
		fseek(fp, SEEK_SET, SEEK_SET);
		shader_data = calloc(1, size);
		fread(shader_data, 1, size, fp);
		fclose(fp);
		for (int i = 0; i < size; ++i) {
			if (shader_data[i] == '\n') {
				++linebreak_amount;
			}
		}
		shader_data[size - linebreak_amount] = '\0';
		//printf("%s", shader_data);
		//printf("%d", size);
		//for debugging
	}
	shader_obj = glCreateShader(type);
	glShaderSource(shader_obj, 1, &shader_data, NULL);
	glCompileShader(shader_obj);
	free(shader_data);
	return shader_obj;
}
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
	glBindTexture(GL_TEXTURE_2D, texture[3]);
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
		unsigned int tmp = rand() % enemy_counter, tmp_bitshift = 0;
		tmp_bitshift = FEDERATION_SCOUT << tmp;
		if (!(active_en & tmp_bitshift))
			tmp_bitshift = CIVILIAN_RANCHER << tmp;

		text_quad tmp_text_quad = { 0.9,PIXEL_TO_NDCY((float)(rand() % window_height), window_height),0.1,0.1,texture[0],0,0 }; //texture width and height not relevant... yet
		enemy* tmp_enemy = calloc(1, sizeof(enemy));
		tmp_enemy->sprite = tmp_text_quad, tmp_enemy->health = 20 * tmp_bitshift, tmp_enemy->score = 10 * tmp_bitshift * 3, tmp_enemy->id = tmp_bitshift;
		LIST_AddElement(&enemies, tmp_enemy);
		reserve -= tmp_bitshift;
		printf("reserve:%d\n", reserve);
	}
	else if (time_withoutreserve >= 100 && !(time % 2) && timer <= 0) {
		goto ADDEN;
	}
	else if (reserve <= 0) {
		time_withoutreserve = time_withoutreserve + 1;
	}
		
	--timer;
}
void RENDER_List(const linkedList* const list) {
	for (int i = 0; i < list->count; ++i) {
		enemy* tmp_enemy = LIST_At(&enemies, i);
		RENDER_TexturedQuad(tmp_enemy->sprite, 1, 1, 1, true); 
	}
}
void RENDER_TexturedQuad(text_quad target, float r, float g, float b, SDL_bool reverse_rendering) {
	float x = target.x, y = target.y, w = target.w, h = target.h;
	if (reverse_rendering) {
		float tmp_vertexes[32] = { x+w,y,0,r,g,b,0,0, x,y,0,r,g,b,1,0, x,y - h,0,r,g,b,1,1, x + w,y - h,0,r,g,b,0,1 };
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
		float tmp_vertexes[32] = { x + w,y,0,r,g,b,textx,texty, x,y,0,r,g,b,textx + textw,texty, x,y - h,0,r,g,b,textx + textw,textx + texth, x + w,y - h,0,r,g,b,textx,textx + texth};
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
void SetTextureBoundedParams(GLenum sampler_target, GLenum filter, GLfloat repeat_type) {
	glTexParameteri(sampler_target, GL_TEXTURE_WRAP_S, repeat_type);
	glTexParameteri(sampler_target, GL_TEXTURE_WRAP_T, repeat_type);
	glTexParameteri(sampler_target, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(sampler_target, GL_TEXTURE_MAG_FILTER, filter);
}
void CreateTexture2D(SDL_Surface* tmp_surface, GLenum format, SDL_bool free_surface, int* w, int* h) {
	glTexImage2D(GL_TEXTURE_2D, 0, format, tmp_surface->w, tmp_surface->h, 0, format, GL_UNSIGNED_BYTE, tmp_surface->pixels);
	if(w)
		w = tmp_surface->w;
	if(h)
		h = tmp_surface->h;
	if (free_surface)
		SDL_FreeSurface(tmp_surface);
}
//here just so that it can clean up the main function, this is initlizing stuff that has to do with graphics
void Init_GL(void){
	SDL_Surface* tmp_surface = IMG_Load("resources/character_1.png");
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VERTEXES_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VERTEXES_VBO);
	glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//textures, player texture
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(4, texture);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	CreateTexture2D(tmp_surface, GL_RGBA, true, &player_quad.texw, &player_quad.texh);
	//first bacground
	tmp_surface = IMG_Load("resources/background_1.png");
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	CreateTexture2D(tmp_surface, GL_RGB, true, NULL, NULL);
	/*tmp_surface = IMG_Load("fed_scout.png");
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	CreateTexture2D(tmp_surface, GL_RGBA, true);*/
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	CreateTexture2D(wave_text, GL_RGBA, true, NULL, NULL); //but we want to keep the surface around so we can change the text after every wave
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	CreateTexture2D(overtime_text, GL_RGBA, true, NULL, NULL); 
	tmp_surface = IMG_Load("resources/character_ic.png"); //for the program icon
	SDL_SetWindowIcon(window, tmp_surface);
	SDL_FreeSurface(tmp_surface);
	tmp_surface = IMG_Load("resources/cursor_f_open.png"); //for the mouse when left mouse button is not pressed
	mouse_opened  = SDL_CreateColorCursor(tmp_surface, 0, 0);
	SDL_FreeSurface(tmp_surface);
	tmp_surface = IMG_Load("resources/cursor_f_closed.png"); //for the mouse when left mouse button is pressed
	mouse_closed = SDL_CreateColorCursor(tmp_surface, 0, 0);
	SDL_FreeSurface(tmp_surface);

}