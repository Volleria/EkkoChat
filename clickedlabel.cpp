#include "clickedlabel.h"

ClickedLabel::ClickedLabel()
{

}

ClickedLabel::ClickedLabel(QWidget *parent) : QLabel(parent),_curstate(ClickLabelState::Normal)
{

    this->setCursor(Qt::PointingHandCursor);
}

void ClickedLabel::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton)
    {

        if(_curstate == ClickLabelState::Normal)
        {
            _curstate = ClickLabelState::Selected;
            setProperty("state",_select_hover);
            repolish(this);
            update();
        }
        else
        {
            _curstate = ClickLabelState::Normal;
            setProperty("state",_normal_hover);
            repolish(this);
            update();
        }
        emit clicked();
    }
    // 调用基类的mousePressEvent以保证正常的事件处理
    QLabel::mousePressEvent(ev);
}

// 处理鼠标悬停进入事件
void ClickedLabel::enterEvent(QEvent* event)
{
    if(_curstate == ClickLabelState::Normal)
    {
        setProperty("state",_normal_hover);
        repolish(this);
        update();
    }
    else
    {
        setProperty("state",_select_hover);
        repolish(this);
        update();
    }
    QLabel::enterEvent(event);
}

// 处理鼠标悬停离开事件
void ClickedLabel::leaveEvent(QEvent* event)
{
    if(_curstate == ClickLabelState::Normal)
    {
        setProperty("state",_normal);
        repolish(this);
        update();
    }
    else
    {
        setProperty("state",_select);
        repolish(this);
        update();
    }
    QLabel::leaveEvent(event);
}


void ClickedLabel::SetState(QString normal, QString normal_hover, QString normal_press,
                            QString select, QString select_hover, QString select_press)
{
    _normal = normal;
    _normal_hover = normal_hover;
    _normal_press = normal_press;

    _select = select;
    _select_hover = select_hover;
    _select_press = select_press;

    setProperty("state",_normal);
    repolish(this);
}

ClickLabelState ClickedLabel::GetCurState()
{
    return _curstate;
}
