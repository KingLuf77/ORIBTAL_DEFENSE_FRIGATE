/*
Thank you for downloading ORBITAL DEFENSE FRIGATE!
Made by: KingLuf77

This game was created in 12 hours.

Controls:
  A / LEFT ARROW - strafe left
  D / RIGHT ARROW - strafe right
  W / S / UP ARROW / DOWN ARROW / SPACE - fire railgun
*/

#include<stdio.h>
#include<iostream>
#include<time.h>
#include<conio.h> // <--- keyboard inputs library; only works with Windows.
#include<Windows.h> // <-- used for setConsoleCursorPosition

using namespace std;

#define frameDelay 20 // ms delay between frames. Lower value to incresase difficulty.

int music = 0; // set music = 1 to play music. It will open gameTheme.mp3.
int fastMode = 0; // 1 - skips intro,  2 - shortened intro

#define gameWidth 46 // ORIGINAL: 46
#define gameHeight 24 // ORIGINAL: 24
#define pxStart 20 // player starting position
#define cruiserHealth 2
#define enemyBulletDamage 2
#define enemySpawnHeight 4 // determines how high the enemies spawn
#define enemySpawnDev 6 // adds a random value from zero to this number on how high the enemy spawns
#define enemySpeed 4 // frames per move - higher is slower
#define enemySpeedDev 3 // frames - higher increases randomness
#define enemyFireSpeed 50 // frames per shot (x2) - higher is slower
#define enemyFireDist 7 // distance that an enemy must be in order to shoot - prevents unfair point-blank shots
#define regenSpeed (3 * 60) // frames per hp gained - higher is slower
#define maxHealth 8
int health = maxHealth;
#define reloadTime 2
int reload = reloadTime;
#define playerBulletSpeed 1

int px = pxStart;
int pBulletsY[gameWidth / 4]; // Y value of bullet [ x value of bullet ] - only one bullet per row
int pBullets2[gameWidth / 4]; // we need another bullet per row
int pBullets3[gameWidth / 4]; // had to add another one XD.
int allPixels[gameHeight * gameWidth / 4];
int gameOver = 0;
int cHeight[gameWidth / 4];
int cHealth[gameWidth / 4];
int cMode[gameWidth / 4];
int cBulletx = 0;
int cBullety = -1;
int cBullet2x = 0;
int cBullet2y = -1;
char keyCode = '0';
int score = 0;
int highScore = 0;
int totalFrames = 0;
int deathType = 0;

#define messageDuration 40
int message1 = 0;
int m1dur = 0;
int message2 = 0;
int m2dur = 0;
int message3 = 0;
int m3dur = 0;
int message4 = 0;
int m4dur = 0;
int message5 = 0;
int m5dur = 0;

void gotoxy(int x, int y) {
	COORD pos = { x, y };
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, pos);
}

void printMessage(int m) {
	switch (m) {
	default:
		cout << "                    ";
		break;
	case 1:
		cout << "      You got HIT!  ";
		break;
	case 2:
		cout << "  Enemy DESTROYED!  ";
		break;
	case 3:
		cout << "     Enemy RAMMED!  ";
		break;
	case 4:
		cout << "        +1 HEALTH   ";
		break;
	}
}

void addMessage(int m) {
	message5 = message4;
	m5dur = messageDuration - 20;
	message4 = message3;
	m4dur = messageDuration - 15;
	message3 = message2;
	m3dur = messageDuration - 10;
	message2 = message1;
	m2dur = messageDuration - 5;
	message1 = m;
	m1dur = messageDuration;
}

void resetCruiser(int xNum) { // When a cruiser is destroyed, it goes back to the start
	int randVal = rand() % enemySpawnDev;
	cHeight[xNum] = -enemySpawnHeight - randVal;
	cHealth[xNum] = cruiserHealth;
}

void initializeValues() { // Before a game starts, this is done
	health = maxHealth;
	system("color 0a");
	gameOver = 0;
	totalFrames = 0;
	score = 0;
	px = pxStart;
	reload = reloadTime;
	cBulletx = 0;
	cBullety = -1;
	cBullet2x = 0;
	cBullet2y = -1;
	for (int i = 0; i < gameWidth / 4; i++) {
		resetCruiser(i);
		cHeight[i] -= 2 + rand() % 10;
		pBulletsY[i] = -1;
		pBullets2[i] = -1;
		pBullets3[i] = -1;
	}
	message1 = 0;
	m1dur = 0;
	message2 = 0;
	m2dur = 0;
	message3 = 0;
	m3dur = 0;
	message4 = 0;
	m4dur = 0;
	message5 = 0;
	m5dur = 0;
}

