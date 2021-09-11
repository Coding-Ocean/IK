#define _IK_EXERCISE
//#define _IK_BASIC
//#define _IK_BASIC_FIXED_ROOT
//#define _IK_TENTACLE
//#define _IK_CLASS_TENTACLE

#ifdef _IK_EXERCISE
#include"libOne.h"
class SEG {
public:
    VECTOR2 sp, ep;
    float len = 0;
};
void gmain() {
    window(1000, 1000);
    while (notQuit) {
        clear();
    }
}
#endif
#ifdef _IK_BASIC
#include"libOne.h"
class SEG {
public:
    VECTOR2 sp, ep;
    float len=0;
    void follow(const VECTOR2& pos) {
        ep = pos;
        //始点を求める
        VECTOR2 dir = pos - sp;
        dir.setMag(len);
        dir *= -1;
        sp = pos + dir;
    }
    void draw() {
        strokeWeight(6);
        line(sp.x, sp.y, ep.x, ep.y);
        strokeWeight(18);
        point(sp.x, sp.y);
    }
};

void gmain() {
    window(1000, 1000);

    VECTOR2 center(width / 2, height / 2);
    
    const int numSegs = 6;
    SEG segs[numSegs];
    for (int i = 0; i < numSegs; i++) {
        segs[i].sp = center;
        segs[i].ep = center;
        segs[i].len = 50;
    }
    
    VECTOR2 mouse = center;
    
    while (notQuit) {
        clear();

        if (isPress(MOUSE_LBUTTON)) {
            mouse = VECTOR2(mouseX, mouseY);
        }

        //先端から根元に向かって、終点、始点を決めていく
        int i = numSegs - 1;//先端ののsegのインデックス
        segs[i].follow(mouse);//先端のsegはマウスを追う
        while (i > 0) {
            i--;
            segs[i].follow(segs[i + 1].sp);
        }

        //根元から先端に向かって描画
        stroke(255);
        for (int i = 0; i < numSegs; i++) {
            segs[i].draw();
        }
    }
}
#endif
#ifdef _IK_BASIC_FIXED_ROOT
//根っこを固定するバージョン
#include"libOne.h"
class SEG {
public:
    VECTOR2 sp, ep;
    float len = 0;
    float angle = 0;//これを追加
    void follow(const VECTOR2& pos) {
        ep = pos;
        //始点を求める
        VECTOR2 vec = pos - sp;
        angle = atan2(vec.y, vec.x);//始点から終点へのangleを求めておく
        vec.setMag(len);
        vec *= -1;
        sp = pos + vec;
    }
    void setPos(const VECTOR2 pos) {
        sp = pos;
        //angleから終点を求める
        VECTOR2 dir(cos(angle), sin(angle));
        ep = sp + dir * len;
    }
    void draw() {
        strokeWeight(6);
        line(sp.x, sp.y, ep.x, ep.y);
        strokeWeight(18);
        point(sp.x, sp.y);
    }
};

void gmain() {
    window(1000, 1000);
    VECTOR2 center(width / 2, height / 2);
    
    const int numSegs = 6;
    SEG segs[numSegs];
    for (int i = 0; i < numSegs; i++) {
        segs[i].sp = center;
        segs[i].ep = center;
        segs[i].len = 50;
    }
    
    VECTOR2 mouse(center.x,center.y);

    VECTOR2 rootPos = center;
    
    while (notQuit) {
        clear();

        if (isPress(MOUSE_LBUTTON)) {
            mouse = VECTOR2(mouseX, mouseY);
        }

        int i = numSegs - 1;
        segs[i].follow(mouse);
        while (i > 0) {
            i--;
            segs[i].follow(segs[i + 1].sp);
        }

        stroke(255);
        for (int i = 0; i < numSegs; i++) {
            if (i == 0)
                segs[i].setPos(rootPos);
            else
                segs[i].setPos(segs[i - 1].ep);
            segs[i].draw();
        }
    }
}
#endif
#ifdef _IK_TENTACLE
//触手に見えるようにする
#include"libOne.h"
class SEG {
public:
    VECTOR2 sp, ep;
    float len = 0;
    float angle = 0;
    void follow(const VECTOR2& pos) {
        ep = pos;
        //始点を求める
        VECTOR2 vec = pos - sp;
        angle = atan2(vec.y, vec.x);//始点から終点へのangleを求めておく
        vec.setMag(len);
        vec *= -1;
        sp = pos + vec;
    }
    void setPos(const VECTOR2 pos) {
        sp = pos;
        //angleから終点を求める
        VECTOR2 vec(cos(angle), sin(angle));
        ep = sp + vec * len;
    }
    void draw() {
        point(sp.x, sp.y);
    }
};

