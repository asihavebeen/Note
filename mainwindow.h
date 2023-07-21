#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "task.h"
#include "article.h"
#include "smallnote.h"

#include <QMainWindow>
#include <QDate>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void find();
    void remove_refresh(int i);

public slots:

    void on_actionAddTask_triggered();

    void on_actionAddArticle_triggered();

    void on_actionDoneTask1_triggered();

    void on_actionReadArticle1_triggered();

    void on_actionDoneTask2_triggered();

    void on_actionDoneTask3_triggered();

    void on_actionDoneTask4_triggered();

    void on_actionDoneTask5_triggered();

    void on_actionDoneTask6_triggered();

    void on_actionOpenArticle1_triggered();

    void on_actionOpenArticle2_triggered();

    void on_actionReadArticle2_triggered();

    void on_actionRecord_triggered();

    void refresh();

private:
    Ui::MainWindow *ui;
    Task *taskwindow;
    Article *articlewindow;
    smallnote *smallwindow;
};
#endif // MAINWINDOW_H