void delay(int delayAmt) {
	clock_t start_time = clock();
	while (clock() < start_time + delayAmt);
}

int slowTypeDelay = 50; // ms delay between letters
void slowType(const char word[], int length) { // Prints a word letter by letter, with a delay between each letter
	for (int i = 0; i < length; i++) {
		printf("%c", word[i]);
		delay(slowTypeDelay);
	}
}

void bigTitle() {
	int lineDelay = 30;
	if (fastMode == 2) {
		lineDelay = 10;
	}
	printf("    _________________ _____ _____ ___   _      ______ ___________ _____ _   _  _____ _____\n"); delay(lineDelay);
	printf("   |  _  | ___ \\ ___ \\_   _|_   _/ _ \\ | |     |  _  \\  ___|  ___|  ___| \\ | |/  ___|  ___|\n"); delay(lineDelay);
	printf("   | | | | |_/ / |_/ / | |   | |/ /_\\ \\| |     | | | | |__ | |_  | |__ |  \\| |\\ `--.| |__ \n"); delay(lineDelay);
	printf("   | | | |    /| ___ \\ | |   | ||  _  || |     | | | |  __||  _| |  __|| . ` | `--. \\  __|\n"); delay(lineDelay);
	printf("   \\ \\_/ / |\\ \\| |_/ /_| |_  | || | | || |____ | |/ /| |___| |   | |___| |\\  |/\\__/ / |___\n"); delay(lineDelay);
	printf("    \\___/\\_| \\_\\____/ \\___/  \\_/\\_| |_/\\_____/ |___/ \\____/\\_|   \\____/\\_| \\_/\\____/\\____/\n"); delay(lineDelay);
	printf("                        ____________ _____ _____   ___ _____ _____\n"); delay(lineDelay);
	printf("                        |  ___| ___ \\_   _|  __ \\ / _ \\_   _|  ___|\n"); delay(lineDelay);
	printf("                        | |_  | |_/ / | | | |  \\// /_\\ \\| | | |__\n"); delay(lineDelay);
	printf("                        |  _| |    /  | | | | __ |  _  || | |  __|\n"); delay(lineDelay);
	printf("                        | |   | |\\ \\ _| |_| |_\\ \\| | | || | | |___\n"); delay(lineDelay);
	printf("                        \\_|   \\_| \\_|\\___/ \\____/\\_| |_/\\_/ \\____/\n");
	if (fastMode != 2) {
		delay(120);
	}
	slowTypeDelay = 12;
	if (fastMode == 2) {
		slowTypeDelay = 5;
	}
	printf("\n                 ");
	slowType("E A R T H ' S   L A S T   L I N E   O F   D E F E N S E", 56);
	if (fastMode != 2) {
		delay(60);
		slowTypeDelay = 35;
	}
	printf("\n\n                                 ");
	slowType("Created by KingLuf77", 21);
	if (fastMode != 2) {
		delay(130);
	}
	printf("\n\n\n");
}

void gameIntro() {
	system("cls");
	slowTypeDelay = 8;
	int lineDelay = 80;
	if (fastMode == 2) {
		lineDelay = 5;
		slowTypeDelay = 2;
	}
	slowType("JAN 19, 2331\n\n", 15); delay(lineDelay);
	slowType("  Foreign space vessels from the Kepler-22 system arrive in attack formation.", 78); delay(lineDelay);
	slowType("\n  You only get one chance to protect Earth.", 45); delay(lineDelay);
	slowType("\n  If any aliens make it to the surface, game over.", 52); delay(lineDelay);
	slowType("\n\n  Your Eagle-Class Space Frigate is equipped with dual ion-thrusters and a 9,000-megajoule railgun.", 102); delay(lineDelay);
	slowType("\n  Use them to your advantage.", 31); delay(lineDelay);
}

