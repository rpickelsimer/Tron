/*	File:	Tron.cpp
	Corporation: ENCOM
	Author:	Ryan Pickelsimer and Kevin Flynn
	Created on: August 15, 2014
	Class: Programming II, Section 1

	Purpose: This file is part of a program designed to breach high security clearance protocols in the 
	ENCOM mainframe	and gain access to the Tron security protocol. In order to destroy the MCP, you must
	defeat a computer program in the Lightcycle trial. Controls are up(8), down(2), left(4), right(6)
	on the num pad, or the lowercase letters i, j, l, m. With each player win the speed of the game increases
	and with each computer win the game speed will slow down.
*/

// Headers
#include "stdafx.h"
#include <iostream>
#include <conio.h>
#include <string>
#include <windows.h>
#include <cstdlib>
#include <ctime>
using namespace std;

const static enum Direction { N, S, E, W };
class Board;
class Cycle;

////////////////////////////////////////////////////////
/////               Board Class                    /////
////////////////////////////////////////////////////////

class Board {

	const static int COLUMNS = 79, ROWS = 37;
	char board[ROWS][COLUMNS];	
	
public:
	void gotoXY(int, int);
	void drawBoard();
	void welcomeScreen();
	void clear();
	void setBoard(int, int, char);
	char getBoardPos(int row, int column) { return board[row][column]; }
};

///////////////////////////////////////////
/////        Board Functions          /////
///////////////////////////////////////////

// Moves cursor to a location that corresponds to board array
void Board::gotoXY(int row, int column)
{
	COORD coord;
	coord.X = column;
	coord.Y = row;
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE),
		coord
		);
}

// Creates and displays game board.
void Board::drawBoard(){

	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++){
			if ((i == 0) || (j == 0) || (i == ROWS - 1 ) || (j == COLUMNS - 1)){
				board[i][j] = '#';
				gotoXY(i, j);
				cout << board[i][j];
			}
			else { board[i][j] = ' '; }
		}
	}
}

// Displays pre game information. *** add direction keys
void Board::welcomeScreen(){

	gotoXY(5, 37);
	cout << "TRON";
	gotoXY(7, 18);
	cout << "Use directional keys on Numpad or I,J,L,M";
	gotoXY(16, 23);
	cout << "Player";	
	gotoXY(16, 50);
	cout << "Comp";	
	gotoXY(24, 27);
	cout << "Press Any Key When Ready";
}

// Clear console.
void Board::clear() {
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
		);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
		);
	SetConsoleCursorPosition(console, topLeft);
}

// Modify the game board externally.
void Board::setBoard(int row, int column, char c){
	 
	board[row][column] = c;
	gotoXY(row, column);
	cout << board[row][column];
}

////////////////////////////////////////////////////////
/////               Cycle Class                    /////
////////////////////////////////////////////////////////

class Cycle {
	
	const static int DIFFICULTY_1 = 10;		// indicates 100% chance of reacting to situation.
	const static int DIFFICULTY_2 = 8;		// indicates 80% chance of reacting to situation.
	const static int DIFFICULTY_3 = 4;		// indicates 40% chance of reacting to situation.
	const static int DIFFICULTY_4 = 2;		// indicates 20% chance of reacting to situation.
	Direction direction;
	int row;
	int column;
	char bike;
	const char horizTrail = '-';
	const char vertTrail = '|';
	bool automatic;
	bool alive;

public:

	Cycle();
	Cycle(bool);
	char markTrail();
	void moveCycle();
	void changeDirection(char);
	void showCycle(Board);
	bool collisionTest(Board);
	bool testAlive();
	int getRow() { return row; }
	int getColumn() { return column; }
	void testForMove(Board);

};

////////////////////////////////////////
/////       Cycle Functions        /////
////////////////////////////////////////

// Default human player constructor.
Cycle::Cycle(){
	
	direction = E;
	bike = '*';
	row = 18;
	column = 26;
	automatic = false;
	alive = true;
}

// Computer simulated player constructor.
Cycle::Cycle(bool aut){

	direction = W;
	bike = '+';
	row = 18;
	column = 52;
	automatic = aut;
	alive = true;
}

// Takes keyboard key value and returns corresponding Direction.
void Cycle::changeDirection(char keyPressed) {
	
	if (keyPressed == '8' || keyPressed == 'i')
		direction = N;
	if (keyPressed == '4' || keyPressed == 'j')
		direction = W;
	if (keyPressed == '6' || keyPressed == 'l')
		direction = E;
	if (keyPressed == '2' || keyPressed == 'm')
		direction = S;		
}

// Displays Cycle bike skin at current position.
void Cycle::showCycle(Board b){

	b.gotoXY(row, column);
	cout << bike;
}

// Marks Cycle trail.
char Cycle::markTrail(){

	char c;
	if (direction == N)
		c = vertTrail;
	if (direction == S)
		c = vertTrail;
	if (direction == W)
		c = horizTrail;
	if (direction == E)
		c = horizTrail;
	return c;
}

// Updates position.
void Cycle::moveCycle(){
	
	if (direction == N)		
		row = row - 1;	
	if (direction == S)		
		row = row + 1;	
	if (direction == W)		
		column = column - 1;	
	if (direction == E)	
		column = column + 1;	
}

// Tests for vehicle crash. *Returns false to create infinite loop and stop MCP forever.
bool Cycle::collisionTest(Board b) {
	
	if (b.getBoardPos(row, column) != ' '){
		alive = false;
		return true;
	}
	return false;
}

