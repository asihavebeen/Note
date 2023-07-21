#ifndef ARTICLE_H
#define ARTICLE_H

#include <QWidget>
#include <QDate>
#include <fstream>

namespace Ui {
class Article;
}

class Article : public QWidget
{
    Q_OBJECT

public:
    explicit Article(QWidget *parent = nullptr);
    ~Article();

private slots:
    void on_actionOK_triggered();

    void on_actionNO_triggered();

    void on_actionSave_triggered();

public:
    Ui::Article *ui;
};

#endif // ARTICLE_H