void gameInstructions() {
	int lineDelay = 80;
	if (fastMode == 2) {
		lineDelay = 10;
	}
	printf("\n\n   db    This is your ship."); delay(lineDelay);
	printf("\n  dHHb  <--  You are the captain."); delay(lineDelay);
	printf("\n  I  I"); delay(lineDelay);
	printf("\n\n   nn    This is an alien cruiser.");
	printf("\n  dYYb  <--  Destroy it."); delay(lineDelay);
	printf("\n\n  A: strafe left"); delay(lineDelay);
	printf("\n  D: strafe right;"); delay(lineDelay);
	printf("\n  SPACE / W / S: fire railgun"); delay(lineDelay);
	printf("\n\n  Enemies can be rammed, but you will take damage. Use it as a last resort."); delay(lineDelay);
	printf("\n\nPress any key to continue...");
	system("pause >null");
}

void playerControls() {
	if (_kbhit()) {
		keyCode = _getch();
	}
	else {
		keyCode = 'x'; // prevents unwanted repetition
	}
	if (keyCode == -32) {
		keyCode = _getch();
		if (keyCode == 72) {
			keyCode = 'w';
		}
		if (keyCode == 80) {
			keyCode = 's';
		}
		if (keyCode == 75) {
			keyCode = 'a';
		}
		if (keyCode == 77) {
			keyCode = 'd';
		}
	}
	if (keyCode == 'a' || keyCode == 'A') {
		px -= 4;
		if (px < 0) {
			px = 0;
		}
	}
	if (keyCode == 'd' || keyCode == 'D') {
		px += 4;
		if (px > gameWidth - 4) {
			px = gameWidth - 4;
		}
	}
	if (keyCode == ' ' || keyCode == 'w' || keyCode == 'W' || keyCode == 's' || keyCode == 'S') {
		if (reload == reloadTime) {
			if (pBulletsY[px / 4] < 0) {
				reload = -1;
				pBulletsY[px / 4] = gameHeight - 3;
			}
			else if (pBullets2[px / 4] < 0) {
				reload = -1;
				pBullets2[px / 4] = gameHeight - 3;
			}
			else if (pBullets3[px / 4] < 0) {
				reload = -1;
				pBullets3[px / 4] = gameHeight - 3;
			}
		}
	}
}

void randShoot() { // enemy fire
	int availableCruisers = 0;
	for (int i = 0; i < gameWidth / 4; i++) {
		if (cHeight[i] > -1 && cHeight[i] < gameHeight - enemyFireDist) {
			availableCruisers++;
		}
	}
	if (availableCruisers > 0) {
		int randPickCruiser = rand() % availableCruisers;
		int shount = 0;
		int shooterCruiser = 0;
		for (int i = 0; i < gameWidth / 4; i++) {
			if (shount == randPickCruiser) {
				shooterCruiser = i;
			}
			if (cHeight[i] > -1 && cHeight[i] < gameHeight - enemyFireDist) {
				shount++;
			}
		}
		cBulletx = shooterCruiser;
		cBullety = cHeight[shooterCruiser] + 1;
	}
}

void randShoot2() { // enemy fire (alt)
	int availableCruisers = 0;
	for (int i = 0; i < gameWidth / 4; i++) {
		if (cHeight[i] > -1 && cHeight[i] < gameHeight - enemyFireDist) {
			availableCruisers++;
		}
	}
	if (availableCruisers > 0) {
		int randPickCruiser = rand() % availableCruisers;
		int shount = 0;
		int shooterCruiser = 0;
		for (int i = 0; i < gameWidth / 4; i++) {
			if (shount == randPickCruiser) {
				shooterCruiser = i;
			}
			if (cHeight[i] > -1 && cHeight[i] < gameHeight - enemyFireDist) {
				shount++;
			}
		}
		cBullet2x = shooterCruiser;
		cBullet2y = cHeight[shooterCruiser] + 1;
	}
}

