#include <iostream>
#include <SFML/Graphics.hpp>

#ifndef _function_HPP_INCLUDED__   
#define _function_HPP_INCLUDED__  


#define N 450

int squarConverterX(double xx)
{
	xx = (xx/N)*10;
	int xi = floor(xx);

	return xi;
}


int squarConverterY(double yy)
{
	yy = (yy/N)*10;
	int yi = floor(yy);

	return yi;
}


void winnerComm(std::string s, sf::RenderWindow &D)
{
	//D.clear();
	sf::Font font;
	sf::Text text;
	font.loadFromFile("Kingthings Petrock light.ttf");
	text.setFont(font);
	std::ostringstream ss;
	ss << s;
	text.setString(ss.str());
	text.setCharacterSize(42);
	text.setColor(sf::Color(15, 255, 255,185));
	text.setPosition(100,200);
	D.draw(text);
	ss.str("");
	D.display();

}

#endif
