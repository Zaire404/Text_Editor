#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Init();
    ui->textEdit->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Init()
{
    //saveflag=false;
    this->setWindowTitle(curFile);
    this->ui->textEdit->setVisible(false);
    setWindowIcon(QIcon("://TEXT.png"));
    replaceFlag=false;
    //取消边框
    //this->setWindowFlags(Qt::FramelessWindowHint);
}

void MainWindow::resetStack()
{
    strUndo.clear();
}

bool MainWindow::TextChanged()
{
    if(ui->textEdit->document()->isModified()||replaceFlag)//文本是否被更改
    {
        QMessageBox box(this);
        box.setIcon(QMessageBox::Warning);
        box.setWindowTitle("警告");
        box.setText("是否保存当前文件？");
        //QPushButton *addButton(const QString &text, ButtonRole role);
        QPushButton *yesbtn = box.addButton("是",QMessageBox::YesRole);
        QPushButton *nobtn=box.addButton("否",QMessageBox::NoRole);
        QPushButton *cancelbtn = box.addButton("取消",QMessageBox::RejectRole);
        box.exec();
        //QAbstractButton *QMessageBox::clickedButton() const
        replaceFlag=false;
        if(box.clickedButton() == (QAbstractButton*)yesbtn) return SaveFile();
        else if(box.clickedButton() == (QAbstractButton*)cancelbtn)return false;
    }
    return true;
}

void MainWindow::set_enabled()
{
    ui->textEdit->setVisible(true);
    ui->action_Save->setEnabled(true);
    ui->action_SaveAs->setEnabled(true);
    ui->action_Undo->setEnabled(true);
    ui->action_Redo->setEnabled(true);
    ui->action_color->setEnabled(true);
    ui->action_fonts->setEnabled(true);
    ui->action_Find->setEnabled(true);
    ui->action_Count->setEnabled(true);
}
void MainWindow::NewFile()
{
    set_enabled();
    if(TextChanged())//需要保存
    {
        curFile="Untitled.txt";
        setWindowTitle(curFile);
        ui->textEdit->clear();
    }
}

bool MainWindow::OpenFile()
{

    if(TextChanged())
    {
        QString filename = QFileDialog::getOpenFileName(nullptr,("打开文件"),QDir::homePath(),
                                                        ("所有文件(*.*);;代码文件(*.cpp *.c *.html);;文本文件(*.txt)"));
        if(!filename.isEmpty())
        {
            QFile file(filename);
            //bool QFile::open(FILE *fh, QIODevice::OpenMode mode, QFileDevice::FileHandleFlags handleFlags = DontCloseHandle)
            if(!file.open(QFile::ReadOnly|QFile::Text))
            {
                QMessageBox::warning(this,"警告","无法读取文件");
                return false;//防止只读打开。
            }
            curFile=filename;
            setWindowTitle(curFile);
            //QTextStream::QTextStream(FILE *fileHandle, QIODevice::OpenMode openMode = QIODevice::ReadWrite)
            QTextStream in(&file);
            in.setCodec( "UTF-8");//防止中文寄了
            ui->textEdit->setPlainText((in.readAll()));//放编辑器里
            resetStack();
            strUndo.push(in.readAll());//初始化Undo栈
            ui->textEdit->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
            set_enabled();
            return true;
        }
        return false;
    }
    else return false;
}

bool MainWindow::SaveFile()
{
    //判断是否是新建的文档需要另存为
    if(curFile=="Untitled.txt")return SaveAsFile();
    else{
        QString filename=curFile;
        QFile file(filename);
        if(!file.open(QFile::WriteOnly | QFile::Text))
        //如果不能写入
        {
            QMessageBox::warning(this,"警告","无法写入文件!");
            return false;
        }
        QTextStream out(&file);
        out.setCodec("UTF-8");
        out << ui->textEdit->toPlainText();
        // 获得文件的标准路径
        curFile = QFileInfo(filename).canonicalFilePath();
        setWindowTitle(curFile);
        return true;
    }
}