void physics() {
	if (totalFrames % regenSpeed == 0 && health < maxHealth) {
		health++;
		addMessage(4);
	}
	if (health > maxHealth) {
		health = maxHealth;
	}
	if (totalFrames % (gameHeight + enemyFireSpeed) == 0) { // Determines rate of enemy fire
		randShoot();
	}
	if (totalFrames % (gameHeight + enemyFireSpeed) == (gameHeight + enemyFireSpeed) / 2) { // Determines rate of enemy fire (alt)
		randShoot2();
	}
	for (int i = 0; i < gameWidth / 4; i++) {
		pBulletsY[i] -= playerBulletSpeed;
		if (pBulletsY[i] < -1) {
			pBulletsY[i] = -1;
		}
		pBullets2[i] -= playerBulletSpeed;
		if (pBullets2[i] < -1) {
			pBullets2[i] = -1;
		}
		pBullets3[i] -= playerBulletSpeed;
		if (pBullets3[i] < -1) {
			pBullets3[i] = -1;
		}
	}
	if (cBullety > -1) {
		cBullety++;
		if (cBullety > gameHeight) {
			cBullety = -1;
		}
	}
	if (cBullet2y > -1) {
		cBullet2y++;
		if (cBullet2y > gameHeight) {
			cBullet2y = -1;
		}
	}
	reload++;
	if (reload > reloadTime) {
		reload = reloadTime;
	}
	for (int i = 0; i < gameWidth / 4; i++) { // enemy moves
		int randVal = rand() % enemySpeedDev;
		int willMove = 0;
		if (cHeight[i] > gameHeight - 7) {
			if (totalFrames % (enemySpeed * 5) == 0) {
				willMove = 1;
			}
		}
		else if (totalFrames % (enemySpeed * 2 + randVal) == 0) {
			willMove = 1;
		}
		if (willMove == 1) {
			cHeight[i]++;
			if (cHeight[i] > gameHeight - 1) {
				gameOver = 1;
				deathType = 1;
				break;
			}
		}
	}
	if (cHeight[px / 4] > gameHeight - 5) { // Detects ramming
		resetCruiser(px / 4);
		health -= cHealth[px / 4];
		addMessage(3);
		score++;
	}
	int warning = 0;
	if (health <= 2) { // Turns screen red when health is low
		system("color 0c");
	}
	else {
		for (int i = 0; i < gameWidth / 4; i++) {
			if (cHeight[i] >= gameHeight - 4) {
				warning = 1;
			}
		}
		if (warning == 1) {
			system("color 06"); // Turns screen yellow if enemy is close
		}
		else {
			system("color 0a"); // Turns screen back to green
		}
	}
	if (cBullety > gameHeight - 3 && cBulletx == px / 4) { // Detects the player being shot by enemy bullet
		health -= enemyBulletDamage;
		addMessage(1);
		cBullety = -1;
	}
	if (cBullet2y > gameHeight - 3 && cBullet2x == px / 4) {
		health -= enemyBulletDamage;
		addMessage(1);
		cBullet2y = -1;
	}
	for (int i = 0; i < gameWidth / 4; i++) {
		if (pBulletsY[i] <= cHeight[i] + 1 && pBulletsY[i] >= 0) {
			cHealth[i]--;
			if (cHealth[i] <= 0) {
				score++;
				addMessage(2);
				resetCruiser(i);
			}
			pBulletsY[i] = -1;
		}
		if (pBullets2[i] <= cHeight[i] + 1 && pBullets2[i] >= 0) {
			cHealth[i]--;
			if (cHealth[i] <= 0) {
				score++;
				addMessage(2);
				resetCruiser(i);
			}
			pBullets2[i] = -1;
		}
		if (pBullets3[i] <= cHeight[i] + 1 && pBullets3[i] >= 0) {
			cHealth[i]--;
			if (cHealth[i] <= 0) {
				score++;
				addMessage(2);
				resetCruiser(i);
			}
			pBullets3[i] = -1;
		}
	}
}

