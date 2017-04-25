// DataPlace.h
#ifndef _DATAPLACE_H_
#define _DATAPLACE_H_

#include <time.h>
#include <string>
#include <vector>
using namespace std;

struct row;

// delete element.h, row.h, row.site_sp

struct element {
    string name;  //	element name
    size_t i;
    double w, h;                   // element width and height
    vector<size_t> nets_of_nodes;  // element of nets
    double pos_x, pos_y;           // element coordinate
    element() : pos_x(0), pos_y(0), i(0) {}
    virtual bool is_terminal() { return true; }
};

typedef element terminal;

struct node : element {
    row *lRow;  // link to row
    node() : element(), lRow(NULL) {}
    bool is_terminal() { return false; }
};

struct row {
    int coord_y;             // coordinate
    int coord_x;             // subroworigin
    float h;                 // height of row
    unsigned int site_sp;    // sitespacing
    unsigned int num_sites;  // numsites
    unsigned int busySRow;
    vector<node *> ls;
    row() : coord_x(0), coord_y(0), busySRow(0) {}
};

struct net {
    vector<element *> ls;
};

struct Point {
    double x, y;
    row *lRow;
    Point() : x(0), y(0), lRow(NULL) {}
    Point(double a, double b, row *r) {
        x = a;
        y = b;
        lRow = r;
    }
    Point(node &cell) {
        x = cell.pos_x;
        y = cell.pos_y;
        lRow = cell.lRow;
    }
    inline Point &operator+=(const Point &right) {
        this->x += right.x;
        this->y += right.y;
        return *this;
    }
};

class DataPlace {
   public:
    unsigned NumNodes;  // include NumTerminals
    unsigned NumTerminals;
    unsigned NumCells;  // without terminals
    unsigned NumRows;
    unsigned NumNets;
    unsigned NumPins;
    double LengthNodes;  //сумма длин ¤чеек
    unsigned heightSC;   // heightRow = heightNodes
    char *path_nodes, *path_nets, *path_scl, *path_pl;
    vector<node> nodes;  // вектор, содержащий структуры node
    vector<terminal> terminals;  // терминалы
    vector<net> nets;            // вектор св¤зей
    vector<row> rows;  // вектор структур строк с их параметрами

    DataPlace();
    void Start(const char *);

    void parseAuxFile(const char *);  //
    void parseNodes();                //
    void parseNets();                 //
    void parseScl();                  //
    void parsePl();                   //
    void parser();  // считывает данные из файлов .nodes, .nets, .scl, .pl
    void print_nodes();  // выводит на экран все вершины
    void print_nets();   // выводит на экран все св¤зи
    void print_scl();    // вывод строк с их параметрами
    void print_pl();     // вывод размещений нодов

    bool checkPointInRow(const Point &point);
    void findCoreRow(Point &point);
    size_t findCellIdx(Point &point);
    bool findClosestWS(Point &loc, Point &WSLoc, double &width);
    Point calcMeanLoc(size_t &cellId);
    double findLimitRow();

    double evalHPWL();
    double calcOverlap(bool det = false);
    double evalPRow();
    void checkPRow();
    double calcInstHPWL(vector<size_t> &movables);
    double calcInstOverlap(vector<size_t> &movables);
    void updateCells(const vector<size_t> &movables, const vector<Point> &soln,
                     double &prow);
    void setLocation(const size_t id, const Point &pt, double &prow);

    void remOverlaps();  // Legalization
    void savePlacement(const char *plFileName) const;

    double RandomDouble(double min, double max);
    unsigned int RandomUnsigned(unsigned int num1, unsigned int num2);
};

#endif