void gmain() {
    window(1000, 1000);
    
    VECTOR2 center(width / 2, height / 2);
    float len = 2;
    const int numSegs = 150;
    SEG segs[numSegs];
    for (int i = 0; i < numSegs; i++) {
        segs[i].sp = center;
        segs[i].ep = center;
        segs[i].len = len;
    }

    VECTOR2 mouse(center.x, center.y);

    VECTOR2 rootPos = center;

    while (notQuit) {
        clear();
        //追いかける範囲をrootoPosを中心、半径を触手の長さの円の内側に制限
        mouse = VECTOR2(mouseX, mouseY);
        VECTOR2 v = mouse - rootPos;
        v.limmit(len * numSegs);
        VECTOR2 fp = rootPos + v;//follow pos
        //末端から根っこに向かって処理
        int i = numSegs - 1;
        segs[i].follow(fp);
        while (i > 0) {
            i--;
            segs[i].follow(segs[i + 1].sp);
        }
        //根っこから末端に向かって、位置を再計算して描画
        stroke(255);
        for (int i = 0; i < numSegs; i++) {
            if (i == 0)
                segs[i].setPos(rootPos);
            else
                segs[i].setPos(segs[i - 1].ep);
            float thickness = 50;
            strokeWeight((numSegs - i) * thickness / numSegs);
            segs[i].draw();
        }
    }
}
#endif
#ifdef _IK_CLASS_TENTACLE
#include"libOne.h"
class SEG {
    friend class TENTACLE;
    VECTOR2 Sp, Ep;
    float Len = 0;
    float Angle = 0;
public:
    void follow(const VECTOR2& pos) {
        Ep = pos;
        //始点を決める
        VECTOR2 dir = pos - Sp;
        Angle = atan2(dir.y, dir.x);
        dir.setMag(Len);
        dir *= -1;
        Sp = pos + dir;
    }
    void setPos(const VECTOR2 pos) {
        Sp = pos;
        //Angleを元に終点を求める
        VECTOR2 dir(cos(Angle), sin(Angle));
        Ep = Sp + dir * Len;
    }
    void draw(int lineFlag=false) {
        if(lineFlag)
            line(Sp.x, Sp.y, Ep.x, Ep.y);
        else
            point(Sp.x, Sp.y);
    }
};
class TENTACLE {
    int NumSegs = 0;
    SEG* Segs = 0;
    VECTOR2 RootPos;
    float TentacleLen=0;
    float Thickness = 0;
    COLOR Color;
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
        TentacleLen = len * numSegs;
        RootPos.x = rootX;
        RootPos.y = rootY;
        Thickness = 50;
        Color = COLOR(0, 200, 0);//RGB
    }
    void follow(const VECTOR2& pos) {
        //末端が追いかける範囲を「rootoPosを中心、半径を触手とする円」の内側に制限
        VECTOR2 v = pos - RootPos;
        v.limmit(TentacleLen);
        VECTOR2 fp = RootPos + v;
        //末端から根っこに向かって処理
        int i = NumSegs - 1;//末端のsegのインデックス
        Segs[i].follow(fp);
        while (i > 0) {
            i--;
            Segs[i].follow(Segs[i + 1].Sp);
        }
    }
    void draw() {
        colorMode(RGB);
        stroke(Color);
        //根っこから末端に向かって、位置を再計算して描画
        for (int i = 0; i < NumSegs; i++) {
            if (i == 0)
                Segs[i].setPos(RootPos);
            else
                Segs[i].setPos(Segs[i - 1].Ep);
            //描画
            strokeWeight((NumSegs - i) * Thickness / NumSegs);
            Segs[i].draw();
        }
    }
};
class BALL {
    float Radius;
    float Hue;
    VECTOR2 Pos, Vel;
    float Gravity;
public:
    BALL() {
        Hue = 0;
        Radius = 20;
        Pos.x = 200;
        Pos.y = 50;
        Vel.x = 7;
        Vel.y = -2.1f;
        Gravity = 0.08f;
    }
    VECTOR2 pos() {
        return Pos;
    }
    void move() {
        Vel.y += Gravity;
        Pos += Vel;
        if (Pos.y > height - Radius) {
            Pos.y = height - Radius;
            Vel.y *= -1;
        }
        if (Pos.x<110 || Pos.x>width - 110) {
            Vel.x *= -1;
        }
    }
    void draw() {
        strokeWeight(Radius * 2);
        colorMode(HSV);
        stroke(Hue, 64, 255, 220);
        Hue += 0.2f;
        point(Pos.x, Pos.y);
    }
};
void gmain() {
    window(900, 600);
    TENTACLE tentacles[2];
    tentacles[0].create(300, 1,       60, height / 2);
    tentacles[1].create(300, 1, width-60, height / 2);
    BALL ball;
    while (notQuit) {
        clear();
        ball.move();
        for (int i = 0; i < 2; i++) {
            tentacles[i].follow(ball.pos());
            tentacles[i].draw();
        }
        ball.draw();
    }
}
#endif
