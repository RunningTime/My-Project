#include "board.h"
#include <QPainter>
#include <QMouseEvent>

Board::Board(QWidget *parent) : QWidget(parent)
{
	this->_r = 20;
	setMinimumSize (_r*20, _r*22);
	init (true);
}
void Board::init(bool bRedSide) {
	for (int i=0; i<32; ++i) {
		_stone[i].init (i);
	}
	if (bRedSide) {
		for (int i=0; i<32; ++i) {
			_stone[i].rotate ();
		}
	}
	_selectid = -1;
	_bRedTurn = true;
	_bside = bRedSide;
	update ();
}
Board::~Board() {

}
void Board::paintEvent(QPaintEvent *) {
	QPainter painter(this);
	painter.setRenderHints (QPainter::Antialiasing | QPainter::TextAntialiasing);

	_r = height () / 22;

	painter.save ();
	drawPlate (painter);
	painter.restore ();

	painter.save ();
	drawPlace (painter);
	painter.restore ();

	painter.save ();
	drawStone (painter);
	painter.restore ();
}

void Board::drawPlate(QPainter &painter) {
	//画10条横线
	for(int i=0; i<10; ++i) {
		if (i == 0 || i == 9) {
			painter.setPen (QPen (Qt::black, 3, Qt::SolidLine));
		} else {
			painter.setPen (QPen (Qt::black, 1, Qt::SolidLine));
		}
		painter.drawLine (center (i, 0), center (i, 8));
	}
	//画9条竖线
	for (int i=0; i<9; ++i) {
		if (i == 0 || i == 8) {
			painter.setPen (QPen (Qt::black, 3, Qt::SolidLine));
			painter.drawLine (center (0, i), center (9, i));
		} else {
			painter.setPen (QPen (Qt::black, 1, Qt::SolidLine));
			painter.drawLine (center (0, i), center (4, i));
			painter.drawLine (center (5, i), center (9, i));
		}
	}
}

void Board::drawPlace (QPainter &painter) {
	//画九宫格
	painter.setPen (QPen (Qt::black, 1.3, Qt::SolidLine));
	painter.drawLine (center (0, 3), center (2, 5));
	painter.drawLine (center (2, 3), center (0, 5));
	painter.drawLine (center (9, 3), center (7, 5));
	painter.drawLine (center (7, 3), center (9, 5));
}

bool Board::red(int id) {
	return _stone[id]._red;
}
bool Board::isDead(int id) {
	if (id == -1) {
		return true;
	} else {
		return _stone[id]._dead;
	}
}
QString Board::name(int id) {
	return _stone[id].name ();
}
QPoint Board::center(int row, int col) {
	QPoint ret = QPoint (_r * 2 * (col + 1), _r * 2 * (row + 1));
	return ret;
}
QPoint Board::center(int id) {
	return center (_stone[id]._row, _stone[id]._col);
}

QPoint Board::topLeft(int row, int col) {
	return center (row, col) - QPoint (_r, _r);
}
QPoint Board::topLeft(int id) {
	return center (id) - QPoint (_r, _r);
}
QRect Board::cell(int row, int col) {
	return QRect (topLeft (row, col), QSize (_r*2-1, _r*2-1));
}
QRect Board::cell(int id) {
	return QRect (topLeft (id), QSize (_r*2-1, _r*2-1));
}

void Board::drawStone(QPainter &painter) {
	//画32个棋子
	for (int i=0; i<32; ++i) {
		drawStone (painter, i);
	}
}
void Board::drawStone(QPainter &painter, int id) {
	if (isDead (id)) {
		return ;
	}

	QColor color;
	if (red (id)) {
		color = Qt::red;
	} else {
		color = Qt::black;
	}
	painter.setPen (QPen (QBrush (color), 1));

	if (id == _selectid) {
		painter.setBrush (QBrush (Qt::gray));
	} else {
		painter.setBrush (QBrush (Qt::yellow));
	}
	painter.drawEllipse (cell (id));

	painter.setFont (QFont ("System", _r, 666));
	painter.drawText (cell (id), name (id), QTextOption (Qt::AlignCenter));
}


