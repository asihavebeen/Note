#include <experimental/filesystem>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <curl/curl.h>
#include <regex>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_article.h"
#include "ui_task.h"
#include "ui_smallnote.h"

using namespace std;
namespace fs = experimental::filesystem;

extern QString timein, contentin, articlein;
extern QDate datein;
extern fs::path dir_now;
extern int num_task, num_old, num_new, num_pice;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    taskwindow = new Task;
    articlewindow = new Article;
    smallwindow = new smallnote;

    ui->setupUi(this);

    ui->AddArticle->setDefaultAction(ui->actionAddArticle);
    ui->AddArticle->setText("添加");
    ui->AddTask->setDefaultAction(ui->actionAddTask);
    ui->AddTask->setText("添加");
    ui->DoneTask1->setDefaultAction(ui->actionDoneTask1);
    ui->DoneTask1->setText("Done");
    ui->DoneTask2->setDefaultAction(ui->actionDoneTask2);
    ui->DoneTask2->setText("Done");
    ui->DoneTask3->setDefaultAction(ui->actionDoneTask3);
    ui->DoneTask3->setText("Done");
    ui->DoneTask4->setDefaultAction(ui->actionDoneTask4);
    ui->DoneTask4->setText("Done");
    ui->DoneTask5->setDefaultAction(ui->actionDoneTask5);
    ui->DoneTask5->setText("Done");
    ui->DoneTask6->setDefaultAction(ui->actionDoneTask6);
    ui->DoneTask6->setText("Done");
    ui->OpenArticle1->setDefaultAction(ui->actionOpenArticle1);
    ui->OpenArticle1->setText("跳转");
    ui->OpenArticle2->setDefaultAction(ui->actionOpenArticle2);
    ui->OpenArticle2->setText("跳转");
    ui->ReadArticle1->setDefaultAction(ui->actionReadArticle1);
    ui->ReadArticle1->setText("已读");
    ui->ReadArticle2->setDefaultAction(ui->actionReadArticle2);
    ui->ReadArticle2->setText("已读");
    ui->Record->setDefaultAction(ui->actionRecord);
    ui->Record->setText("记笔记");

    connect(taskwindow->ui->actionOK, SIGNAL(triggered()), this, SLOT(refresh()));
    connect(taskwindow->ui->actionNO, SIGNAL(triggered()), this, SLOT(refresh()));
    connect(articlewindow->ui->actionOK, SIGNAL(triggered()), this, SLOT(refresh()));
    connect(articlewindow->ui->actionNO, SIGNAL(triggered()), this, SLOT(refresh()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::find()
{
    // 创建容器，用于存储所有txt文件的内容
    vector<string> lines_task1;
    vector<string> lines_task2;
    vector<string> lines_article1;
    vector<string> lines_article2;
    vector<string> lines_article3;
    vector<string> all_task_place;
    vector<string> all_article_place;
    int count_task = 0;
    int count_article = 0;

    // 遍历给定文件夹中的所有txt文件
    for (const auto& entry : fs::directory_iterator(dir_now / "task")) {
        // 获取文件名
        string fn = entry.path().string();
        all_task_place.push_back(fn);
    }
    sort(all_task_place.begin(), all_task_place.end());
    int ntask = all_task_place.size();
    for (int var = 0; var < ntask; ++var) {
        string filename = all_task_place[var];
        // 判断是否是txt文件
        if (filename.substr(filename.size() - 4) == ".txt") {
            // 打开文件
            ifstream fin(filename);
            if (fin) {
                // 读取两行内容，并存入容器中
                string line1, line2;
                getline(fin, line1);
                getline(fin, line2);
                lines_task1.push_back(line1);
                lines_task2.push_back(line2);
                // 关闭文件
                fin.close();
                // 更新文件个数
                count_task++;
            }
        }
    }
    if (count_task < 6) {
        for (int var = 0; var < 6 - count_task; ++var) {
            lines_task1.push_back("无");
            lines_task2.push_back("无");
        }
    }
    ui->Time1->setText(QString::fromStdString(lines_task1[0]));
    ui->Task1->setText(QString::fromStdString(lines_task2[0]));
    ui->Time2->setText(QString::fromStdString(lines_task1[1]));
    ui->Task2->setText(QString::fromStdString(lines_task2[1]));
    ui->Time3->setText(QString::fromStdString(lines_task1[2]));
    ui->Task3->setText(QString::fromStdString(lines_task2[2]));
    ui->Time4->setText(QString::fromStdString(lines_task1[3]));
    ui->Task4->setText(QString::fromStdString(lines_task2[3]));
    ui->Time5->setText(QString::fromStdString(lines_task1[4]));
    ui->Task5->setText(QString::fromStdString(lines_task2[4]));
    ui->Time6->setText(QString::fromStdString(lines_task1[5]));
    ui->Task6->setText(QString::fromStdString(lines_task2[5]));

    // 遍历给定文件夹中的所有txt文件
    for (const auto& entry : fs::directory_iterator(dir_now / "new")) {
        // 获取文件名
        string fn = entry.path().string();
        all_article_place.push_back(fn);
    }
    sort(all_article_place.begin(), all_article_place.end());
    int narticle = all_article_place.size();
    for (int var = 0; var < narticle; ++var) {
        string filename = all_article_place[var];
        // 判断是否是txt文件
        if (filename.substr(filename.size() - 4) == ".txt") {
            // 打开文件
            ifstream fin(filename);
            if (fin) {
                // 读取三行内容，并存入容器中
                string line1, line2, line3;
                getline(fin, line1);
                getline(fin, line2);
                getline(fin, line3);
                lines_article1.push_back(line1);
                lines_article2.push_back(line2);
                lines_article3.push_back(line3);
                // 关闭文件
                fin.close();
                // 更新文件个数
                count_article++;
            }
        }
    }
    if (count_article < 2) {
        for (int var = 0; var < 2 - count_article; ++var) {
            lines_article1.push_back("无");
            lines_article2.push_back("无");
            lines_article3.push_back("无");
        }
    }

    string artname1 = lines_article2[0];
    string artname2 = lines_article2[1];
    for (int i=0; i<artname1.length(); ++i){
        if (artname1[i] == '+'){
            artname1[i] = ' ';
        }
    }
    for (int i=0; i<artname2.length(); ++i){
        if (artname2[i] == '+'){
            artname2[i] = ' ';
        }
    }

    ui->ArticleName1->setText(QString::fromStdString(artname1));
    ui->ArticleName2->setText(QString::fromStdString(artname2));
    ui->ArticleContent1->setText(QString::fromStdString(lines_article3[0]));
    ui->ArticleContent2->setText(QString::fromStdString(lines_article3[1]));
}


void MainWindow::refresh()
{
    // 创建两个容器，用于存储所有txt文件的内容
    vector<string> lines_task1;
    vector<string> lines_task2;
    vector<string> lines_article1;
    vector<string> lines_article2;
    vector<string> lines_article3;
    vector<string> all_task_place;
    vector<string> all_article_place;
    int count_task = 0;
    int count_article = 0;

    // 遍历给定文件夹中的所有txt文件
    for (const auto& entry : fs::directory_iterator(dir_now / "task")) {
        // 获取文件名
        string fn = entry.path().string();
        all_task_place.push_back(fn);
    }
    sort(all_task_place.begin(), all_task_place.end());
    int ntask = all_task_place.size();
    for (int var = 0; var < ntask; ++var) {
        string filename = all_task_place[var];
        // 判断是否是txt文件
        if (filename.substr(filename.size() - 4) == ".txt") {
            // 打开文件
            ifstream fin(filename);
            if (fin) {
                // 读取两行内容，并存入容器中
                string line1, line2;
                getline(fin, line1);
                getline(fin, line2);
                lines_task1.push_back(line1);
                lines_task2.push_back(line2);
                // 关闭文件
                fin.close();
                // 更新文件个数
                count_task++;
            }
        }
    }
    int place = 0;
    if (count_task!=num_task) {
        for (int var = 0; var < num_task; ++var) {
            if (lines_task1[num_task].compare(lines_task1[var]) > 0) {
                place += 1;
            }
        }
        for (int var = num_task; var > place; --var) {
            rename((dir_now / "task" / (to_string(var) + ".txt")).c_str(),
                   (dir_now / "task" / (to_string(var + 1) + ".txt")).c_str());
        }
        rename((dir_now / "task" / "new.txt").c_str(),
               (dir_now / "task" / (to_string(place + 1) + ".txt")).c_str());
        num_task = count_task;
    }

    // 遍历给定文件夹中的所有txt文件
    for (const auto& entry : fs::directory_iterator(dir_now / "new")) {
        // 获取文件名
        string fn = entry.path().string();
        all_article_place.push_back(fn);
    }
    sort(all_article_place.begin(), all_article_place.end());
    int narticle = all_article_place.size();
    for (int var = 0; var < narticle; ++var) {
        string filename = all_article_place[var];
        // 判断是否是txt文件
        if (filename.substr(filename.size() - 4) == ".txt") {
            // 打开文件
            ifstream fin(filename);
            if (fin) {
                // 读取两行内容，并存入容器中
                string line1, line2, line3;
                getline(fin, line1);
                getline(fin, line2);
                getline(fin, line3);
                lines_article1.push_back(line1);
                lines_article2.push_back(line2);
                lines_article3.push_back(line3);
                // 关闭文件
                fin.close();
                // 更新文件个数
                count_article++;
            }
        }
    }
    place = 0;
    if (count_article!=num_new) {
        for (int var = 0; var < num_new; ++var) {
            if (lines_article1[num_new].compare(lines_article1[var]) > 0) {
                place += 1;
            }
        }
        for (int var = num_new; var > place; --var) {
            rename((dir_now / "new" / (to_string(var) + ".txt")).c_str(),
                   (dir_now / "new" / (to_string(var + 1) + ".txt")).c_str());
        }
        rename((dir_now / "new" / "new.txt").c_str(),
               (dir_now / "new" / (to_string(place + 1) + ".txt")).c_str());
        num_new = count_article;
    }

    this->find();
}

void MainWindow::remove_refresh(int i)
{
    // 创建两个容器，用于存储所有txt文件的内容
    vector<string> lines_task1;
    vector<string> lines_task2;
    vector<string> lines_article1;
    vector<string> lines_article2;
    vector<string> lines_article3;
    vector<string> all_task_place;
    vector<string> all_article_place;
    int count_task = 0;
    int count_article = 0;

    // 遍历给定文件夹中的所有txt文件
    for (const auto& entry : fs::directory_iterator(dir_now / "task")) {
        // 获取文件名
        string fn = entry.path().string();
        all_task_place.push_back(fn);
    }
    sort(all_task_place.begin(), all_task_place.end());
    int ntask = all_task_place.size();
    for (int var = 0; var < ntask; ++var) {
        string filename = all_task_place[var];
        // 判断是否是txt文件
        if (filename.substr(filename.size() - 4) == ".txt") {
            // 打开文件
            ifstream fin(filename);
            if (fin) {
                // 读取两行内容，并存入容器中
                string line1, line2;
                getline(fin, line1);
                getline(fin, line2);
                lines_task1.push_back(line1);
                lines_task2.push_back(line2);
                // 关闭文件
                fin.close();
                // 更新文件个数
                count_task++;
            }
        }
    }
    if (count_task!=num_task) {
        for (int var = i; var < num_task; ++var) {
            rename((dir_now / "task" / (to_string(var + 1) + ".txt")).c_str(),
                   (dir_now / "task" / (to_string(var) + ".txt")).c_str());
        }
        num_task = count_task;
    }

    // 遍历给定文件夹中的所有txt文件
    for (const auto& entry : fs::directory_iterator(dir_now / "new")) {
        // 获取文件名
        string fn = entry.path().string();
        all_article_place.push_back(fn);
    }
    sort(all_article_place.begin(), all_article_place.end());
    int narticle = all_article_place.size();
    for (int var = 0; var < narticle; ++var) {
        string filename = all_article_place[var];
        // 判断是否是txt文件
        if (filename.substr(filename.size() - 4) == ".txt") {
            // 打开文件
            ifstream fin(filename);
            if (fin) {
                // 读取两行内容，并存入容器中
                string line1, line2, line3;
                getline(fin, line1);
                getline(fin, line2);
                getline(fin, line3);
                lines_article1.push_back(line1);
                lines_article2.push_back(line2);
                lines_article3.push_back(line3);
                // 关闭文件
                fin.close();
                // 更新文件个数
                count_article++;
            }
        }
    }
    if (count_article!=num_new) {
        for (int var = i; var < num_new; ++var) {
            rename((dir_now / "new" / (to_string(var + 1) + ".txt")).c_str(),
                   (dir_now / "new" / (to_string(var) + ".txt")).c_str());
        }
        num_new = count_article;
    }

    this->find();
}


void MainWindow::on_actionAddTask_triggered()
{
    taskwindow->show();
}


void MainWindow::on_actionAddArticle_triggered()
{
    articlewindow->show();
}


void MainWindow::on_actionDoneTask1_triggered()
{
    string file = dir_now / "task" / "1.txt";  //文件路径
    remove(file.c_str());

    this->remove_refresh(1);
}


void MainWindow::on_actionDoneTask2_triggered()
{
    string file = dir_now / "task" / "2.txt";  //文件路径
    remove(file.c_str());

    this->remove_refresh(2);
}


void MainWindow::on_actionDoneTask3_triggered()
{
    string file = dir_now / "task" / "3.txt";  //文件路径
    remove(file.c_str());

    this->remove_refresh(3);
}


void MainWindow::on_actionDoneTask4_triggered()
{
    string file = dir_now / "task" / "4.txt";  //文件路径
    remove(file.c_str());

    this->remove_refresh(4);
}


void MainWindow::on_actionDoneTask5_triggered()
{
    string file = dir_now / "task" / "5.txt";  //文件路径
    remove(file.c_str());

    this->remove_refresh(5);
}


void MainWindow::on_actionDoneTask6_triggered()
{
    string file = dir_now / "task" / "6.txt";  //文件路径
    remove(file.c_str());

    this->remove_refresh(6);
}


void MainWindow::on_actionOpenArticle1_triggered()
{
    string filename = dir_now / "new" / "1.txt";
    string line1, line2;

    // 打开文件
    ifstream fin(filename);
    if (fin) {
        // 读取两行内容，并存入容器中
        getline(fin, line1);
        getline(fin, line2);
        // 关闭文件
        fin.close();
    }
    // 定义一个字符串，表示pdf文件的路径
    string path = dir_now / "article" / (line1 + "_" + line2 + ".pdf");
    // 构造一个命令字符串，使用evince作为pdf阅读器
    string command = "FoxitReader " + path;
    // 调用system函数，执行命令
    system(command.c_str());
}


void MainWindow::on_actionOpenArticle2_triggered()
{
    string filename = dir_now / "new" / "2.txt";
    string line1, line2;

    // 打开文件
    ifstream fin(filename);
    if (fin) {
        // 读取两行内容，并存入容器中
        getline(fin, line1);
        getline(fin, line2);
        // 关闭文件
        fin.close();
    }
    // 定义一个字符串，表示pdf文件的路径
    string path = dir_now / "article" / (line1 + "_" + line2 + ".pdf");
    // 构造一个命令字符串，使用evince作为pdf阅读器
    string command = "FoxitReader " + path;
    // 调用system函数，执行命令
    system(command.c_str());
}


void MainWindow::on_actionReadArticle1_triggered()
{
    string oldpath = dir_now / "new" / "1.txt";
    string line1, line2;

    // 打开文件
    ifstream fin(oldpath);
    if (fin) {
        // 读取两行内容，并存入容器中
        getline(fin, line1);
        getline(fin, line2);
        // 关闭文件
        fin.close();
    }
    string newpath = dir_now / "old" / (line1 + " " + line2 + ".txt");
    std::rename(oldpath.c_str(), newpath.c_str());

    this->remove_refresh(1);
}


void MainWindow::on_actionReadArticle2_triggered()
{
    string oldpath = dir_now / "new" / "2.txt";
    string line1, line2;

    // 打开文件
    ifstream fin(oldpath);
    if (fin) {
        // 读取两行内容，并存入容器中
        getline(fin, line1);
        getline(fin, line2);
        // 关闭文件
        fin.close();
    }
    string newpath = dir_now / "old" / (line1 + " " + line2 + ".txt");
    std::rename(oldpath.c_str(), newpath.c_str());

    this->remove_refresh(2);
}


void MainWindow::on_actionRecord_triggered()
{
    smallwindow->show();
}


