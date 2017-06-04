#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <unistd.h>
#include "bth.h"



#ifndef __chars_H_INCLUDED__   
#define _chars_H_INCLUDED__  



class Character
{

protected:

	int id;
	int hpUnit;
	int hp;
	int quantity;
	int x;
	int y;
	int **neighborhood;
	bool alive;
public:

	sf::Texture texture;
	sf::Texture deadTexture;
	sf::Sprite tempTexture;
	sf::Font font;
	sf::Text atext;
	sf::Text sigh;
	sf::Text blood;
	std::ostringstream ss;

	Character();
	Character(int _id, int _hpUnit, int _quantity, int _x, int _y, std::string );
	
		
	virtual ~Character(){}

	int getX();
	int getY();
	int getID();
	bool isAlive();
	void dying(Battlefield &BF);
	int* getFreeSpaceNearEnemy(Battlefield &BF);
	void set(int, int, Battlefield &BF);
	void move(int, int, Battlefield &BF);
	void locateInBTF(Battlefield &BF);
	void changeLocateInBTF(Battlefield &BF);
	void checkNeighborhood(Battlefield &BF);
	void showMask();
	void showHP();
	void drawHP(sf::RenderWindow &D);
	void drawSelected(sf::RenderWindow &D);
	void blooding(sf::RenderWindow &D);
	bool checkIfOtherCharIsNext(Battlefield &BF);
	std::string toSave();
	void setFromFile(std::string s);

	Character* getCharByID(int _id);
	void drawOjb(sf::RenderWindow &D);	
	virtual void attack(Character &C, int dx, int dy,sf::RenderWindow &D,Battlefield &BF) = 0;
	virtual	void defend(int dmg,sf::RenderWindow &D,Battlefield &BF) = 0;
	void walking(sf::RenderWindow &D, int,int,Battlefield &BF);
	virtual void animateAttack(int dx,int dy,sf::RenderWindow &D,Battlefield &) = 0;

};


class Warrior
	: public Character
{
	int dmg;
	int def;
	std::string st;


public:

	Warrior(int _id, int _quantity, int _x, int _y, std::string st)
	:	Character(_id, 1200, _quantity, _x, _y, st)
	{dmg = 1000; def = 250;}


	void attack(Character &C, int dx, int dy,sf::RenderWindow &D,Battlefield &BF);
	void defend(int dmg,sf::RenderWindow &D,Battlefield &BF);
	void animateAttack(int dx,int dy,sf::RenderWindow &D,Battlefield &){}
};




class Paladin
	: public Character
{
	int dmg;
	int def;

public:



	Paladin(int _id, int _quantity, int _x, int _y, std::string st)
	:	Character(_id, 1200, _quantity, _x, _y, st)
	{dmg = 1000; def = 150;}


	void attack(Character &C, int dx, int dy,sf::RenderWindow &D,Battlefield &BF);
	void defend(int dmg,sf::RenderWindow &D,Battlefield &BF);
	void animateAttack(int,int,sf::RenderWindow &D,Battlefield &){}

};


class Wizard
	: public Character
{
	int dmg;
	int def;
	sf::Text attackSigh;
	int **buff;
	std::vector<Character *> chars;

public:

	Wizard(int _id, int _quantity, int _x, int _y, std::string st)
	:	Character(_id, 1000, _quantity, _x, _y, st)
	{dmg = 1500; def = 100; attackSigh.setFont(font);
	attackSigh.setColor(sf::Color(0, 255, 255,155)); //rgb(0, 255, 255)
	buff = new int*[3];
	for(int k = 0; k < 3; ++k)
	buff[k] = new int[3];}

	void attack(Character &C, int dx, int dy,sf::RenderWindow &D,Battlefield &BF);
	void defend(int dmg,sf::RenderWindow &D,Battlefield &BF);
	void animateAttack(int,int,sf::RenderWindow &,Battlefield &);
	

};


class Knight
	: public Character
{
	int dmg;
	int def;


public:

	sf::Texture knightTexture;
	sf::Sprite tempTexture;

	Knight(int _id, int _quantity, int _x, int _y, std::string st)
	:	Character(_id, 1200, _quantity, _x, _y,st)
	{dmg = 2000; def = 350;}


	void attack(Character &C, int dx, int dy,sf::RenderWindow &D,Battlefield &BF);
	void defend(int dmg,sf::RenderWindow &D,Battlefield &BF);
	void animateAttack(int,int,sf::RenderWindow &,Battlefield &){}
};




#endif 
