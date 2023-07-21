#include <experimental/filesystem>
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <regex>
#include <vector>

#include <QtPdfium/QPdfium>
#include <QFile>
#include <QTextStream>
#include <QString>

#include "article.h"
#include "ui_article.h"

using namespace std;
namespace fs = experimental::filesystem;

extern QString timein, contentin, articlein;
extern QDate datein;
extern fs::path dir_now;
extern int num_task, num_old, num_new, num_pice;

Article::Article(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Article)
{
    ui->setupUi(this);
    ui->OK->setDefaultAction(ui->actionOK);
    ui->OK->setText("确定");
    ui->NO->setDefaultAction(ui->actionNO);
    ui->NO->setText("取消");
    ui->Save->setDefaultAction(ui->actionSave);
    ui->Save->setText("保存");
}


Article::~Article()
{
    delete ui;
}

// 定义一个回调函数，用于接收响应数据
size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
    // 将响应数据转换为字符串，并追加到流中
    string data((const char*) ptr, (size_t) size * nmemb);
    *((stringstream*) stream) << data;
    return size * nmemb;
}

// 定义一个函数，用于发送HTTP请求，并返回响应数据
string send_request(const string& url) {
    // 创建一个curl对象
    CURL *curl = curl_easy_init();
    // 创建一个字符串流，用于存储响应数据
    stringstream response;
    if (curl) {
        // 设置curl选项，如URL，回调函数，用户代理等
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, true);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");
        // 执行curl请求
        CURLcode res = curl_easy_perform(curl);
        // 检查是否有错误发生
        if (res != CURLE_OK) {
            // 输出错误信息
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        }
        // 释放curl对象
        curl_easy_cleanup(curl);
    }
    // 返回响应数据
    return response.str();
}

//定义一个函数，接受一个pdf文件名作为参数，返回一个QString，包含abstract部分的文本
QString extractAbstract(const QString &fileName)
{
    //创建一个QPdfium对象，加载pdf文件
    QPdfium pdf(fileName);
    //检查是否加载成功
    if (!pdf.isValid())
    {
        //如果失败，返回一个空字符串
        return QString();
    }
    //获取pdf文件的页数
    // int pageCount = pdf.pageCount();
    //遍历每一页
    for (int i = 0; i < 1; i++)
    {
        //获取当前页的QPdfiumPage对象
        QPdfiumPage page = pdf.page(i);
        //获取当前页的文本
        QString pageText = page.text();
        cout << pageText.toStdString() << endl;
        //检查是否包含"abstract"或"摘要"这样的关键词，不区分大小写
        if (pageText.contains("abstract", Qt::CaseInsensitive) || pageText.contains("摘要", Qt::CaseInsensitive))
        {
            //如果包含，说明找到了abstract部分，返回当前页的文本
            return pageText;
        }
    }
    //如果遍历完所有页都没有找到abstract部分，返回一个空字符串
    return QString();
}


void Article::on_actionSave_triggered()
{
    QString article_in = ui->NameIn->text();
    articlein = article_in;

    string article_ori = articlein.toStdString();
    string article_name = article_ori;
    for (int i=0; i<article_name.length(); ++i){
        if (article_name[i] == ' '){
            article_name[i] = '+';
        }
    }
    string pathName = dir_now / "new" / "new.txt"; // 你要创建文件的路径

    // 定义一个字符串，表示论文名称
    string paper_name = article_name;
    // 定义一个字符串，表示保存路径
    string save_path = dir_now / "article" / (QDate::currentDate().toString("yyyy.MM.dd").toStdString() + "_" + paper_name + ".pdf");

    // 构造一个搜索URL，使用paper_name作为查询参数
    string search_url = "https://scholar.google.com/scholar?q=" + paper_name;
    // 发送HTTP请求，并获取响应数据
    string search_response = send_request(search_url);
    // 创建一个正则表达式对象，用于匹配第一个搜索结果的链接
    regex link_regex("<a href=\"http(.*?)\" data-clk=");
    // 创建一个匹配结果对象，用于存储匹配结果
    smatch link_match;
    // 在响应数据中查找匹配结果
    if (regex_search(search_response, link_match, link_regex)) {
        // 获取第一个匹配结果的第一个捕获组，即链接地址
        string link_url = link_match[1].str() + "\" data-clk=";
        if (link_url.find("<a href=\"http") == string::npos)
            link_url = "<a href=\"http" + link_url;
        // 输出链接地址
        cout << "Found link: " << link_url << endl;
        // 创建一个匹配结果对象，用于存储匹配结果
        smatch pdf_match;
        // 在响应数据中查找匹配结果
        if (regex_search(link_url, pdf_match, link_regex)) {
            string pdf_url = "http" + pdf_match[1].str();
            if (pdf_url.find("pdf") == string::npos){
                pdf_url = pdf_url + ".pdf";
            }
            // 输出PDF文件地址
            cout << "Found PDF: " << pdf_url << endl;
            // 发送HTTP请求，并获取响应数据
            string pdf_response = send_request(pdf_url);
            // 打开一个文件流，用于写入PDF文件内容
            ofstream fout(save_path);
            if (fout) {
                // 写入PDF文件内容，并关闭文件流
                fout << pdf_response;
                fout.close();
                // 输出保存结果
                cout << "Saved PDF to: " << save_path << endl;
            }
            else {
                // 输出错误信息
                cerr << "Failed to open file: " << save_path << endl;
            }
        }
        else {
            // 输出提示信息
            cout << "No PDF found." << endl;
        }
    }
    else {
        // 输出提示信息
        cout << "No link found." << endl;
    }

    //传入处理的pdf文件名
    QString abstractText = extractAbstract(QString::fromStdString(save_path));
    string input = abstractText.toStdString();
    string output;
    int absplace, introplace;
    absplace = input.find("Abstract");
    introplace = input.find("Introduction");
    if (absplace!=-1 && introplace!=-1){
        output = input.substr(absplace, (introplace - absplace));
        int length = output.length();
        for (int i = 0; i < length; i++){
            if (output[i] == '\n'){
                output[i] = ' ';
            }
        }
        for (int i = 0; i < length; i++){
            if (output[i] == '\n'){
                output[i] = ' ';
            }
        }
    }else{
        output = "无法提取摘要";
    }


    ofstream fout(pathName);
    if (fout) { // 如果创建成功
    fout << QDate::currentDate().toString("yyyy.MM.dd").toStdString() << endl; // 使用与cout同样的方式进行写入
    fout << paper_name << endl;
    fout << output << endl;
    fout.close();  // 执行完操作后关闭文件句柄
    }
}


void Article::on_actionNO_triggered()
{
    ui->NameIn->clear();

    QWidget::close();
}


void Article::on_actionOK_triggered()
{
    ui->NameIn->clear();

    QWidget::close();
}

