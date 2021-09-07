//#define _IK_BASIC
//#define _IK_BASIC_FIXED_ROOT
#define _IK_TENTACLE

#ifdef _IK_BASIC
#include"libOne.h"
class SEG {
public:
    VECTOR2 Sp, Ep;
    float Len=0;
    void follow(const VECTOR2& pos) {
        VECTOR2 dir = pos - Sp;
        dir.setMag(Len);
        dir *= -1;
        Sp = pos + dir;
        Ep = pos;
    }
    void drawPoint(){
        point(Sp.x, Sp.y);
    }
    void drawLine() {
        line(Sp.x, Sp.y, Ep.x, Ep.y);
    }
};

void gmain() {
    window(1000, 1000);
    const int numSegs = 5;
    SEG segs[numSegs];
    for (int i = 0; i < numSegs; i++) {
        segs[i].Len = 50;
    }
    while (notQuit) {
        clear();
        
        VECTOR2 mouse(mouseX, mouseY);
        
        int i = numSegs - 1;//最後のseg
        segs[i].follow(mouse);//最後のsegはマウスを追う
        while (i > 0) {
            i--;
            segs[i].follow(segs[i + 1].Sp);//次のセグメントの始点を追う
        }

        stroke(255,100,100);
        for (int i = 0; i < numSegs; i++) {
            strokeWeight(4);
            segs[i].drawLine();
            strokeWeight(10);
            segs[i].drawPoint();
        }
    }
}
#endif

#ifdef _IK_BASIC_FIXED_ROOT
#include"libOne.h"
class SEG {
public:
    VECTOR2 Sp, Ep;
    float Len = 0;
    float Angle = 1.57f;
    void follow(const VECTOR2& pos) {
        VECTOR2 dir = pos - Sp;
        Angle = atan2(dir.y, dir.x);
        dir.setMag(Len);
        dir *= -1;
        Sp = pos + dir;
        Ep = pos;
    }
    void setPos(const VECTOR2 pos) {
        Sp = pos;
        //End position
        VECTOR2 dir(cos(Angle), sin(Angle));
        Ep = Sp + dir * Len;
    }
    void drawPoint() {
        point(Sp.x, Sp.y);
    }
    void drawLine() {
        line(Sp.x, Sp.y, Ep.x, Ep.y);
    }
};

void gmain() {
    window(1000, 1000);

    const int numSegs = 5;
    SEG segs[numSegs];
    for (int i = 0; i < numSegs; i++) {
        segs[i].Len = 50;
    }

    VECTOR2 rootPos(width / 2, height / 2);

    while (notQuit) {
        clear();

        VECTOR2 mouse(mouseX, mouseY);

        int i = numSegs - 1;//最後のseg
        segs[i].follow(mouse);//最後のsegはマウスを追う
        while (i > 0) {
            i--;
            segs[i].follow(segs[i + 1].Sp);//次のセグメントの始点を追う
        }

        stroke(255, 100, 100);
        for (int i = 0; i < numSegs; i++) {
            if (i == 0)
                segs[i].setPos(rootPos);
            else
                segs[i].setPos(segs[i - 1].Ep);
            strokeWeight(4);
            segs[i].drawLine();
            strokeWeight(10);
            segs[i].drawPoint();
        }
    }
}
#endif

