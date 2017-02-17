#include "display.h"
#include "ui_display.h"

#include<QPainter>
#include<vector>
#include<math.h>

#include<QTimer>

using std::vector;

#define max(a,b) ((a>b)?(a):(b))
#define sqr(a) ((a)*(a))

int Size;

Display::Display(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Display)
{
    ui->setupUi(this);
    ui->size->setMinimum(10);
    ui->size->setMaximum(30);
    ui->size->setValue(20);

    ui->speed->setMinimum(1);
    ui->speed->setMaximum(40);
    ui->speed->setValue(10);

    QTimer *timer = NULL;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000/ui->speed->value());

    Size = ui->size->value();
}

Display::~Display()
{
    delete ui;
}

QPoint transform(QPoint a,QPoint b,QPoint c){
return b+c-a;
}
class Triangle{
    QPoint a,b,c;
    QColor color;
public:
    Triangle(QPoint ta,QPoint tb,QPoint tc):a(ta),b(tb),c(tc),color(Qt::red){}
    vector<Triangle> Neighbours(){
        vector<Triangle> r;
        r.push_back(Triangle(a,b,transform(c,a,b)));
        r.push_back(Triangle(a,transform(b,a,c),c));
        r.push_back(Triangle(transform(a,b,c),b,c));
    return r;
    }
    void SetColor(QColor c){color =c;}
    void Draw(QPainter* painter){
    QPainterPath path;
    QPolygon pol;
    pol.append(a);
    pol.append(b);
    pol.append(c);
    path.addPolygon(pol);
    painter->setBrush(QBrush(color));
    painter->setPen(QPen(color));
    painter->drawPath(path);
    }
    friend bool operator==(const Triangle a,const Triangle b){
    return (a.a == b.a || a.a == b.b || a.a == b.c)&&
    (a.b == b.a || a.b == b.b || a.b == b.c) &&
    (a.c == b.a || a.c == b.b || a.c == b.c);
    }
    int dist(int x,int y){
    QPoint med = (a+b+c)/3;
    return sqr(x-med.x())+sqr(y-med.y());
    }
    };
    int Size = 10;
    int xCoord = 700;
    int yCoord = 400;
    static int step = 0;
    static int stepsCount = 1000;

vector<Triangle> Old;
vector<Triangle> New;
vector<Triangle> Available;

bool isFree(Triangle t){
    return std::find (Old.begin(),Old.end(),t)==Old.end() &&
        std::find (New.begin(),New.end(),t)==New.end();
}

bool isUsed(Triangle a){
    vector<Triangle> q = a.Neighbours();
    foreach(Triangle t,q)
        if( std::find (Old.begin(),Old.end(),t)==Old.end() &&
            std::find (New.begin(),New.end(),t)==New.end())
                return false;
    return true;
}

void Display::paintEvent(QPaintEvent *event){
    QPainter *painter = new QPainter(this);

    for(int i= Old.size()-1;i>=0;--i)
    Old[i].Draw(painter);
    for(int i=New.size()-1;i>=0;--i)
    New[i].Draw(painter);
    delete painter;
    if(step>=stepsCount)
        return;
    srand(time(0));
    if(step == 0){
        Triangle x(
        QPoint(xCoord-Size*sqrt(3)/4,yCoord-Size/2),
        QPoint(xCoord-Size*sqrt(3)/4,yCoord+Size/2),
        QPoint(xCoord+Size/2,yCoord));
        x.SetColor(Qt::green);
        Old.push_back(x);
        vector<Triangle> q = x.Neighbours();
        foreach(Triangle a,q)
        Available.push_back(a);
        ++step;
        return;
    }
    int x;
    Old[Old.size()-1].SetColor(Qt::black);
    if(rand()%4)
    x = rand()%Available.size();
    else
    x = max(0,(int)Available.size() - 1-rand()%2);
    Old.push_back(Available[x]);
    vector<Triangle> q = Available[x].Neighbours();
    foreach(Triangle a,q)
        if(isFree(a) &&
        std::find(Available.begin(),Available.end(),a)==Available.end())
            Available.push_back(a);
    Available.erase(Available.begin()+x);
    if(rand()%6==0 && step>50 && Available.size()>1)
        Available.erase(Available.begin()+rand()%Available.size());
    ++step;
}


void Display::on_restart_clicked()
{
    step = 0;
    if(timer!=NULL)
        delete timer;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000/ui->speed->value());

    Old.clear();
    Available.clear();
}
