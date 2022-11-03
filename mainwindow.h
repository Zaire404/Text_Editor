#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "globaldef.h"
#include "finddialog.h"
#include "count.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void Init();    //初始化
    void resetStack();  //把Undo栈情况
    bool TextChanged(); //文字是否改变过，是否需要保存
    void NewFile(); //新建文件
    bool OpenFile(); //打开文件
    bool SaveFile();  //保存文件
    bool SaveAsFile();  //另存为文件
    int lastIndex;  //查找到的文本下标
    int lastLength; //查找到的长度
    void mergeformat(const QTextCharFormat &fmt);
    void textColor();   //改变字体颜色
    void textFont();    //改变字体格式
    void set_enabled();//把各个对象enabled

private:
    Ui::MainWindow *ui;
    //bool isOpen;    //是否打开文件
    QString curFile;    //现在的文件名 
    QStack<QString> strUndo;    //每次操作都把原文本压入栈
    FindDialog *f;
    Count *c;
    bool replaceFlag;   //标记是否被替换过
public slots:
    void on_action_New_triggered();
    void on_action_Open_triggered();
    void on_action_Save_triggered();
    void on_action_SaveAs_triggered();
    void on_action_Undo_triggered();
    void on_action_Redo_triggered();
    void on_action_Find_triggered();
    void receiveIndex(int index,int length);    //接收信息
    void receiveString(QString s);
    void on_action_color_triggered();
    void on_action_fonts_triggered();
    void on_action_Count_triggered();
    void closeFind();
};
#endif // MAINWINDOW_H