#ifdef _IK_TENTACLE
#include"libOne.h"
class SEG {
    friend class TENTACLE;
    VECTOR2 Sp, Ep;
    float Len = 0;
    float Angle = 1.57f;
public:
    void follow(const VECTOR2& pos) {
        VECTOR2 dir = pos - Sp;
        Angle = atan2(dir.y, dir.x);
        dir.setMag(Len);
        dir *= -1;
        Sp = pos + dir;
        Ep = pos;
    }
    void setPos(const VECTOR2 pos) {
        Sp = pos;
        //End position
        VECTOR2 dir(cos(Angle), sin(Angle));
        Ep = Sp + dir * Len;
    }
    void drawPoint() {
        point(Sp.x, Sp.y);
    }
    void drawLine() {
        line(Sp.x, Sp.y, Ep.x, Ep.y);
    }
};
class TENTACLE {
    int NumSegs = 0;
    SEG* Segs = 0;
    VECTOR2 RootPos;
    float Len;
public:
    ~TENTACLE() {
        delete[] Segs;
    }
    void create(int numSegs, float len, float rootX, float rootY) {
        NumSegs = numSegs;
        Segs = new SEG[NumSegs];
        for (int i = 0; i < NumSegs; i++) {
            Segs[i].Len = len;
        }
        Len = len;
        RootPos.x = rootX;
        RootPos.y = rootY;
    }
    void follow(const VECTOR2& pos) {
        VECTOR2 vec = pos - RootPos;
        vec.limmit(Len*NumSegs);

        int i = NumSegs - 1;//最後のseg
        Segs[i].follow(RootPos+vec);//最後のsegはマウスを追う
        while (i > 0) {
            i--;
            Segs[i].follow(Segs[i + 1].Sp);//次のセグメントの始点を追う
        }
    }
    void draw() {
        stroke(6.28f/3,255,255);
        for (int i = 0; i < NumSegs; i++) {
            if (i == 0)
                Segs[i].setPos(RootPos);
            else
                Segs[i].setPos(Segs[i - 1].Ep);
            strokeWeight((NumSegs-i+2)*0.8f);
            Segs[i].drawLine();
            //strokeWeight(10);
            //Segs[i].drawPoint();
        }
    }
};
class BALL {
    float Diameter;
    float Hue;
    VECTOR2 Pos, Vel;
    float Gravity;
public:
    BALL() {
        Hue = 0;
        Diameter = 50;
        Pos.x = 200;
        Pos.y = 100;
        Vel.x = 7;
        Vel.y = -2.1f;
        Gravity = 0.08f;
    }
    VECTOR2 pos() {
        return Pos;
    }
    //constructor() {
    //    this.pos = createVector(390, 100);
    //    this.dir = createVector(6, -2.1);
    //    this.initDy = -8.6;
    //    this.gravity = 0.1;
    //}
    void move() {
        Vel.y += Gravity;
        Pos += Vel;
        if (Pos.y > height - Diameter/2) {
            Pos.y = height - Diameter/2;
            Vel.y *= -1;
        }
        if (Pos.x<110 || Pos.x>width - 110) {
            Vel.x *= -1;
        }
    }
    //move() {
    //    this.pos.add(this.dir);
    //    this.dir.y += this.gravity;
    //    if (this.pos.y > height - 15) {
    //        this.dir.y = this.initDy;
    //    }
    //    if (this.pos.x<110 || this.pos.x>width - 110) {
    //        this.dir.x *= -1;
    //    }
    //}
    void draw() {
        strokeWeight(Diameter);
        colorMode(HSV);
        stroke(Hue,64,255,200);
        Hue += 0.2f;
        point(Pos.x, Pos.y);
        //noStroke();
        //fill(220, 100, 100);
        //circle(Pos.x, Pos.y, 30);
    }
    //show() {
    //    noStroke();
    //    fill(220, 100, 100);
    //    circle(this.pos.x, this.pos.y, 30);
    //}
};
void gmain() {
    window(900, 600);
    TENTACLE tentacle[2];
    tentacle[0].create(60, 5, 60, height / 2);
    tentacle[1].create(60, 5, width-60, height / 2);
    BALL ball;
    while (notQuit) {
        clear();
        ball.move();
        for (int i = 0; i < 2; i++) {
            tentacle[i].follow(ball.pos());
            tentacle[i].draw();
        }
        ball.draw();
    }
}
#endif

#ifdef _IK_TENTA
#include"libOne.h"
class SEG {
public:
    VECTOR2 Sp, Ep;
    float Len=0;
    float Angle=1.57f;
    void follow(VECTOR2 pos) {
        VECTOR2 dir = pos - Sp;
        Angle = dir.heading();
        dir.setMag(Len);
        dir *= -1;
        Sp = pos + dir;
    }
    void setPos(VECTOR2 pos) {
        Sp = pos;
        //End position
        VECTOR2 dir(cos(Angle), sin(Angle));
        Ep = Sp + dir * Len;
    }
    void drawPoint() {
        point(Sp.x, Sp.y);
    }
    void drawLine() {
        line(Sp.x, Sp.y, Ep.x, Ep.y);
    }
};

void gmain() {
    window(1000, 1000);
    VECTOR2 rootPos(0, height/2);
    const int numSegs = 100;
    SEG segs[numSegs];
    for (int i = 0; i < numSegs; i++) {
        segs[i].create(4, 0);
    }
    VECTOR2 ball,vec(5,10);
    while (notQuit) {
        clear();
        
        ball += vec;
        strokeWeight(10);
        stroke(255);
        point(ball.x, ball.y);
        if (ball.x < 0 || ball.x>width)vec.x *= -1;
        if (ball.y < 0||ball.y>height)vec.y *= -1;

        VECTOR2 mouse(mouseX, mouseY);
        VECTOR2 range = ball - rootPos;
        range.limmit(400);
        VECTOR2 followPos = rootPos + range;

        int i = numSegs - 1;
        segs[i].follow(followPos);//最後のsegsはマウスを追う
        while (i > 0) {
            i--;
            segs[i].follow(segs[i + 1].Sp);//次のセグメントの始点を追う

            //segs[i].drawPoint();
        }

        stroke(200, 100, 150);
        for (i = 0; i < numSegs; i++) {
            if (i == 0)
                segs[i].setPos(rootPos);
            else
                segs[i].setPos(segs[i - 1].Ep);


            strokeWeight((numSegs - i));
            if (i < numSegs - 20)
                segs[i].drawPoint();
            else
                segs[i].drawLine();
        }
    }
}
#endif
