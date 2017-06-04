#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <memory>


#ifndef __bth_H_INCLUDED__   
#define _bth_H_INCLUDED__  

class Character;

class Battlefield
{
	int **field;
	int **idBot;
	int **mask;
	int size;
	std::vector<Character*> charsNeg;
	std::vector<Character*> charsPos;

public:
	Battlefield(){}
	Battlefield(int size);
	
	void clearBTF();
	void markDead(int,int);
	void insertPtrPosToChar(Character *C);
	void insertPtrNegToChar(Character *C);
	void showBattleField(int size);
	void addID(int xx, int yy, int id);
	void clearID(int xx, int yy, int id);
	void moveID(int xx, int yy, int xxN, int yyN, int id);
	void checkNeighborhood(int xx, int yy);
	int** returnMask(int xx, int yy);
	void clearMask();
	int returnValue(int xx, int yy);
	//void getid(Character &);
	Character *getCharByID(int id);

friend class Character;
friend class Player;

};


#endif 