void render(int renderVal) {
	switch (renderVal) {
	case 0:
		cout << "    "; // empty space
		break;
	case 1:
		if (health > 5) {
			cout << " db "; // frigate 1
		}
		else if (health > 1) {
			cout << " ob ";
		}
		else {
			cout << " .b ";
		}
		break;
	case 2:
		if (health > 7) {
			cout << "dHHb";  // frigate 2
		}
		else if (health > 5) {
			cout << "d.Hb";
		}
		else if (health > 3) {
			cout << "d.H.";
		}
		else if (health > 1) {
			cout << "d.h.";
		}
		else {
			cout << "n.l.";
		}
		break;
	case 3:
		if (health > 6) {
			cout << "I  I"; // frigate 3
		}
		else if (health > 4) {
			cout << "I  i";
		}
		else if (health > 2) {
			cout << ";  i";
		}
		else {
			cout << ";  '";
		}
		break;
	case 4:
		cout << " II "; // player bullet
		break;
	case 5:
		cout << " mm "; // enemy cruiser
		break;
	case 6:
		cout << "dYYb"; // enemy cruiser 2
		break;
	case 7:
		cout << " -n "; // enemy cruiser 1 damaged
		break;
	case 8:
		cout << "nY-."; // enemy cruiser 2 damaged
		break;
	case 9:
		cout << ":VV:"; // enemy bullet
		break;
	case 10:
		switch (totalFrames % 6) { // BVR warning
		default:
			cout << "    ";
			break;
		case 1:
			cout << " .. ";
			break;
		case 2:
			cout << ".::.";
			break;
		case 3:
			cout << ":..:";
			break;
		case 4:
			cout << ".::.";
			break;
		case 5:
			cout << " .. ";
			break;
		case 6:
			cout << "    ";
			break;
		}
	}
}

void pixelDef() { // This sets values for the allPixels array; the array that holds values for all "pixels" using my 4:1 2D rendering system.
	for (int scany = 0; scany < gameHeight; scany++) {
		for (int scanx = 0; scanx < gameWidth / 4; scanx++) {
			int toRender = scanx + (scany * gameWidth / 4);
			int renderNum = 0; // default; space
			/*
			if (scany == 0) { // BVR warning system
				if (cHeight[scanx] < -1 && cHeight[scanx] > -6) {
					renderNum = 10;
				}
			}
			*/
			if (scany == cHeight[scanx]) { // cruiser 1
				renderNum = 5;
				if (cHealth[scanx] == 1 && cruiserHealth != 1) {
					renderNum = 7;
				}
			}
			if (scany == cHeight[scanx] + 1) { // cruiser 2
				renderNum = 6;
				if (cHealth[scanx] == 1 && cruiserHealth != 1) {
					renderNum = 8;
				}
			}
			if (scany == pBulletsY[scanx]) { // player bullet
				renderNum = 4;
			}
			if (scany == pBullets2[scanx]) { // player bullet
				renderNum = 4;
			}
			if (scany == pBullets3[scanx]) { // player bullet
				renderNum = 4;
			}
			if (scany == gameHeight - 3 && scanx == px / 4) { // Player ship
				renderNum = 1;
			}
			if (scany == gameHeight - 2 && scanx == px / 4) {
				renderNum = 2;
			}
			if (scany == gameHeight - 1 && scanx == px / 4) {
				renderNum = 3;
			}
			if (totalFrames < 8) { // Ship ascends into play area (1)
				if (scany == gameHeight - 3 && scanx == px / 4) {
					renderNum = 0;
				}
				if (scany == gameHeight - 2 && scanx == px / 4) {
					renderNum = 0;
				}
				if (scany == gameHeight - 1 && scanx == px / 4) {
					renderNum = 0;
				}
			}
			else if (totalFrames < 10) { // Ship ascends into play area (2)
				if (scany == gameHeight - 3 && scanx == px / 4) {
					renderNum = 0;
				}
				if (scany == gameHeight - 2 && scanx == px / 4) {
					renderNum = 0;
				}
				if (scany == gameHeight - 1 && scanx == px / 4) {
					renderNum = 1;
				}
			}
			else if (totalFrames < 12) { // Ship ascends into play area (3)
				if (scany == gameHeight - 3 && scanx == px / 4) {
					renderNum = 0;
				}
				if (scany == gameHeight - 2 && scanx == px / 4) {
					renderNum = 1;
				}
				if (scany == gameHeight - 1 && scanx == px / 4) {
					renderNum = 2;
				}
			}
			if (scany == cBullety && scanx == cBulletx) {
				renderNum = 9;
			}
			if (scany == cBullet2y && scanx == cBullet2x) {
				renderNum = 9;
			}
			if (scany == cBullety && scanx == cBulletx) {
				renderNum = 9;
			}
			if (scany == cBullet2y && scanx == cBullet2x) {
				renderNum = 9;
			}
			allPixels[toRender] = renderNum;
		}
	}
}
void messageHandler() { // handles the ingame messages that appear on the bottom right corner
	m1dur--;
	m2dur--;
	m3dur--;
	m4dur--;
	m5dur--;
	if (m5dur <= 0) {
		message5 = 0;
	}
	if (m4dur <= 0) {
		message4 = 0;
	}
	if (m3dur <= 0) {
		message3 = 0;
	}
	if (m2dur <= 0) {
		message2 = 0;
	}
	if (m1dur <= 0) {
		message1 = 0;
	}
}

