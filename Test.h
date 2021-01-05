#ifndef _TEST_H
#define _TEST_H
#include "Game.h"
#include <string>
class CTest : public CGame
{
struct player
{
	std::string _name;
	int _x;
	int _y;
};
private:
	player m_p[2];
public:
	//¹¹Ôìº¯Êý
	CTest(const char * name1, const char * name2);
	void Input(int id, int type, int p1, int p2);
	bool Output(int id, char ** data, int *len);
	void Run();
};
#endif