bool MainWindow::SaveAsFile()
{
    QString filename = QFileDialog::getSaveFileName(this,"另存为",curFile);
    if (filename.isEmpty()) return false;
    QFile file(filename);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    //如果不能写入
    {
        QMessageBox::warning(this,"警告","无法写入文件!");
        return false;
    }
    QTextStream out(&file);
    out << ui->textEdit->toPlainText();
    // 获得文件的标准路径
    curFile = QFileInfo(filename).canonicalFilePath();
    setWindowTitle(curFile);
    return true;
}

void MainWindow::receiveIndex(int index, int length)
{
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.setPosition(lastIndex);
    cursor.setPosition(lastIndex+lastLength,QTextCursor::KeepAnchor);//选择[lastIndex,lastIndex+lastLength]
    //取消上一次的高亮
    QTextCharFormat fmt;
    fmt.setBackground(Qt::white);
    cursor.mergeCharFormat(fmt);
    //这次的高亮
    cursor.setPosition(index);
    cursor.setPosition(index+length,QTextCursor::KeepAnchor);
    fmt.setBackground(Qt::yellow);
    cursor.mergeCharFormat(fmt);
    lastIndex=index;
    lastLength=length;
}
void MainWindow::receiveString(QString s)
{
    ui->textEdit->setText(s);
    replaceFlag=true;
}
void MainWindow::closeFind()
{
    if(!f)return ;
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.setPosition(lastIndex);
    cursor.setPosition(lastIndex+lastLength,QTextCursor::KeepAnchor);//选择[lastIndex,lastIndex+lastLength]
    //取消上一次的高亮
    QTextCharFormat fmt;
    fmt.setBackground(Qt::white);
    cursor.mergeCharFormat(fmt);
    qDebug()<<"closeFind"<<endl;
}
void MainWindow::mergeformat(const QTextCharFormat &fmt)
{
    QTextCursor cursor = ui->textEdit->textCursor();
    if(!cursor.hasSelection())//没有选择就是全部修改
    {
        cursor.select(QTextCursor::Document);
    }
    cursor.mergeCharFormat(fmt);
    ui->textEdit->mergeCurrentCharFormat(fmt);
}
void MainWindow::textColor()
{
    QColor c =QColorDialog::getColor(Qt::white,this,"更改字体颜色");
    if(c.isValid())
    {
        QTextCharFormat fmt;
        fmt.setForeground(c);
        mergeformat(fmt);
    }
}

void MainWindow:: textFont()
{
    bool f;
    QFont font=QFontDialog::getFont(&f);
    if(f)
    {
        QTextCharFormat fmt;
        fmt.setFont(font);
        mergeformat(fmt);
    }
}

void MainWindow::on_action_New_triggered()
{
    NewFile();
}
void MainWindow::on_action_Open_triggered()
{
    OpenFile();
}
void MainWindow::on_action_Save_triggered()
{
    SaveFile();
}
void MainWindow::on_action_SaveAs_triggered()
{
    SaveAsFile();
}
void MainWindow::on_action_Undo_triggered()
{
    ui->textEdit->undo();
}
void MainWindow::on_action_Redo_triggered()
{
    ui->textEdit->redo();
}
void MainWindow::on_action_Find_triggered()
{
    f=new FindDialog;
    //QString t=ui->textEdit->toPlainText();
    f->content=ui->textEdit->toPlainText().prepend(" ");//传入当前文本
    connect(f,SIGNAL(closed()),this,SLOT(closeFind()));
    connect(f,SIGNAL(sendIndex(int,int)),this,SLOT(receiveIndex(int,int)));
    connect(f,SIGNAL(sendString(QString)),this,SLOT(receiveString(QString)));
    //连接信号与槽
    f->show();
}
void MainWindow::on_action_color_triggered()
{
    textColor();
}
void MainWindow::on_action_fonts_triggered()
{
    textFont();
}
void MainWindow::on_action_Count_triggered()
{
    c=new Count;
    c->content=ui->textEdit->toPlainText().prepend(" ");
    c->count();
    c->show();
}