// Tests whether Cycle survived race.
bool Cycle::testAlive(){

	if (alive == false)
		return false;
	else
	return true;
}

void Cycle::testForMove(Board b) {

	srand(static_cast<unsigned int>(time(0)));
	int ran = rand() % 10;
	int r = row;
	int c = column;

	///   Locate cell in front of Cycle.
	if (direction == N)
		r = row - 1;
	if (direction == S)
		r = row + 1;
	if (direction == W)
		c = column - 1;
	if (direction == E)
		c = column + 1;
	
	///   Cycle has chance to turn before crash. May turn any direction but current.
	if (b.getBoardPos(r, c) != ' ') {
		if (ran < DIFFICULTY_1) {
			do {
				ran = rand() % 4;				
			} while (direction == (Direction)ran);
			direction = (Direction)ran;
		}
	}
	///   Cycle may make ninety degree turn. Chance of turn increases in zones near border.
	///   Zone 1
	else if (((row == 2 || row == 3) && direction == N) || ((row == 33 || row == 34) && direction == S)){
		if (ran < DIFFICULTY_2) {
			ran = ran % 2;
			if (ran == 0)
				direction = E;
			else
				direction = W;
		}
	}
	else if (((column == 2 || column == 3) && direction == W) || ((column == 75 || column == 76) && direction == E)){
		if (ran < DIFFICULTY_2) {
			ran = ran % 2;
			if (ran == 0)
				direction = N;
			else
				direction = S;
		}
	}
	///   Zone 2
	else if (((row >= 4 && row <= 6) && direction == N) || ((row >= 30 && row <= 32) && direction == S)){
		if (ran < DIFFICULTY_3) {
			ran = ran % 2;
			if (ran == 0)
				direction = E;
			else
				direction = W;
		}
	}
	else if (((column >= 4 && column <= 6) && direction == W) || ((column >= 72 && column <= 74) && direction == E)){
		if (ran < DIFFICULTY_3) {
			ran = ran % 2;
			if (ran == 0)
				direction = N;
			else
				direction = S;
		}
	}
	///   Zone 3
	else if (ran < DIFFICULTY_4){		
			ran = ran % 2;
			if (direction == N || direction == S) {
				if (ran == 0)
					direction = E;
				else
					direction = W;
			}
			else if (direction == E || direction == W) {
				if (ran == 0)
					direction = N;
				else
					direction = S;
			}			
	}
}
///////////////////////////////////////////
/////          Main Program           /////
///////////////////////////////////////////

int _tmain(int argc, _TCHAR* argv[]){
	
	// resize console window
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r); //stores the console's current dimensions
	MoveWindow(console, r.left, r.top, 800, 500, TRUE); // 800 width, 500 height
	
	//////////////////////
	//  Main Game Loop  //
	//////////////////////

	const int GAME_SPEED = 100;
	const int LEVEL_UP_SCORE = 1000;
	const int SCORE_MULTIPLIER = 10;
	int clockMultiplier = 0, playerPoints = 0, points = 0;
	bool replay = false;
	int keyPressed;
	bool gameOver = false;	
	
	do {
		//setup game board
		Board b;
		Cycle player;
		Cycle comp(true);

		b.clear();
		b.drawBoard();
		player.showCycle(b);
		comp.showCycle(b);
		b.welcomeScreen();

		//wait for player to begin
		bool ready = false;
		while (!ready){
			if (_kbhit()){
				ready = true;
			}
		}
		
		///////////////////////
		//  Inner Game Loop  //
		///////////////////////

		b.clear();
		b.drawBoard();
		player.showCycle(b);
		comp.showCycle(b);	

		while (!gameOver) {
			Sleep(GAME_SPEED - clockMultiplier);

			//test for player input
			if (_kbhit()){

				keyPressed = _getch();

				// key is only assigned a valid value.
				try {
					player.changeDirection(keyPressed);
				}
				catch (const char* msg){
					cerr << msg << endl;
				}
			}

			//player move
			char p = player.markTrail();
			int row = player.getRow();
			int col = player.getColumn();
			b.setBoard(row, col, p);
			player.moveCycle();
			gameOver = player.collisionTest(b);
			player.showCycle(b);

			//computer move
			if (gameOver == false){
				comp.testForMove(b);
				char c = comp.markTrail();
				row = comp.getRow();
				col = comp.getColumn();
				b.setBoard(row, col, c);
				comp.moveCycle();
				gameOver = comp.collisionTest(b);
				comp.showCycle(b);
			}			
		} //end inner game loop

		if (player.testAlive() == false) {
			b.gotoXY(18, 34);
			cout << "Game Over!";
			clockMultiplier -= 10;
		}
		else {
			b.gotoXY(18, 36);
			cout << "You Win!";
			points = LEVEL_UP_SCORE + (clockMultiplier * SCORE_MULTIPLIER);
			playerPoints += points;
			clockMultiplier += 10;
		}

		b.gotoXY(22, 27);
		cout << "Your score this round: " << points;
		b.gotoXY(24, 30);
		cout << "Your total score: " << playerPoints;
		b.gotoXY(28, 20);
		cout << "Would you like to play again? (y,n) ";
		char n;
		cin >> n;
		if (n == 'y' || n == 'Y'){
			replay = true;
			gameOver = false;
		}
	} while (replay == true); 
	//end main game loop
	return 0;
}