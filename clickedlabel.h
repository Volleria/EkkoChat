#ifndef CLICKEDLABEL_H
#define CLICKEDLABEL_H
#include <QLabel>
#include "global.h"
#include <QMouseEvent>

class ClickedLabel : public QLabel
{
    Q_OBJECT
public:
    ClickedLabel();
    ClickedLabel(QWidget *parent=nullptr);

    virtual void mousePressEvent(QMouseEvent *ev) override;
    virtual void enterEvent(QEvent *ev) override;
    virtual void leaveEvent(QEvent *ev) override;

    void SetState(QString normal = "", QString hover = "", QString press ="",
                  QString select = "", QString select_hover="", QString select_press="");

    ClickLabelState GetCurState();

private:
    QString _normal;
    QString _normal_hover;
    QString _normal_press;

    QString _selected;
    QString _selected_hover;
    QString _selected_press;

    ClickLabelState _curstate;

signals:
    void clicked(void);
};

#endif // CLICKEDLABEL_H