int Board::relation(int row1, int col1, int row2, int col2) {
	return qAbs (row1 - row2) * 10 + qAbs (col1 - col2);
}
bool Board::isButtomSide(int id) {
	return _bside == _stone[id]._red;
}
int Board::getStoneId(int row, int col) {
	for (int i=0; i<32; ++i) {
		if (_stone[i]._row == row && _stone[i]._col == col && !_stone[i]._dead) {
			return i;
		}
	}
	return -1;
}
int Board::getStoneCountAtLine(int row1, int col1, int row2, int col2) {
	if (row1 != row2 && col1 != col2) {
		return -1;
	} else if (row1 == row2 && col1 == col2) {
		return -1;
	}
	int ret = 0;
	if (row1 == row2) {
		int mn = col1 < col2 ? col1 : col2;
		int mx = col1 < col2 ? col2 : col1;
		for (int col=mn+1; col<mx; ++col) {
			if (getStoneId (row1, col) != -1) {
				ret++;
			}
		}
	} else {
		int mn = row1 < row2 ? row1 : row2;
		int mx = row1 < row2 ? row2 : row1;
		for (int row=mn+1; row<mx; ++row) {
			if (getStoneId (row, col1) != -1) {
				ret++;
			}
		}
	}
	return ret;
}

bool Board::canMoveJIANG(int moveid, int row, int col, int killid) {
	int row1, col1;
	getRowCol (row1, col1, moveid);
	int r = relation (row1, col1, row, col);
	if (r != 1 && r != 10) {
		return false;
	}
	if (col < 3 || col > 5) {
		return false;
	}
	if (isButtomSide (killid)) {
		if (row < 7) {
			return false;
		}
	} else {
		if (row > 2) {
			return false;
		}
	}
	return true;
}
bool Board::canMoveCHE(int moveid, int row, int col, int killid) {
	int row1, col1;
	getRowCol (row1, col1, moveid);
	int cnt = getStoneCountAtLine (row1, col1, row, col);
	if (cnt != 0) {
		return false;
	}
	return true;
}
bool Board::canMovePAO(int moveid, int row, int col, int killid) {
	int row1, col1;
	getRowCol (row1, col1, moveid);
	int cnt = getStoneCountAtLine (row1, col1, row, col);
	//there is a bug
	if (killid != -1) {
		if (cnt == 1) {
			return true;
		}
	} else if (cnt == 0) {
		return true;
	}
	return false;
}
bool Board::canMoveMA(int moveid, int row, int col, int killid) {
	int row1, col1;
	getRowCol (row1, col1, moveid);
	int r = relation (row1, col1, row, col);
	if (r != 12 && r != 21) {
		return false;
	}
	if (r == 12) {
		if (getStoneId (row1, (col1+col)/2) != -1) {
			return false;
		}
	} else {
		if (getStoneId ((row1+row)/2, col1) != -1) {
			return false;
		}
	}
	return true;
}
bool Board::canMoveXIANG(int moveid, int row, int col, int killid) {
	int row1, col1;
	getRowCol (row1, col1, moveid);
	int r = relation (row1, col1, row, col);
	if (r != 22) {
		return false;
	}
	int rEye = (row1 + row) / 2;
	int cEye = (col1 + col) / 2;
	if (getStoneId (rEye, cEye) != -1) {
		return false;
	}
	if (isButtomSide (killid)) {
		if (row < 4) {
			return false;
		}
	} else {
		if (row > 5) {
			return false;
		}
	}
	return true;
}
bool Board::canMoveSHI(int moveid, int row, int col, int killid) {
	int row1, col1;
	getRowCol (row1, col1, moveid);
	int r = relation (row1, col1, row, col);
	if (r != 11) {
		return false;
	}
	if (col < 3 || col > 5) {
		return false;
	}
	if (isButtomSide (killid)) {
		if (row < 7) {
			return false;
		}
	} else {
		if (row > 2) {
			return false;
		}
	}
	return true;
}
bool Board::canMoveBING(int moveid, int row, int col, int killid) {
	int row1, col1;
	getRowCol (row1, col1, moveid);
	int r = relation (row1, col1, row, col);
	if (r != 1 && r != 10) {
		return false;
	}
	if (isButtomSide (moveid)) {
		if (row > row1) {
			return false;
		}
		if (row1 >= 5 && row1 == row) {
			return false;
		}
	} else {
		if (row < row1) {
			return false;
		}
		if (row1 <= 4 && row1 == row) {
			return false;
		}
	}
	return true;
}