void addSpaces(int spaceAmt) {
	for (int i = 0; i < spaceAmt; i++) {
		cout << " ";
	}
}

int prevTime = 0;
void graphicsEngine() { // This double-loop system prints "pixels" on two loops. Outside loop: rows, inside loop: columns.
	int sidebarLines = 24 / 3;
	messageHandler();
	if (totalFrames % 80 == 0) {
		system("cls");
	}
	for (int scany = 0; scany < gameHeight; scany++) {
		gotoxy(0, scany);
		if ((totalFrames - 5 - scany) % 4 > 1) { // Left border
			printf("  . .   ");
		}
		else {
			printf("        ");
		}
		for (int scanx = 0; scanx < gameWidth / 4; scanx++) { // This is the core of the rendering system; the entire play area is rendered in this loop.
			int toRender = scanx + (scany * gameWidth / 4); // Picks the "pixel" to render
			render(allPixels[toRender]); // Prints the rendered "pixel"
			cout << " ";
		}
		if ((totalFrames - 5 - scany) % 4 > 1) { // Right border
			printf("   . .");
		}
		else {
			printf("      ");
		}
		if (health < 3 && (totalFrames + gameHeight - scany) % 4 > 1) { // HUD Scattering at low health
			printf(" ");
		}
		int hudSpacing = 3; // How far HUD elements are from the border
		if (scany == gameHeight - 23) {
			addSpaces(hudSpacing - 1);
			printf("ORBITAL DEFENSE FRIGATE:  ");
		}
		if (scany == gameHeight - 22) {
			addSpaces(hudSpacing - 1);
			printf("EARTH'S LAST LINE OF DEFENSE  ");
		}
		if (scany == gameHeight - 21) {
			addSpaces(hudSpacing - 1);
			printf("Created by KingLuf77  ");
		}
		if (scany == gameHeight - 19) {
			addSpaces(hudSpacing - 1);
			int theTime = clock();
			int newTime = clock() - prevTime;
			int fps = 1000 / newTime;
			if (totalFrames % 4 == 0) {
				printf("FPS: %d    ", fps);
			}
			prevTime = theTime;
		}
		if (scany == gameHeight - 18) { // top sidebar lines
			addSpaces(hudSpacing - 1);
			printf("<<");
			for (int s = 0; s < sidebarLines; s++) {
				printf("-+-");
			}
			printf(">>");
		}
		if (scany == gameHeight - 16) { // railgun status
			addSpaces(hudSpacing);
			if (reload == reloadTime) {
				if (pBullets3[px / 4] != -1 && pBullets2[px / 4] != -1 && pBulletsY[px / 4] != -1) {
					printf("RAILGUN: - - -  ");
				}
				else {
					printf("RAILGUN: READY  ");
				}
			}
			else {
				printf("RAILGUN: - - -  ");
			}
		}
		if (scany == gameHeight - 15) { // health
			addSpaces(hudSpacing);
			printf("HEALTH: [");
			for (int i = 0; i < health; i++) {
				printf("I");
			}
			for (int i = health; i < maxHealth; i++) {
				printf(" ");
			}
			printf("]  ");
		}
		if (scany == gameHeight - 14) { // score
			addSpaces(hudSpacing);
			printf("SCORE:     %d      ", score);
		}
		if (scany == gameHeight - 13) { // new high score!
			addSpaces(hudSpacing);
			if (score > highScore) {
				printf("New High Score!  ");
			}
			else {
				printf("HighScore: %d      ", highScore);
			}
		}
		if (scany == gameHeight - 11) { // bottom sidebar
			addSpaces(hudSpacing - 1);
			printf("<<");
			for (int s = 0; s < sidebarLines; s++) {
				printf("-+-");
			}
			printf(">>  ");
		}
		if (scany == gameHeight - 9) { // message 1 (top)
			addSpaces(hudSpacing - 1 + (sidebarLines * 3) + 5 - 19);
			printMessage(message1);
		}
		if (scany == gameHeight - 8) { // message 2
			addSpaces(hudSpacing - 1 + (sidebarLines * 3) + 5 - 19);
			printMessage(message2);
		}
		if (scany == gameHeight - 7) { // message 3
			addSpaces(hudSpacing - 1 + (sidebarLines * 3) + 5 - 19);
			printMessage(message3);
		}
		if (scany == gameHeight - 6) { // message 4
			addSpaces(hudSpacing - 1 + (sidebarLines * 3) + 5 - 19);
			printMessage(message4);
		}
		if (scany == gameHeight - 5) { // message 5
			addSpaces(hudSpacing - 1 + (sidebarLines * 3) + 5 - 19);
			printMessage(message5);
		}
		for (int b = 0; b < 3; b++) { // warning signs - screen will turn yellow and you will see "warning!" on the bottom right corner as enemies approach Earth.
			if (scany == gameHeight - b - 1) {
				addSpaces(hudSpacing - 1);
				int warning = 0;
				for (int i = 0; i < gameWidth / 4; i++) {
					if (cHeight[i] >= gameHeight - 2 - b) {
						warning = 1;
						break;
					}
				}
				if (warning == 1 && totalFrames % 6 > 2) { // this makes the warning flash
					printf("WARNING!");
					for (int d = 0; d < 2 - b; d++) { // Adds exclamation points based on which warning label it is.
						printf("!");
					}
					printf("    ");
				}
				else {
					printf("            ");
				}
			}
		}
		printf("\n");
	}
}

