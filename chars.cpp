#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <unistd.h>
#include "chars.h"
#include "function.hpp"

#define PI 3.14

Character* Character::getCharByID(int _id)
{
	if(_id == id)
	return this;
	else
	return NULL;
}


bool Character::checkIfOtherCharIsNext(Battlefield &BF)
{
	checkNeighborhood(BF);
	for(int k = 0; k < 3; ++k)
	{
		for(int j = 0; j < 3; ++j)
		if(neighborhood[k][j] != 0 && neighborhood[k][j] != -9 && neighborhood[k][j] != id)
		return true;
	}
	return false;
}


bool Character::isAlive()
{
	if(!alive)
	return false;
	else
	return true;

}

void Character::walking(sf::RenderWindow &D, int dx , int dy, Battlefield &BF)
{
	
	for(int i = 0; i <abs(dx); ++i)
	{
		tempTexture.move(abs(dx)/dx,0);
		atext.move(abs(dx)/dx,0);
		move(abs(dx)/dx,0,BF);
		D.draw(tempTexture);
		D.draw(atext);
		D.display();
		usleep(2000);
	}
	for(int j = 0; j < abs(dy); ++j)
	{	
		tempTexture.move(0,abs(dy)/dy);

		move(0,abs(dy)/dy,BF);
		D.draw(tempTexture);
		atext.move(0,abs(dy)/dy);
		D.draw(atext);
		D.display();
		usleep(2000);
	}
	locateInBTF(BF);

}

void Character::drawSelected(sf::RenderWindow &D)
{
	ss << "!";
	sigh.setString(ss.str());
	sigh.setPosition(x,y-2);
	D.draw(sigh);
	ss.str("");
}

void Character::drawHP(sf::RenderWindow &D)
{
	ss << quantity;
	atext.setString(ss.str());
	atext.setPosition(x+25,y+2);
	D.draw(atext);
	ss.str("");

}
void Character::drawOjb(sf::RenderWindow &D)
{
	tempTexture.setPosition(getX(),getY());
	D.draw( tempTexture );			
}


Character::Character(int _id, int _hpUnit, int _quantity, int _x, int _y, std::string s)
:	id(_id), hpUnit(_hpUnit), quantity(_quantity), x(_x), y(_y)
{
	alive = true;
	hp = hpUnit*quantity;
	neighborhood = new int*[3];
	for(int k = 0; k < 3; ++k)
	{
		neighborhood[k] = new int[3];
		for(int j = 0; j < 3; ++j)
		neighborhood[k][j] = 0;
	}
	font.loadFromFile("Kingthings Petrock light.ttf");
	deadTexture.loadFromFile("dead.png");
	texture.loadFromFile(s);
	tempTexture.setTexture(texture);
	tempTexture.setPosition(x,y);
	atext.setFont(font);
	atext.setCharacterSize(16);
	atext.setColor(sf::Color::Green);
	blood.setFont(font);
	blood.setColor(sf::Color::Red);
	sigh.setFont(font);
	sigh.setCharacterSize(16);
	sigh.setColor(sf::Color::Red);
}


void Character::dying(Battlefield &BF)
{
	tempTexture.setTexture(deadTexture);
	id = 0;
	alive = false;
	BF.markDead(squarConverterX(getX()), squarConverterY(getY()));

}

void Character::showHP()
{
	std::cout << hp <<"\t"<< quantity << std::endl;
}


void Character::showMask()
{

	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			std::cout << neighborhood[i][j];
		}
		std::cout <<std::endl;
	}
}


int Character::getID()
{
	return id;
}


void Character::checkNeighborhood(Battlefield &BF)
{
	neighborhood = BF.returnMask(squarConverterX(getX()), squarConverterY(getY()));

	return;

}


void Character::locateInBTF(Battlefield &BF)
{	
	BF.addID(squarConverterX(getX()), squarConverterY(getY()), getID());
	return;
}


void Character::move(int xx, int yy, Battlefield &BF)
{
	BF.moveID(squarConverterX(x),squarConverterY(y),squarConverterX(x+xx),squarConverterY(y+yy),id);
	x = x + xx;
	y = y + yy;
	locateInBTF(BF);
	checkNeighborhood(BF);

	return;
}


void Character::set(int xx, int yy, Battlefield &BF)
{
	BF.clearID(squarConverterX(x),squarConverterY(y),id);
	x =  xx;
	y =  yy;
	locateInBTF(BF);

	return;
}

int Character::getX()
{
	return x;
}


int Character::getY()
{
	return y;
}


int* Character::getFreeSpaceNearEnemy(Battlefield &BF)
{
	int *s = new int[2];
	int i = 0;
	int j = 0;
	checkNeighborhood(BF);
	showMask();

	if(id < 0)
	{

		for( i = 0; i < 3; ++i)
		{	
			for( j = 0; j < 3; ++j)
			{	
				if(neighborhood[j][i] == 0)
				{
					s[0] = N*(i - 1)/10;
					s[1] = N*(j - 1)/10;
					std::cout << i <<"\t"<<j<<std::endl;
					return s;
				}
			}	
		}
	}
	else
	{std::cout <<"Error4" <<std::endl;
		for( i = 2; i >= 0; --i)
		{
			for( j = 0; j < 3; ++j)
			{
				if(neighborhood[j][i] == 0)
				{
					s[0] = N*(i - 1)/10;
					s[1] = N*(j - 1)/10;
					return s;
				}
			}
		}
	}


}


