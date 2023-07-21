#include <experimental/filesystem>

#include "task.h"
#include "ui_task.h"

using namespace std;
namespace fs = experimental::filesystem;

extern QString timein, contentin, articlein;
extern QDate datein;
extern fs::path dir_now;
extern int num_task, num_old, num_new, num_pice;

Task::Task(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Task)
{
    ui->setupUi(this);
    ui->OK->setDefaultAction(ui->actionOK);
    ui->OK->setText("确定");
    ui->NO->setDefaultAction(ui->actionNO);
    ui->NO->setText("取消");
    ui->Save->setDefaultAction(ui->actionSave);
    ui->Save->setText("保存");
}

Task::~Task()
{
    delete ui;
}

void Task::on_actionSave_triggered()
{
    QDate time_in = ui->TimeIn->date();
    datein = time_in;
    timein = datein.toString("yyyy-MM-dd");
    QString content_in = ui->ContentIn->text();
    contentin = content_in;

    string pathName = dir_now / "task" / "new.txt"; // 你要创建文件的路径
    ofstream fout(pathName);
    if (fout) { // 如果创建成功
    fout << timein.toStdString() << endl; // 使用与cout同样的方式进行写入
    fout << contentin.toStdString() << endl;
    fout.close();  // 执行完操作后关闭文件句柄
    }
}

void Task::on_actionOK_triggered()
{
    ui->TimeIn->clear();
    ui->ContentIn->clear();

    QWidget::close();
}

void Task::on_actionNO_triggered()
{
    ui->TimeIn->clear();
    ui->ContentIn->clear();

    QWidget::close();
}

