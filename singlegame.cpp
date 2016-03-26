#include "singlegame.h"


/*
void SingleGame::click(int id, int row, int col) {
	if (!this->_bRedTurn) {
		return ;
	}
	Board::click (id, row, col);
	if (!this->_bRedTurn) {
		Step *step = getBestMove ();
		moveStone (step->_moveid, step->_killid, step->_rowTo, step->_colTo);
		//computerMove ();
	}
}

Step* SingleGame::getBestMove() {
	//1.哪些步骤可以走
	QVector<Step*> steps;
	Step *ret;
	getAllPossibleMove(steps);

	//2.试着走一下 3.评估走的结果
	int maxScore = -100000;
	for (auto it=steps.begin (); it!=steps.end (); ++it) {
		Step* step = *it;
		fakeMove (step);
		int score = calcScore ();
		unfakeMove (step);
		if (score > maxScore) {
			maxScore = score;
			ret = step;
		}
	}
	//4.取最好的结果
	return ret;
}

void SingleGame::getAllPossibleMove(QVector<Step*> &steps) {
	for (int i=16; i<32; ++i) {
		if (_stone[i]._dead) {
			continue;
		}
		for (int row=0; row<=9; ++row) {
			for (int col=0; col<=8; ++col) {
				int killid = this->getStoneId (row, col);
				if (sameColor (killid, i)) {
					continue;
				}
				if (canMove (i, killid, row, col, steps)) {
					saveStep (i, killid, row, col, steps);
				}
			}
		}
	}
}

void SingleGame::fakeMove(Step *step) {
	killStone (step->_killid);
	moveStone (step->_moveid, step->_rowTo, step->_colTo);
}

void SingleGame::unfakeMove(Step *step) {
	reliveStone (step->_killid);
	moveStone (step->_moveid, step->_rowFrom, step->_colFrom);
}
int SingleGame::calcScore() {
	//enum TYPE{};
	int redTotalScore = 0;
	int blackTotalScore = 0;
	static int chessScore[] = {100,	50, 50, 20, 1500, 10, 10};
	for (int i=0; i<16; ++i) {
		if (_stone[i]._dead) {
			continue;
		}
		redTotalScore += chessScore[_stone[i]._type];
	}
	for (int i=16; i<32; ++i) {
		if (_stone[i]._dead) {
			continue;
		}
		blackTotalScore += chessScore[_stone[i]._type];
	}
	return blackTotalScore - redTotalScore;
}
*/


SingleGame::SingleGame()
{

}

SingleGame::~SingleGame()
{

}
