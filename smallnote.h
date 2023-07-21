#ifndef SMALLNOTE_H
#define SMALLNOTE_H

#include <QWidget>
#include <QDateTime>
#include <fstream>

namespace Ui {
class smallnote;
}

class smallnote : public QWidget
{
    Q_OBJECT

public:
    explicit smallnote(QWidget *parent = nullptr);
    ~smallnote();

private slots:
    void on_actionOK_triggered();

    void on_actionNO_triggered();

private:
    Ui::smallnote *ui;
};

#endif // SMALLNOTE_H
