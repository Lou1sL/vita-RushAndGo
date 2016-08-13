
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <sys/time.h>

#include <psp2/ctrl.h>
#include <psp2/touch.h>
#include <psp2/display.h>
#include <psp2/gxm.h>
#include <psp2/types.h>
#include <psp2/moduleinfo.h>
#include <psp2/kernel/processmgr.h>

#include "utils.h"
#include "draw.h"


void startGame();
void overGame(int score);
void endGame();


int main()
{
	init_video();

	/* Enable analog stick */
	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);

	/* Enable front touchscreen */
	sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, 1);

	/* FPS counting */
	SceUInt64 cur_micros = 0, delta_micros = 0, last_micros = 0;
	uint32_t frames = 0;
	float fps = 0.0f;

	/* Square width anime */
	int w = 940;
	int initw = 1;

	uint32_t color = RGBA8(0, 0, 0, 255);

	/* Input variables */
	SceCtrlData pad;
	SceTouchData touch;

	while (1) {
		clear_screen();

		/* Read controls and touchscreen */
		sceCtrlPeekBufferPositive(0, &pad, 1);
		sceTouchPeek(0, &touch, 1);


		/* Touchscreen! */
		if (touch.reportNum > 0) {
			/* Front touchscreen: 1920x1088 */

			int touchx = lerp(touch.report[0].x, 1920, SCREEN_W);
			int touchy = lerp(touch.report[0].y, 1088, SCREEN_H);

			font_draw_stringf(20, 30, RGBA8(0, 0, 255, 255), "X=%d, Y=%d", touchx, touchy);

			/* Press EXIT */
			if (touchx > 600 && touchx < 835 && touchy > 460 && touchy < 510)endGame();

			/* Press START */
			if (touchx > 300 && touchx < 535 && touchy > 460 && touchy < 510)startGame();


		}
		else{

			font_draw_string_small(20, 30, RGBA8(0, 0, 255, 255), "X=NULL, Y=NULL");
		}

		/* Anime */

		if (initw < w) {
			initw = initw+10;
		}


		int h = 160;

		int x = SCREEN_W / 2 - w / 2;
		int y = SCREEN_H / 2 - h / 2 - 55;

		draw_rectangle(x, y, initw, h, color);

        font_draw_string(200, 200, RGBA8(255, 255, 255, 255), "^_^ Race And Go! ^_^ ");
		font_draw_string_small(600, 310, RGBA8(0, 0, 0, 255), "Programmed via RyuBAI");

		font_draw_stringf(20, 10, RGBA8(255, 0, 0, 255), "FPS: %.2f", fps);

		/* initw/4 = 235 */
		draw_rectangle(300, 450, initw/4, 50, color);

		font_draw_string_small(350, 470, RGBA8(255, 255, 255, 255), "START");

		draw_rectangle(600, 450, initw/4, 50, color);

		font_draw_string_small(650, 470, RGBA8(255, 255, 255, 255), "EXIT");

		/* FPS */
		cur_micros = sceKernelGetProcessTimeWide();

		if (cur_micros >= (last_micros + 1000000)) {
			delta_micros = cur_micros - last_micros;
			last_micros = cur_micros;
			fps = (frames/(double)delta_micros)*1000000.0f;
			frames = 0;
		}



		swap_buffers();
		sceDisplayWaitVblankStart();
		frames++;
	}





	
	return 0;
}


