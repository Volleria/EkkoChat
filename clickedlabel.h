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

    void SetState(QString normal = "", QString normal_hover = "", QString normal_press ="",
                  QString select = "", QString select_hover="", QString select_press="");

    ClickLabelState GetCurState();

private:
    QString _normal;
    QString _normal_hover;
    QString _normal_press;
    QString _select;
    QString _select_hover;
    QString _select_press;

    ClickLabelState _curstate = ClickLabelState::Normal;

signals:
    void clicked(void);
};

#endif // CLICKEDLABEL_H
