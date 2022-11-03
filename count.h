#ifndef COUNT_H
#define COUNT_H

#include <QWidget>
#include "globaldef.h"

namespace Ui {
class Count;
}

class Count : public QWidget
{
    Q_OBJECT

public:
    explicit Count(QWidget *parent = nullptr);
    ~Count();
    bool isChinese(QChar s);//判断是否是中文字符
    bool isEng(QChar s);   //判断是否是英文字符
    QString content;
    void count();
private:
    Ui::Count *ui;
    int word;
    int Ch;//中文字
    int all;
    int blank;
    int paragraph;
};

#endif // COUNT_H