void Character::blooding(sf::RenderWindow &D)
{
	int i = 1;
	int xx = x +22;
	int yy = y +10;
	ss << "X";
	blood.setString(ss.str());
	blood.setPosition(xx,yy);
	

	while(i < 5)
	{
		blood.setCharacterSize(8*i);
		D.draw(blood);
		usleep(25000);
		D.display();
		i++;
	}
	ss.str("");
}







void Warrior::attack(Character &C, int dx, int dy, sf::RenderWindow &D,Battlefield &BF)
{
	int *s = new int[2];
	s = C.getFreeSpaceNearEnemy(BF);
	int dist = abs(squarConverterX(dx))+abs(squarConverterY(dy));

	if(((abs(squarConverterX(dx)) == 1) || abs((squarConverterY(dy)) == 1))&&(abs(squarConverterX(dx))+abs(squarConverterY(dy))) <= 2)
	{
		C.defend(6*dmg,D,BF);	
	}
	else
	{
		walking(D,dx+s[0],dy+s[1],BF);
		C.defend(4*dmg*dist/8,D,BF);
	}
	delete s;


}

void Warrior::defend(int dmg,sf::RenderWindow &D,Battlefield &BF)
{
	hp = hp - (dmg -def);
	quantity = ceil(hp/hpUnit);
	blooding(D);
	if(quantity <= 0)
	dying(BF);
}





void Paladin::attack(Character &C, int dx, int dy,sf::RenderWindow &D,Battlefield &BF)
{		
	
		C.defend(15*dmg/(abs(squarConverterX(dx)) + abs(squarConverterY(dy))),D,BF);	
}


void Paladin::defend(int dmg,sf::RenderWindow &D,Battlefield &BF)
{
	hp = hp - (dmg -def);
	quantity = ceil(hp/hpUnit);
	blooding(D);
	if(quantity <= 0)
	dying(BF);

}



void Wizard::animateAttack(int dx, int dy, sf::RenderWindow &D, Battlefield &)
{
	int interval = 45;
	int xx = x + dx - interval;
	int yy = y + dy - interval;
	ss << "X";
	attackSigh.setString(ss.str());
	attackSigh.setCharacterSize(48);
	
	for(int i = 0; i < 3; ++i)
	{	
		
		for(int j = 0; j < 3; ++j)
		{
			attackSigh.setPosition(xx+5,yy);
			D.draw(attackSigh);
			xx = xx +interval;
		}
		yy = yy + interval;
		xx = x + dx - interval;
	}
	D.display();

	ss.str("");
	
}



void Wizard::attack(Character &C, int dx, int dy,sf::RenderWindow &D,Battlefield &BF)
{	
	if(!checkIfOtherCharIsNext(BF))
	{
		chars.clear();	
		buff = BF.returnMask(squarConverterX(dx+x), squarConverterY(dy+y));
		animateAttack(dx,dy,D,BF);
		for(int k = 0; k < 3; ++k)
		{
			for(int j = 0; j < 3; ++j)
			if(buff[k][j] != 0 && buff[k][j] != -9)
			{
			chars.insert(chars.begin(),BF.getCharByID(buff[k][j]));
			}
		}

		for( std::vector<Character*>::iterator it = chars.begin(); it !=chars.end(); ++it)
		if((*it)->isAlive())
		(*it)->defend(10*dmg/(abs(squarConverterX(dx)) + abs(squarConverterY(dy))),D,BF);
	}
	else
	{
		int *s = new int[2];
		s = C.getFreeSpaceNearEnemy(BF);
		int dist = abs(squarConverterX(dx))+abs(squarConverterY(dy));

		if(((abs(squarConverterX(dx)) == 1) || abs((squarConverterY(dy)) == 1))&&(abs(squarConverterX(dx))+abs(squarConverterY(dy))) <= 2)
		{
			C.defend(dmg,D,BF);	
		}
		else
		{
			walking(D,dx+s[0],dy+s[1],BF);
			C.defend(2*dmg/dist,D,BF);
		}
		delete s;
	}

}


void Wizard::defend(int dmg,sf::RenderWindow &D, Battlefield &BF)
{
	hp = hp - (dmg -def);
	quantity = ceil(hp/hpUnit);
	blooding(D);
	if(quantity <= 0)
	dying(BF);

}




void Knight::attack(Character &C, int dx, int dy,sf::RenderWindow &D,Battlefield &BF)
{
	int *s = new int[2];
	s = C.getFreeSpaceNearEnemy(BF);
	int dist = abs(squarConverterX(dx))+abs(squarConverterY(dy));

	if(((abs(squarConverterX(dx)) == 1) || abs((squarConverterY(dy)) == 1))&&(abs(squarConverterX(dx))+abs(squarConverterY(dy))) <= 2)
	{
		C.defend(8*dmg,D,BF);	
	}
	else
	{
		walking(D,dx+s[0],dy+s[1],BF);
		C.defend(2*dmg/dist,D,BF);
	}
	delete s;

}

void Knight::defend(int dmg,sf::RenderWindow &D,Battlefield &BF)
{
	hp = hp - (dmg -def);
	quantity = ceil(hp/hpUnit);
	blooding(D);
	if(quantity <= 0)
	dying(BF);
}