void startGame(){



	/* FPS counting */
	SceUInt64 cur_micros = 0, delta_micros = 0, last_micros = 0;
	uint32_t frames = 0;
	float fps = 0.0f;

	/* Input variables */
	SceCtrlData pad;
	SceTouchData touch;

	/* SCREEN */
	int scrw = 959;
	int scrh = 543;


	/* PLAYER SCORE */
	int SCORE = 0;
	int LIFE = 5;

	/* PLAYER POSI */
	int posiw = 70;
	int posih = 50;
	int posix = 480;
	int posiy = 460;

	//int value = rand() % (MAX + 1 - MIN) + MIN;
	/* Enemy POSI */

	int enew = 50;
	int eneh = 70;
	int enex = rand() % ((scrw-enew) + 1 - 0) + 0;
	int eney = 0;

	/* SPEEDS */
	int ctlspeed = 15;
	int fallspeed = 7;

	while (1) {

		/* GAME OVER */
		if (LIFE == 0){
			overGame(SCORE);
			break;
		}

		if (eney < scrh - eneh) {
			eney = eney + fallspeed;
		}
		else{
			eney = 0;
			enex = rand() % ((scrw - enew) + 1 - 0) + 0;
		}

		if (posix < 0)posix = scrw - posiw;
		if (posix > (scrw - posiw))posix = 0;

		clear_screen();

		/* Read controls and touchscreen */
		sceCtrlPeekBufferPositive(0, &pad, 1);
		sceTouchPeek(0, &touch, 1);



		/* BUTTONS */
		if (pad.buttons & SCE_CTRL_START) {
			

		}
		if (pad.buttons & SCE_CTRL_LTRIGGER) {

			posix = posix - ctlspeed;

		}
		else if (pad.buttons & SCE_CTRL_RTRIGGER) {

			posix = posix + ctlspeed;
		}




		/* Touchscreen! */
		if (touch.reportNum > 0) {
			/* Front touchscreen: 1920x1088 */

			int touchx = lerp(touch.report[0].x, 1920, SCREEN_W);
			int touchy = lerp(touch.report[0].y, 1088, SCREEN_H);

			font_draw_stringf(20, 30, RGBA8(0, 0, 255, 255), "X=%d, Y=%d", touchx, touchy);

		}
		else{

			font_draw_string_small(20, 30, RGBA8(0, 0, 255, 255), "X=NULL, Y=NULL");
		}

		/* Draw */
		draw_rectangle(enex, eney, enew, eneh, RGBA8(0, 0, 0, 255));
		/* PLAYER */
		draw_rectangle(posix, posiy, posiw, posih, RGBA8(0, 0, 0, 255));

		font_draw_stringf(20, 10, RGBA8(255, 0, 0, 255), "FPS: %.2f", fps);

		font_draw_stringf_big(330, 30, RGBA8(0, 0, 0, 255), "SCORE: %d",SCORE);
		font_draw_stringf_big(350, 70, RGBA8(0, 0, 0, 255), "LIFE: %d", LIFE);

		
		/* Y */
		if (posiy < eney + eneh && posiy > eney) {

			/* X */
			if (posix > enex - posiw && posix < enex + enew){
				SCORE++;
			}
			else{

				eney = 0;
				enex = rand() % ((scrw - enew) + 1 - 0) + 0;

				LIFE--;
			}



		}


		/* Calculate FPS */
		cur_micros = sceKernelGetProcessTimeWide();

		if (cur_micros >= (last_micros + 1000000)) {
			delta_micros = cur_micros - last_micros;
			last_micros = cur_micros;
			fps = (frames / (double)delta_micros)*1000000.0f;
			frames = 0;
		}



		swap_buffers();
		sceDisplayWaitVblankStart();
		frames++;


	}

}


void overGame(int score) {

	/* Input variables */
	SceCtrlData pad;
	SceTouchData touch;
	while (1){

		clear_screen();

		/* Read controls and touchscreen */
		sceCtrlPeekBufferPositive(0, &pad, 1);
		sceTouchPeek(0, &touch, 1);


		font_draw_stringf_big(350, 230, RGBA8(0, 0, 0, 255), "SCORE: %d", score);
		font_draw_string_small(250, 350, RGBA8(0, 0, 0, 255), "X_X Touch Screen 2 Return X_X");

		/* Touchscreen! */
		if (touch.reportNum > 0)break;
	

		swap_buffers();
		sceDisplayWaitVblankStart();
	}


	

}

void endGame() {

	end_video();
	sceKernelExitProcess(0);


}

