#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <unistd.h>
#include "chars.h"


#define N 450

int menu();
int confLoader(std::fstream &f);
void winnerComm(std::string s, sf::RenderWindow &D);
int squarConverterX(double xx);
int squarConverterY(double yy);

class Player

{
	bool side;
	int numTurn;
	int numChars;
	Character **playerChars;
	
public:

	Player(int _side, sf::RenderWindow &D, Battlefield &BF);
	void move(sf::RenderWindow &D, Player &P,Battlefield &BF,bool &CW);
	void init();
	void drawAll(sf::RenderWindow &D);
	void animateWalking(sf::RenderWindow &D,int dx, int dy ,int, Player &P,Battlefield &BF);
	void insertPtr(Battlefield &BF);
	bool checkIfThereIsStillAlive();
	std::string toSave();
	void save();
	void clearFile();
	int loadFromFile(std::fstream &f,Battlefield &BF);

};

int Player::loadFromFile(std::fstream &f,Battlefield &BF)
{
	char Cbuff;
	std::stringstream ss;
	std::string data;
	getline( f, data );
	ss.str(data);
	ss >> side;
	ss >> Cbuff;
	ss >> numTurn;
	for(int i = 0; i <numChars; ++i)
	{
		getline(f,data);
		playerChars[i]->setFromFile(data);
	}
	
	for(int i = 0; i <numChars; ++i)
	{
		if(playerChars[i]->isAlive())
		playerChars[i]->locateInBTF(BF);
		else
		BF.markDead(squarConverterX(playerChars[i]->getX()), squarConverterY(playerChars[i]->getY()));
	}

	return side;

}
void Player::clearFile()
{
	std::fstream file("save.txt",std::ios::out|std::ios::trunc);
	if(file.good())
	{
		file.clear();
		file.close();	
	}
	return;

}

void Player::save()
{
	std::string s;
	std::fstream file("save.txt", std::ios::out | std::ios::in);
	file.seekp( 0, std::ios_base::end);
	if(file.good() == true)
	{
		file << toSave();
		for(int i = 0; i < numChars; ++i)
		file << playerChars[i]->toSave();

		file.close();
	}
	return;

}
std::string Player::toSave()
{
	std::ostringstream ss;

	ss << side << "," << numTurn<< std::endl;

	std::string s;
	s = ss.str();

	return s;

}

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


void Player::move(sf::RenderWindow &D, Player &P,Battlefield &BF,bool &CW)
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
				run = false;
				CW = true;
				break;
			}
			else if(action.type == sf::Event::KeyPressed && action.key.code == sf::Keyboard::S)
			{
			std::cout << "saving"<<std::endl;
				clearFile();
				save();
				P.save();
			}
	
			usleep(1000);
		
	}

	
}


Player::Player(int _side, sf::RenderWindow &D, Battlefield &BF)
{
	numTurn = 0;
	numChars = 4;
	side = _side;
	playerChars = new Character *[numChars];
	if(_side == 0)
	{
		playerChars[0] = new Warrior(1, 10, 0, 45,"Warrior.png");
		playerChars[1] = new Wizard(2, 10, 0, 135,"wizard.png");
		playerChars[2] = new Paladin(3, 10,0, 225,"Paladin.png");
		playerChars[3] = new Knight(4, 10, 0, 315,"knight.png");
	}
	else
	{
		playerChars[0] = new Warrior(-1, 10, 405, 45, "Warrior_right.png");
		playerChars[1] = new Wizard(-2, 10, 405, 135,"wizard_right.png");
		playerChars[2] = new Paladin(-3, 10, 405, 225,"Paladin_right.png");
		playerChars[3] = new Knight(-4, 10, 405, 315,"knight_right.png");
	}
	for(int i = 0; i <numChars; ++i)
	{
		playerChars[i]->locateInBTF(BF);
	}

};


int main() {
	
	
	std::string c;
	bool gameOver = false;
	bool side = 0;
	bool closedWindow = false;
	int typeGame = menu();
	sf::RenderWindow RW(sf::VideoMode(N, N, 32), "GamePlay");
	Battlefield bField(N/45);

	Player p1(0,RW,bField);
	Player p2(1,RW,bField);
	p1.insertPtr(bField);
	p2.insertPtr(bField);


	if(typeGame)
	{	std::fstream f;
		f.exceptions(std::fstream::failbit | std::fstream::badbit);
		try
		{
			f.open( "save.txt", std::ios::in );
			side = confLoader(f);
			bField.clearBTF();
			if(side == 0)
			{
				p1.loadFromFile(f,bField);
				p2.loadFromFile(f,bField);
			}
			else
			{
				p2.loadFromFile(f,bField);
				p1.loadFromFile(f,bField);
			}
		}
		catch(std::fstream::failure w)
		{
			std::cout << "Exception opening/reading file" <<std::endl;
		}
		f.close();
	}
	p1.drawAll(RW);
	p2.drawAll(RW);
	RW.display();
	bField.showBattleField(N/45);
	while(!gameOver)
	{	
		if(side == 0)
			{
			p1.move(RW, p2,bField,closedWindow);
			if(closedWindow)
			break;
			RW.display();

			if(!p1.checkIfThereIsStillAlive()||!p2.checkIfThereIsStillAlive())
			{
				if(!p1.checkIfThereIsStillAlive())
				winnerComm("Wygral gracz prawy",RW);
				else
				winnerComm("Wygral gracz lewy",RW);
			gameOver = true;
			}
		side = 1;
		}
		if(side == 1)
		{
			p2.move(RW, p1,bField,closedWindow);
			if(closedWindow)
			break;
			RW.display();

			if(!p1.checkIfThereIsStillAlive()||!p2.checkIfThereIsStillAlive())
			{
				if(!p1.checkIfThereIsStillAlive())
				winnerComm("Wygral gracz prawy",RW);
				else
				winnerComm("Wygral gracz lewy",RW);
			gameOver = true;
			}
		side = 0;
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
