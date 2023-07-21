#include <iostream>
#include <experimental/filesystem>
#include <vector>

#include "mainwindow.h"

#include <QApplication>
#include <QDate>

using namespace std;
namespace fs = experimental::filesystem;

QString timein, contentin, articlein;
QDate datein;
fs::path dir_now = fs::current_path() / "Note";
int num_task, num_old, num_new, num_pice;

// 定义一个函数，接受一个文件夹路径和一个数组作为参数
void check_and_create(const fs::path& dir) {
  // 定义一个字符串数组，存储四个文件夹的名字
  string folders[5] = {"task", "new", "old", "pice", "article"};
  int counter[5] = {0};
  // 遍历四个文件夹的名字
  for (int i = 0; i < 5; i++) {
    // 拼接文件夹路径
    fs::path sub_dir = dir / folders[i];
    // 检查文件夹是否存在，如果不存在就创建
    if (!fs::exists(sub_dir)) {
      fs::create_directory(sub_dir);
    }
    // 计算文件夹中文件的数量，并存入数组
    int count = 0;
    for (auto& p : fs::directory_iterator(sub_dir)) {
      if (fs::is_regular_file(p)) {
        count++;
      }
    }
    counter[i] = count;
  }
  num_task = counter[0];
  num_new = counter[1];
  num_old = counter[2];
  num_pice = counter[3];
}

int main(int argc, char *argv[])
{
    check_and_create(dir_now);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.refresh();
    return a.exec();

    return 0;
}
