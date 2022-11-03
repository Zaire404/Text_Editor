#include "count.h"
#include "ui_count.h"

Count::Count(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Count)
{
    ui->setupUi(this);
    this->setFixedSize(this->geometry().size());//固定窗口大小不变
    this->setWindowTitle("字数统计");
}

Count::~Count()
{
    delete ui;
}
bool Count::isChinese(QChar s)
{
    ushort uni = s.unicode();
    if(uni >= 0x4E00 && uni <= 0x9FA5)return true;
    else return false;
}
bool Count::isEng(QChar s)
{
    if((s>='A'&&s<='Z')||(s>='a'&&s<='z'))return true;
    else return false;
}
void Count::count()
{
    word=Ch=blank=0;
    all=content.length()-1;
    for(int i=1;i<content.length();++i)
    {
        if(content[i]==' ')
        {
            blank++;
            if(isEng(content[i-1]))word++;
        }
        else if(content[i]=='\n'&&content[i-1]!='\n')paragraph++;
        else if(isChinese(content[i]))Ch++;
    }
    if(isEng(content[content.length()-1]))word++;   //防止最后没有字符少判一个单词
    //遍历一遍就有
    qDebug()<<Ch<<endl;
    ui->Chinese->setNum(Ch);
    ui->word->setNum(word);
    ui->Character->setNum(all);
    ui->CharacterWithNoBlank->setNum(all-blank);
}
