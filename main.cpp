#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <unistd.h>
#include "chars.h"


#define N 450

void winnerComm(std::string s, sf::RenderWindow &D);
int squarConverterX(double xx);
int squarConverterY(double yy);

class Player

{

	int numTurn;
	int numChars;
	Character **playerChars;
	
public:

	Player(int _side, sf::RenderWindow &D, Battlefield &BF);
	void move(sf::RenderWindow &D, Player &P,Battlefield &BF);
	void init();
	void drawAll(sf::RenderWindow &D);
	void animateWalking(sf::RenderWindow &D,int dx, int dy ,int, Player &P,Battlefield &BF);
	void insertPtr(Battlefield &BF);
	bool checkIfThereIsStillAlive();


};


bool Player::checkIfThereIsStillAlive()
{
	for(int i = 0; i < numChars; ++i)
	if(playerChars[i]->isAlive())
	return true;
	
	return false;	

}
void Player::insertPtr(Battlefield &BF)
{	int n = numChars;
	while(n > 0)
	{
		if(playerChars[n - 1]->getID() > 0)
		BF.insertPtrPosToChar(playerChars[n - 1]);
		else
		BF.insertPtrNegToChar(playerChars[n - 1]);

		n--;
	}

}

void Player::drawAll(sf::RenderWindow &D)
{
	
	for(int i = 0; i <numChars; ++i)
	{
		playerChars[i]->drawOjb(D);
		if(playerChars[i]->isAlive())
		playerChars[i]->drawHP(D);
	}
	return;
	
}


void Player::move(sf::RenderWindow &D, Player &P,Battlefield &BF)
{

	bool run = true;
	int val;
	sf::Event action;
	int xx;
	int yy;
	int dx,dy;
	double buffX;
	double buffY;
	if(!(playerChars[numTurn]->isAlive()))
	{
		if(numTurn < numChars - 1)
		numTurn++;
		else
		numTurn = 0;
		return;
	}
	else
	while(run)
	{	
			playerChars[numTurn]->drawSelected(D);
			D.display();

			while(D.pollEvent(action))
			if(action.type == sf::Event::MouseButtonPressed &&
	 			action.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2i positionMouse = sf::Mouse::getPosition(D);

				buffX = ((double)positionMouse.x/N)*10;
				buffY = ((double)positionMouse.y/N)*10;
				xx = floor(buffX)*N/10;
				yy = floor(buffY)*N/10;
				val = BF.returnValue(squarConverterX(xx), squarConverterY(yy));
				

				if(val == 0)
				{
					dx = xx - playerChars[numTurn]->getX();
					dy = yy - playerChars[numTurn]->getY();
					playerChars[numTurn]->walking(D,dx,dy,BF);
					drawAll(D);
					P.drawAll(D);

				}
				else if((val > 0 && playerChars[numTurn]->getID() > 0) || (val < 0 && playerChars[numTurn]->getID() < 0) || val == -9)
				continue;
				else if(((val > 0 && playerChars[numTurn]->getID() < 0) || (val < 0 && playerChars[numTurn]->getID() > 0))&& val != -9)
				{
					dx = xx - playerChars[numTurn]->getX();
					dy = yy - playerChars[numTurn]->getY();
					playerChars[numTurn]->attack(*P.playerChars[abs(val)-1],dx,dy,D,BF);	
					D.clear();
					drawAll(D);
					P.drawAll(D);

				}	
			
				run = false;
				if(numTurn < numChars - 1)
				numTurn++;
				else
				numTurn = 0;
				break;
			}
			else if(action.type == sf::Event::Closed)
			{
				D.close();
				break;
			}
	
		
		
	}

	
}


Player::Player(int _side, sf::RenderWindow &D, Battlefield &BF)
{
	numTurn = 0;
	numChars = 4;
	playerChars = new Character *[numChars];
	if(_side == 0)
	{
		playerChars[0] = new Warrior(1, 10, _side, 45,"Warrior.png");
		playerChars[1] = new Wizard(2, 10, _side, 135,"wizard.png");
		playerChars[2] = new Paladin(3, 10, _side, 225,"Paladin.png");
		playerChars[3] = new Knight(4, 10, _side, 315,"knight.png");
	}
	else
	{
		playerChars[0] = new Warrior(-1, 10, _side, 45, "Warrior_right.png");
		playerChars[1] = new Wizard(-2, 10, _side, 135,"wizard_right.png");
		playerChars[2] = new Paladin(-3, 10, _side, 225,"Paladin_right.png");
		playerChars[3] = new Knight(-4, 10, _side, 315,"knight_right.png");
	}
	for(int i = 0; i <numChars; ++i)
	{
		playerChars[i]->drawOjb(D);
		playerChars[i]->locateInBTF(BF);
		playerChars[i]->drawHP(D);
	}

};


int main() {
	
	
	std::string c;
	bool gameOver = false;
	bool closedWindow = false;	
	sf::RenderWindow RW(sf::VideoMode(N, N, 32), "GamePlay");
	Battlefield bField(N/45);

	Player p1(0,RW,bField);
	Player p2(405,RW,bField);
	p1.insertPtr(bField);
	p2.insertPtr(bField);
	bField.showBattleField(N/45);

	
	RW.display();

	while(!gameOver)
	{	
		
		p1.move(RW, p2,bField);
		RW.display();
	bField.showBattleField(N/45);
		if(!p1.checkIfThereIsStillAlive()||!p2.checkIfThereIsStillAlive())
		{
			if(!p1.checkIfThereIsStillAlive())
			winnerComm("Wygral gracz prawy",RW);
			else
			winnerComm("Wygral gracz lewy",RW);
		gameOver = true;
		}

		p2.move(RW, p1,bField);
		RW.display();
	bField.showBattleField(N/45);
		if(!p1.checkIfThereIsStillAlive()||!p2.checkIfThereIsStillAlive())
		{
			if(!p1.checkIfThereIsStillAlive())
			winnerComm("Wygral gracz prawy",RW);
			else
			winnerComm("Wygral gracz lewy",RW);
		gameOver = true;
		}
		

	}

	while(!closedWindow)
	{
		sf::Event action;
		while(RW.pollEvent(action))
		{
			if(action.type == sf::Event::Closed)
			{
				RW.close();
				closedWindow = true;
				break;
			}
		}
	}

	return 0;
}