bool Board::canMove(int moveid, int row, int col, int killid) {
	if (sameColor (moveid, killid)) {
		return false;
	}
	switch (_stone[moveid]._type) {
		case Stone::JIANG:
			return canMoveJIANG (moveid, row, col, killid);
		case Stone::CHE:
			return canMoveCHE (moveid, row, col, killid);
		case Stone::PAO:
			return canMovePAO (moveid, row, col, killid);
		case Stone::MA:
			return canMoveMA (moveid, row, col, killid);
		case Stone::XIANG:
			return canMoveXIANG (moveid, row, col, killid);
		case Stone::SHI:
			return canMoveSHI (moveid, row, col, killid);
		case Stone::BING:
			return canMoveBING (moveid, row, col, killid);
	}
	return true;
}

bool Board::getClickRowCol(QPoint point, int &row, int &col) {
	for(row=0; row<=9; ++row) {
		for(col=0; col<=8; ++col)
		{
			QPoint dist = center (row, col) - point;
			if (dist.x () * dist.x () + dist.y () * dist.y () < _r*_r) {
				return true;
			}
		}
	}
	return false;
	/*
	int trow = _ptRelease.y () / (_r * 2);
	int tcol = _ptRelease.x () / (_r * 2);
	for (row =trow-2; row<=trow+2; ++row) {
		for (col=tcol-2; col<=tcol+2; ++col) {
			if (row < 0 || row > 9 || col < 0 || col > 8) continue;
			QPoint c = center (row, col);
			int dx = _ptRelease.x () - c.x ();
			int dy = _ptRelease.y () - c.y ();
			int dist = dx * dx + dy * dy;
			if (dist < _r * _r) {
				return true;
			}
		}
	}
	return false;
	*/
}
void Board::getRowCol(int &row, int &col, int moveid) {
	row = _stone[moveid]._row;
	col = _stone[moveid]._col;
}

bool Board::sameColor(int id1, int id2) {
	if (id1 == -1 || id2 == -1) {
		return false;
	}
	return red (id1) == red (id2);
}
void Board::killStone(int killid) {
	if (killid == -1) {
		return ;
	}
	_stone[killid]._dead = true;
}
void Board::moveStone(int moveid, int row, int col, int killid) {
	//saveStep (row, col, killid, _steps);
	killStone (killid);
	_stone[moveid]._row = row;
	_stone[moveid]._col = col;
	_bRedTurn = !_bRedTurn;
}
void Board::tryMoveStone(int row, int col, int killid) {
	if (killid != -1 && sameColor (killid, _selectid)) {
		trySelectStone (killid);
		return ;
	}
	if (canMove (_selectid, row, col, killid)) {
		moveStone (_selectid, row, col, killid);
		_selectid = -1;
		update ();
	}
}
bool Board::canSelect(int id) {
	return _bRedTurn == _stone[id]._red;
}
void Board::trySelectStone(int id) {
	if (id == -1) {
		return ;
	}
	if (!canSelect (id)) {
		return ;
	}
	_selectid = id;
	update ();
}
void Board::click(int id, int row, int col) {
	if (this->_selectid == -1) {
		trySelectStone (id);
	} else {
		tryMoveStone (row, col, id);
	}
}
void Board::click(QPoint point) {
	int row, col;
	if (!getClickRowCol (point, row, col)) {
		return ;
	}
	int id = getStoneId (row, col);
	click (id, row, col);
}
void Board::mouseReleaseEvent(QMouseEvent *ev) {
	if (ev->button () != Qt::LeftButton) {
		return ;
	}
	click (ev->pos ());
}
