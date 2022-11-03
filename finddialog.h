#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include "globaldef.h"
namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindDialog(QWidget *parent = nullptr);
    ~FindDialog();

    QString content;    //下标从1开始
    QString cur_toFind; //现在的查找内容框里的字符串
    QString pre_toFind; //之前查找内容框里的字符串（其实没啥用
    //QStack<int>pos; //把每次查找到的结果压入栈，用于“上一个”功能
    QVector<int>pos;    //用动态数组存位置
    void Init();    //初始化
    bool checkTextchanged();    //检测本次查找和上次查找是否是同个模式串
    int findNext(); //找下一个匹配的字符串，返回下标
    int findPrev(); //从栈里取上一个匹配的结果
    void generateNext(const QString p); //生成模式串p的next数组
    QString replaceAll(const QString t);   //把所有cur_toFind替换成需要替换的
    void FindKMP();
private:
    Ui::FindDialog *ui;
    QVector<int>ne; //记录模式串的next数组
    int currentCursor;
    int cnt;
    int now;
signals:
    void sendIndex(int,int);
    void sendString(QString);
    void closed();
public slots:
    void on_nextButton_clicked();
    void on_prevButton_clicked();
    void on_allReplaceButton_clicked();
    void closeEvent(QCloseEvent *);
};

#endif // FINDDIALOG_H
