#include <QWidget>
#include <QFrame>
#include "stone.h"

class Board : public QWidget
{
	Q_OBJECT
public:
	explicit Board(QWidget *parent = 0);
	~Board();

	bool _bside;
	QPoint _ptRelease;

	/* game data */
	Stone _stone[32];
	int _r;  //stone's radius

	/* game status */
	int _selectid;
	bool _bRedTurn;
	void init(bool bRedSide);

	/* draw functions */
	virtual void paintEvent(QPaintEvent *);
	void drawPlate(QPainter &painter);
	void drawPlace (QPainter &painter);
	void drawStone(QPainter &painter);
	void drawStone(QPainter &painter, int id);

	/* functions for coordinate */
	QPoint center(int id); //stone's coordinate
	QPoint center(int row, int col);
	QPoint topLeft(int row, int col);
	QPoint topLeft(int id);
	QRect cell(int row, int col);
	QRect cell(int id);

	/* chess rule */
	bool canMove(int moveid, int row, int col, int killid);
	bool canMoveJIANG(int moveid, int row, int col, int killid);
	bool canMoveCHE(int moveid, int row, int col, int killid);
	bool canMovePAO(int moveid, int row, int col, int killid);
	bool canMoveMA(int moveid, int row, int col, int killid);
	bool canMoveXIANG(int moveid, int row, int col, int killid);
	bool canMoveSHI(int moveid, int row, int col, int killid);
	bool canMoveBING(int moveid, int row, int col, int killid);

	virtual void mouseReleaseEvent(QMouseEvent *ev);
	void click(QPoint point);
	virtual void click(int id, int row, int col);
	void tryMoveStone(int row, int col, int killid);
	void moveStone(int moveid, int row, int col, int killid);
	void killStone(int killid);
	bool canSelect(int id);
	bool sameColor(int id1, int id2);
	void trySelectStone(int id);

	bool red(int id);
	bool isDead(int id);
	QString name(int id);

	int relation(int row1, int col1, int row2, int col2);
	int getStoneCountAtLine(int row1, int col1, int row2, int col2);
	bool isButtomSide(int id);
	bool getClickRowCol(QPoint point, int &row, int &col);
	void getRowCol(int &row, int &col, int moveid);
	int getStoneId(int row, int col);

signals:

public slots:
};
