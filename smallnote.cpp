#include <experimental/filesystem>

#include "smallnote.h"
#include "ui_smallnote.h"

using namespace std;
namespace fs = experimental::filesystem;

extern QString timein, contentin, articlein;
extern QDate datein;
extern fs::path dir_now;
extern int num_task, num_old, num_new, num_pice;

smallnote::smallnote(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::smallnote)
{
    ui->setupUi(this);

    ui->OK->setDefaultAction(ui->actionOK);
    ui->OK->setText("确定");
    ui->NO->setDefaultAction(ui->actionNO);
    ui->NO->setText("取消");
}

smallnote::~smallnote()
{
    delete ui;
}

void smallnote::on_actionOK_triggered()
{
    string name = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss").toStdString()+ ".txt";
    string save_path = dir_now / "pice" / name;

    QString content_in = ui->TextIn->toPlainText();
    contentin = content_in;

    ofstream fout(save_path);
    if (fout) { // 如果创建成功
    fout << contentin.toStdString() << endl;
    fout.close();  // 执行完操作后关闭文件句柄
    }

    ui->TextIn->clear();

    QWidget::close();
}


void smallnote::on_actionNO_triggered()
{
    ui->TextIn->clear();

    QWidget::close();
}

