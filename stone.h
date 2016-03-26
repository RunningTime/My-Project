#ifndef STONE_H
#define STONE_H

#include <QString>

class Stone
{
public:
	enum TYPE{JIANG, CHE, PAO, MA, XIANG, SHI, BING};
	TYPE _type;
	int _row, _col, _id;
	bool _dead, _red;
	void init(int id);
	void rotate();
	QString name();
	Stone();
	~Stone();
};

#endif // STONE_H
