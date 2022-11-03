#include "finddialog.h"
#include "ui_finddialog.h"

FindDialog::FindDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindDialog)
{
    ui->setupUi(this);
    //因为下标从1开始比较方便后面查找
    //content="!"+content;
    //反人类QString居然不能直接进行相加！！！！
    setWindowTitle("查找/替换");
    ui->info->setVisible(false);
    cnt=0;
}

FindDialog::~FindDialog()
{
    delete ui;
}
bool FindDialog::checkTextchanged()
{
    if(ui->findString->text().prepend(" ")!=cur_toFind){
        pre_toFind=cur_toFind;
        cur_toFind=ui->findString->text().prepend(" ");
        qDebug()<<"checktextchanged  cur_toFind="<<cur_toFind<<endl;
        return true;
    }
    return false;
}
void FindDialog::generateNext(const QString p)
{
    ne.resize(p.length()+1);
    ne.fill(0);
    for (int i = 2, j = 0; i < p.length(); i ++ )
    {
        while (j && p[i] != p[j + 1]) j = ne[j];
        if (p[i] == p[j + 1]) j ++ ;
        ne[i] = j;
    }
}
void FindDialog::FindKMP()
{
    QString &s=content,&p=cur_toFind;
    int n=content.length()-1,m=cur_toFind.length()-1;
    qDebug()<<"content="<<content<<endl;
    qDebug()<<"cur_toFind="<<cur_toFind<<endl;
    qDebug()<<"n="<<n<<" m="<<m<<endl;
    qDebug()<<"s="<<s<<endl;
    qDebug()<<"p="<<p<<endl;
    qDebug()<<currentCursor<<endl;
    int &i = currentCursor;
    for (int j = 0; i <= n; i ++ )
    {
        while (j && s[i] != p[j + 1]) j = ne[j];
        if (s[i] == p[j + 1]) j ++ ;
        if (j == m)
        {
            j = ne[j];
            // 匹配成功后的逻辑
            int res=i-m+1;
            pos.push_back(res);
            cnt++;
            //把找到的下标压入栈
            qDebug()<<"findNext"<<i-m+1<<endl;
        }
    }
}
int FindDialog::findNext()
{
    if((now+1)>=pos.size())return 0;
    else {
        ui->info->setVisible(true);
        ui->info->setText(QString("%1/%2").arg(now+2).arg(cnt+1));
        return pos[++now];
    }
}

QString FindDialog::replaceAll(const QString t)
{
    QString res;
    int pre=1;
    QString &s=content,&p=cur_toFind;
    int n=content.length()-1,m=cur_toFind.length()-1;
    qDebug()<<"content="<<content<<endl;
    qDebug()<<"cur_toFind="<<cur_toFind<<endl;
    qDebug()<<"n="<<n<<" m="<<m<<endl;
    qDebug()<<"s="<<s<<endl;
    qDebug()<<"p="<<p<<endl;
    qDebug()<<currentCursor<<endl;
    int &i = currentCursor;
    for (int j = 0; i <= n; i ++ )
    {
        while (j && s[i] != p[j + 1]) j = ne[j];
        if (s[i] == p[j + 1]) j ++ ;
        if (j == m)
        {
            j = ne[j];
            // 匹配成功后的逻辑
            res+=s.mid(pre,i-m+1-pre);
            res+=t;
            pre=i+1;
            //把找到的下标压入栈
            qDebug()<<"findNext"<<i-m+1<<endl;
        }
    }
    res+=s.mid(pre,i-pre+1);//剩余的字符串处理一下
    content=res;
    content.prepend(" ");
    qDebug()<<content<<endl;
    qDebug()<<res<<endl;
    i=1;
    return res;//因为FindDialog为了计算方便第0位空着，但是要传回要从第0位开始
}
int FindDialog::findPrev()
{
    if((now-1)<0)return 0;
    else
    {
        ui->info->setVisible(true);
        ui->info->setText(QString("%1/%2").arg(now).arg(cnt+1));
        return pos[--now];
    }
}
void FindDialog::on_nextButton_clicked()
{
    //如果要查找的字符串改变了，则重置模式串的next数组
    if(checkTextchanged())
    {
        qDebug()<<"文字改变"<<endl;
        qDebug()<<"cur_toFind.length="<<cur_toFind.length()<<endl;
        generateNext(cur_toFind);
        pos.clear();
        currentCursor=0;
        cnt=-1;
        now=-1;
        FindKMP();
    }
    currentCursor++;
    int ans=findNext();
    if(ans==0)
        QMessageBox::warning(this,"警告","没有找到下一个");
    else
    {
        //TODO
        //高亮查找到的字符串
        qDebug()<<"next"<<ans-1<<endl;
        emit(sendIndex(ans-1,ui->findString->text().length()));
    }
}
void FindDialog::on_prevButton_clicked()
{
    if(checkTextchanged()){
        generateNext(cur_toFind);
        pos.clear();
        currentCursor=1;
        cnt=-1;
        now=-1;
        FindKMP();
    }
    int ans=findPrev();
    if(ans==0)
        QMessageBox::warning(this,"警告","没有找到上一个");
    else
    {
        //TODO
        qDebug()<<"prev"<<endl;
        currentCursor=ans+1;
        emit(sendIndex(ans-1,ui->findString->text().length()));
    }
}
void FindDialog::on_allReplaceButton_clicked()
{
    if(checkTextchanged())generateNext(cur_toFind);
    //直接用KMP把找到的替换成需要替换的然后把content传回给mainwindow的textedit
    emit(sendString(replaceAll(ui->replaceString->text())));
}
void FindDialog::closeEvent(QCloseEvent *)
{
    emit(closed());
}
