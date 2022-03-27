#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES	1

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <bangtal.h>

// 선언

SceneID scene;
ObjectID game_board[16], first_board[16];
ObjectID start, acja;


TimerID timer1, timer2, playtime;

int mixCount;
int j;

//조각 만들기 위한 배열

const char* board_image[16] = {
	"rain_1.png",
	"rain_2.png",
	"rain_3.png",
	"rain_4.png",
	"rain_5.png",
	"rain_6.png",
	"rain_7.png",
	"rain_8.png",
	"rain_9.png",
	"rain_10.png",
	"rain_11.png",
	"rain_12.png",
	"rain_13.png",
	"rain_14.png",
	"rain_15.png",
	"rain_16.png",
};
const int board_x[16] = { 300,450,600,750,300,450,600,750,300,450,600,750,300,450,600,750 };
const int board_y[16] = { 470,470,470,470,320,320,320,320,170,170,170,170,20,20,20,20 };

// 마우스 사용

int blank;

int board_index(ObjectID object) {
	for (int i = 0; i < 16; i++) {
		if (game_board[i] == object) return i;
	}
	return -1;
}

void move(int index) {
	ObjectID t = game_board[blank];
	game_board[blank] = game_board[index];
	game_board[index] = t;

	locateObject(game_board[blank], scene, board_x[blank], board_y[blank]);
	locateObject(game_board[index], scene, board_x[index], board_y[index]);

	blank = index;
}

bool canmove(int index) {
	if (blank < 0 || blank > 15) return false;

	if (blank % 4 != 0 and blank - 1 == index) return true;
	if (blank % 4 != 3 and blank + 1 == index) return true;
	if (blank / 4 != 0 and blank - 4 == index) return true;
	if (blank / 4 != 3 and blank + 4 == index) return true;

	return false;
}

void mixxing() {
	int index;

	do {
		switch (rand() % 4) {
		case 0: index = blank - 1; break;
		case 1: index = blank + 1; break;
		case 2: index = blank - 4; break;
		case 3: index = blank + 4; break;
		}
	} while (!canmove(index));
	move(index);
}


bool clear() {
	for (int i = 0; i < 16; i++) {
		if (game_board[i] != first_board[i]) {
			return false;
		}
	}

	return true;
}


void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == start) {
		mixCount = 10;
		j = 0;
		setTimer(timer1, 1.f);
		startTimer(timer1);
	}

	else {
		int index = board_index(object);
		if (canmove(index))
			move(index);

		if (clear()) {
			showMessage("Completed!!");
			printf("\"\"Your Clear Time is %d seconds.\"\"", j);
			setObjectImage(start, "restart.png");
			showObject(start);
			stopTimer(timer2);
			playtime = createTimer(0.f);
		}
	}
}

void timerCallback(TimerID timer) {
	if (timer == timer1) {
		mixCount--;
		if (mixCount > 0) {
			mixxing();

			timer1 = createTimer(1.f);
			startTimer(timer1);
		}
		else if (mixCount == 0) {
			hideObject(start);
			timer2 = createTimer(1.f);
			startTimer(timer2);
			showTimer(playtime);
			acja = createObject("acja.jpg");
			locateObject(acja, scene, 600, 650);
			scaleObject(acja, 0.1f);
			showObject(acja);
		}
		else {

		}
	}

	else if (timer == timer2) {
		increaseTimer(playtime, 1);
		j++;
		timer2 = createTimer(1.f);
		startTimer(timer2);
	}
}


int main() {

	setMouseCallback(mouseCallback);
	setTimerCallback(timerCallback);

	scene = createScene("Rain", "Raincutted.png");

	for (int i = 0; i < 16; i++) {
		game_board[i] = createObject(board_image[i]);
		first_board[i] = game_board[i];
		locateObject(game_board[i], scene, board_x[i], board_y[i]);
		showObject(game_board[i]);
	}

	blank = 11;
	hideObject(game_board[11]);

	start = createObject("startbutton.png");
	locateObject(start, scene, 540, 70);
	showObject(start);



	timer1 = createTimer(1.f);
	startTimer(timer1);

	playtime = createTimer(0.f);

	startGame(scene);


	return 0;

}