void runGame() {
	while (gameOver == 0) {
		playerControls(); // keyboard inputs
		physics(); // gameplay - scoring, damage, etc.
		pixelDef(); // defines pixel array values using my 4x1 Rendering System; creates render values for the play area
		graphicsEngine(); // renders play area and HUD elements
		if (health <= 0) {
			gameOver = 1;
			deathType = 2; // Type 2 means that you were killed by the enemy, as opposed to letting them get to Earth
			break;
		}
		delay(frameDelay);
		totalFrames++;
	}
	system("cls"); // clears screen when game is over
}

void gameOverScreen() {
	system("color 0a");
	slowTypeDelay = 15;
	if (deathType == 1) {
		slowType("YOU LET THEM PASS. ", 20);
		delay(200);
	}
	if (deathType == 2) {
		slowType("YOU DIED. ", 11);
		delay(200);
	}
	slowType("GAME OVER.", 11);
	delay(150);
	printf("\n");
	slowTypeDelay = 15;
	if (score > highScore) {
		slowType("SCORE:", 7);
		delay(slowTypeDelay * 2);
		printf(" %d\n", score);
		highScore = score;
		for (int f = 0; f < 10; f++) {
			system("cls");
			if (deathType == 1) {
				printf("YOU LET THEM PASS. ");
			}
			if (deathType == 2) {
				printf("YOU DIED. ");
			}
			printf("GAME OVER.\nSCORE: %d\n", score);
			if (f % 2 == 1) {
				printf("New High Score!");
			}
			else {
				printf("               ");
			}
			delay(60);
		}
	}
	else {
		slowType("SCORE:", 7);
		delay(120);
		printf(" %d\n", score);
		delay(120);
		slowType("HIGH SCORE:", 12);
		delay(120);
		printf(" %d", highScore);
	}
	printf("\n\nPress any key to play again...");
	system("pause >null");
}

int main() {
	system("color 0a");
	if (fastMode != 1 && music == 1) {
		system("start gameTheme.mp3");
	}
	system("dir");
	srand(time(NULL));
	printf("\nGame Ready!\nPress any key to start...");
	system("pause >null");
	system("cls");
	if (fastMode != 1) {
		bigTitle();
	}
	printf("Press any key to begin...");
	system("pause >null");
	if (fastMode != 1) {
		gameIntro();
		gameInstructions();
	}
	while (1) {
		initializeValues();
		runGame();
		gameOverScreen();
	}
	return(0);
}
