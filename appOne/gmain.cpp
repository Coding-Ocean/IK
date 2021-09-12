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
    float len = 0;
    void follow(const VECTOR2& pos) {
        ep = pos;
        //�n�_�����߂�
        VECTOR2 vec = pos - sp;
        vec.setMag(len);
        sp = pos + -vec;
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

        //��[���獪���Ɍ������āA�I�_�A�n�_�����߂Ă���
        VECTOR2 fp = mouse;//��[��seg�̓}�E�X��ǂ�
        int i = numSegs - 1;//��[�̂�seg�̃C���f�b�N�X
        while (i >= 0) {
            segs[i].follow(fp);
            fp = segs[i].sp;
            i--;
        }

        //���������[�Ɍ������ĕ`��
        stroke(255);
        for (int i = 0; i < numSegs; i++) {
            segs[i].draw();
        }
    }
}
#endif
#ifdef _IK_BASIC_FIXED_ROOT
//���������Œ肷��o�[�W����
#include"libOne.h"
class SEG {
public:
    VECTOR2 sp, ep;
    float len = 0;
    float angle = 0;//�����ǉ�
    void follow(const VECTOR2& pos) {
        ep = pos;
        //�n�_�����߂�
        VECTOR2 vec = pos - sp;
        angle = atan2(vec.y, vec.x);//�n�_����I�_�ւ�angle�����߂Ă���
        vec.setMag(len);
        sp = pos + -vec;
    }
    void setPos(const VECTOR2 pos) {
        sp = pos;
        //angle����I�_�����߂�
        VECTOR2 vec(cos(angle), sin(angle));
        ep = sp + vec * len;
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
    
    VECTOR2 mouse(center.x, center.y);

    VECTOR2 root = center;
    
    while (notQuit) {
        clear();

        if (isPress(MOUSE_LBUTTON)) {
            mouse = VECTOR2(mouseX, mouseY);
        }

        VECTOR2 fp = mouse;
        for (int i = numSegs - 1; i >= 0; i--) {
            segs[i].follow(fp);
            fp = segs[i].sp;
        }

        VECTOR2 sp = root;
        stroke(255);
        for (int i = 0; i < numSegs; i++) {
            segs[i].setPos(sp);
            sp = segs[i].ep;
            segs[i].draw();
        }
    }
}
#endif
#ifdef _IK_TENTACLE
//�G��Ɍ�����悤�ɂ���
#include"libOne.h"
class SEG {
public:
    VECTOR2 sp, ep;
    float len = 0;
    float angle = 0;
    void follow(const VECTOR2& pos) {
        ep = pos;
        //�n�_�����߂�
        VECTOR2 vec = pos - sp;
        angle = atan2(vec.y, vec.x);//�n�_����I�_�ւ�angle�����߂Ă���
        vec.setMag(len);
        sp = pos + -vec;
    }
    void setPos(const VECTOR2 pos) {
        sp = pos;
        //angle����I�_�����߂�
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

    VECTOR2 root = center;
    
    float thickness = 50;

    while (notQuit) {
        clear();
        //�ǂ�������͈͂�rootoPos�𒆐S�A���a��G��̒����̉~�̓����ɐ���
        mouse = VECTOR2(mouseX, mouseY);
        VECTOR2 vec = mouse - root;
        vec.limmit(len * numSegs);
        VECTOR2 fp = root + vec;//follow pos
        //���[���獪�����Ɍ������ď���
        for (int i = numSegs - 1; i >= 0; i--) {
            segs[i].follow(fp);
            fp = segs[i].sp;
        }
        //���������疖�[�Ɍ������āA�ʒu���Čv�Z���ĕ`��
        stroke(255);
        VECTOR2 sp = root;
        for (int i = 0; i < numSegs; i++) {
            segs[i].setPos(sp);
            sp = segs[i].ep;
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
        //�n�_�����߂�
        VECTOR2 vec = pos - Sp;
        Angle = atan2(vec.y, vec.x);
        vec.setMag(Len);
        Sp = pos + -vec;
    }
    void setPos(const VECTOR2 pos) {
        Sp = pos;
        //Angle�����ɏI�_�����߂�
        VECTOR2 vec(cos(Angle), sin(Angle));
        Ep = Sp + vec * Len;
    }
    void draw() {
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
        //���[���ǂ�������͈͂��urootoPos�𒆐S�A���a��G��Ƃ���~�v�̓����ɐ���
        VECTOR2 vec = pos - RootPos;
        vec.limmit(TentacleLen);
        VECTOR2 fp = RootPos + vec;//follow position
        //���[���獪�����Ɍ������ď���
        for(int i = NumSegs - 1; i>= 0; i--){
            Segs[i].follow(fp);
            fp = Segs[i].Sp;
        }
    }
    void draw() {
        //���������疖�[�Ɍ������āA�ʒu���Čv�Z���ĕ`��
        VECTOR2 sp = RootPos;
        for (int i = 0; i < NumSegs; i++) {
            Segs[i].setPos(sp);
            sp = Segs[i].Ep;
            //�`��
            strokeWeight((NumSegs - i) * Thickness / NumSegs);
            stroke(3.14f/NumSegs*i, 255, 255);
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
