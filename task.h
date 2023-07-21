#ifndef TASK_H
#define TASK_H

#include <QWidget>

#include <fstream>

namespace Ui {
class Task;
}

class Task : public QWidget
{
    Q_OBJECT

public:
    explicit Task(QWidget *parent = nullptr);
    ~Task();

private slots:
    void on_actionOK_triggered();

    void on_actionNO_triggered();

    void on_actionSave_triggered();

public:
    Ui::Task *ui;
};

#endif // TASK_H
