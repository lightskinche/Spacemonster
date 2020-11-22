#include "main_h.h"

//Globals
SDL_Window* window;
SDL_GLContext glcontext;
SDL_Surface* test_font_surface;
unsigned int window_width = WINDOW_WIDTH_START, window_height = WINDOW_HEIGHT_START;
//text renderering
SDL_Color white = { 255,255,255,255 };
TTF_Font* test_font;
//shaders
GLint shader_texturedobj;
GLint shader_colored;
//openGL stuff
GLint texture[3];
GLint VAO;
GLint VERTEXES_VBO;
//player stuff
text_quad player_quad;
float player_wasd_speed = 1.35;
//syncing
float delta_time = 0;
float start;
//misc
float background_vertexes[32] = { -1,1,0,1,1,1,0,0, 1,1,0,1,1,1,1,0, 1,-1,0,1,1,1,1,1, -1,-1,0,1,1,1,0,1 };
SDL_Cursor* mouse_opened;
SDL_Cursor* mouse_closed;
linkedList test_list;
Uint8 left = 127;
//Prototypes
GLint CompileShader(char* shader_fname, GLenum type);
void SetTextureBoundedParams(GLenum sampler_target, GLenum filter, GLfloat repeat_type);
void MoveShapeXY(float x, float y, int number_of_vertexes, int stride, float* vertex_data);
void RENDER_List(const linkedList* const list);
void CreateTexture2D(SDL_Surface* tmp_surface, GLenum format, SDL_bool free_surface);
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
	//set up fonts
	test_font = TTF_OpenFont("testfont.ttf", 50);
	if (!test_font) {
		puts("Error loading true type font");
	}
	char* test_text_string = "what do you want it to be?";
	test_font_surface = TTF_RenderText_Blended(test_font, test_text_string, white);
	if (!test_font_surface) {
		puts("Failed to create SDL_Surface from TTF");
	}
	//set up audio
	if(Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 2048) < 0) {
		printf("Error opening audio device, %s\n", Mix_GetError());
	}
	printf("amount of channels: %d\n", Mix_AllocateChannels(32));
	//???
	int number_decoders = Mix_GetNumChunkDecoders();
	for (int i = 0; i < number_decoders; ++i)
		printf("Sample chunk decoder %d is for %s\n", i, Mix_GetChunkDecoder(i));

	Mix_Chunk* stero_test;
	stero_test = Mix_LoadWAV("testa.wav");
	Mix_PlayChannel(0, stero_test, -1);
	//load in audio data, turns out you use Mix_Music for large files
	Mix_Music* test, *other_test;
	test = Mix_LoadMUS("test.wav"), other_test = Mix_LoadMUS("testb.wav");
	if (!test) {
		printf("Failed to load WAV, %s\n", Mix_GetError());
	}
	Mix_SetMusicPosition(60);
	Mix_VolumeMusic(64);
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
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
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
	player_quad.x = -1.0, player_quad.y = 1.0, player_quad.w = 0.1, player_quad.h = 0.2;
	player_quad.textid = texture[0];
	text_quad text_test_quad = { 0,0,QUAD_TEXTURE_RECT_TEXT_CAL(test_text_string),0.1, texture[2] };
	//create a lot of enemys
	test_list.head = NULL;
	for (int i = 0; i < 10; ++i) {
		float tmp_vertexes[32] = { -0.2 + (float)i / 20,1,0,1,1,1,0,0, -0.25 + (float)i / 20,1,0,1,1,1,1,0, -0.25 + (float)i / 20,0.9,0,1,1,1,1,1, -0.2 + (float)i / 20,0.9,0,1,1,1,0,1 };
		personMale* test_person = calloc(1, sizeof(personMale));
		memcpy(test_person->vertexes, tmp_vertexes, 32 * sizeof(float));
		test_person->health = 100;
		LIST_AddElement(&test_list, test_person);
	    
	}
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
		if (key_input[SDL_SCANCODE_RIGHT]) {
			text_test_quad.x = text_test_quad.x + player_wasd_speed * delta_time;
		}
		if (key_input[SDL_SCANCODE_LEFT]) {
			text_test_quad.x = text_test_quad.x - player_wasd_speed * delta_time;
		}
		if (key_input[SDL_SCANCODE_UP]) {
			left++;
		}
		if (key_input[SDL_SCANCODE_DOWN]) {
			left--;
		}
		Mix_SetPanning(0, left, 255 - left);
		if (key_input[SDL_SCANCODE_SPACE]) {
			Mix_PlayMusic(test, 1);
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
		//TODO ADD JUMP PYHSICS AND ACTUAL PYHSICS
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
		quad test_colored = { 0,0,0.2,0.2 };
		quad test_colored2 = { 0,-0.2, 0.2, 0.2 };
		quad test_colored3 = { 0,-0.4, 0.2, 0.2 };
		quad source_rect = { 0,0,1,1 };
		RENDER_TexturedQuadSheet(player_quad,source_rect,1,1,1, false);
		RENDER_Quad(test_colored, 1, 0, 0);
		RENDER_Quad(test_colored2, 0, 1, 0);
		RENDER_Quad(test_colored3, 0, 0, 1);
		RENDER_TexturedQuad(text_test_quad, 0.05, 1, 0.1, false);
		RENDER_List(&test_list);
		//render text
		//printf("time:%f\n", delta_time);
		SDL_GL_SwapWindow(window);
		delta_time = clock() - start;
		delta_time /= CLOCKS_PER_SEC;
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
void RENDER_List(const linkedList* const list) {
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glUseProgram(shader_texturedobj);
	for (int i = 0; i < list->count; ++i) {
		personMale* tmp_person_p = LIST_At(list, i);
		float* tmp_float_p = tmp_person_p->vertexes;
		glBufferSubData(GL_ARRAY_BUFFER, NULL, 32 * sizeof(float), tmp_float_p);
		glDrawArrays(GL_QUADS, 0, 4);
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
void RENDER_TexturedQuadSheet(text_quad target, quad source_rect, float r, float g, float b, SDL_bool reverse_rendering) {
	float x = target.x, y = target.y, w = target.w, h = target.h;
	if (reverse_rendering) {
		float tmp_vertexes[32] = { x + w,y,0,r,g,b,source_rect.x,source_rect.y, x,y,0,r,g,b,source_rect.x + source_rect.w,source_rect.y, x,y - h,0,r,g,b,source_rect.x + source_rect.w,source_rect.y + source_rect.h, x + w,y - h,0,r,g,b,source_rect.x,source_rect.y + source_rect.h };
		glBufferSubData(GL_ARRAY_BUFFER, NULL, 32 * sizeof(float), tmp_vertexes);
		glBindTexture(GL_TEXTURE_2D, target.textid);
		glUseProgram(shader_texturedobj);
		glDrawArrays(GL_QUADS, 0, 4);
	}
	else {
		//this is so unreadable, definitly clean this up later
		float tmp_vertexes[32] = { x,y,0,r,g,b,source_rect.x,source_rect.y, x + w,y,0,r,g,b,source_rect.x + source_rect.w,source_rect.y, x + w,y - h,0,r,g,b,source_rect.x + source_rect.w,source_rect.y + source_rect.h, x,y - h,0,r,g,b,source_rect.x,source_rect.y + source_rect.h };
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
void MoveShapeXY(float x, float y, int number_of_vertexes, int stride, float* vertex_data) {
	for (int i = 0; i <= number_of_vertexes; ++i) {
	    vertex_data[stride * i] += x;
		vertex_data[stride * i + 1] -= y;
	}

}
void CreateTexture2D(SDL_Surface* tmp_surface, GLenum format, SDL_bool free_surface) {
	glTexImage2D(GL_TEXTURE_2D, 0, format, tmp_surface->w, tmp_surface->h, 0, format, GL_UNSIGNED_BYTE, tmp_surface->pixels);
	if (free_surface)
		SDL_FreeSurface(tmp_surface);
}
//here just so that it can clean up the main function
void Init_GL(void){
	//TODO MAYBE ABSTRACT THE VBO SO ITS EASIER TO CREATE OBJECTS
	SDL_Surface* tmp_surface = IMG_Load("character_1.png");
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
	glGenTextures(3, texture);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tmp_surface->w, tmp_surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tmp_surface->pixels);
	SDL_FreeSurface(tmp_surface);
	//first bacground
	tmp_surface = IMG_Load("background_1.png");
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tmp_surface->w, tmp_surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, tmp_surface->pixels);
	SDL_FreeSurface(tmp_surface);
	//for the test text on screen
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	CreateTexture2D(test_font_surface, GL_RGBA, true);
	tmp_surface = IMG_Load("character_ic.png"); //for the program icon
	SDL_SetWindowIcon(window, tmp_surface);
	SDL_FreeSurface(tmp_surface);
	tmp_surface = IMG_Load("cursor_f_open.png"); //for the mouse when left mouse button is not pressed
	mouse_opened  = SDL_CreateColorCursor(tmp_surface, 0, 0);
	SDL_FreeSurface(tmp_surface);
	tmp_surface = IMG_Load("cursor_f_closed.png"); //for the mouse when left mouse button is pressed
	mouse_closed = SDL_CreateColorCursor(tmp_surface, 0, 0);
	SDL_FreeSurface(tmp_surface);

}