#include <QCoreApplication>
#include <iostream>
#include <QtSql>

#include <header.h>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString s;
    QVector<QString> args(0);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("my_db.sqlite");

    if (!db.open()){
        qDebug() << "<БД не открывается";
        return -1;
        createDb();
    }

    if (argc > 1){
        for (int i=1; i< argc; i++){
            s = argv[i];
            args.push_back(s);
        }
        if (args[0]=="help") {
            help(args);
        }
        else if (args[0]=="rank"){
            rank1(args);
        }
        else if (args[0]=="subdivision"){
            subdivision(args);
        }
        else if (args[0]=="position"){
            position(args);
        }
        else if (args[0]=="person"){
            people(args);
        }
        else if (args[0]=="createDB"){
            createDb();
        }
        else cout<<"Неверно введен параметр, информацию можно посмотреть по команде help"<<endl;
    }
    else globalHelp();

    return 0;
}
