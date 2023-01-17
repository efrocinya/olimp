#include <header.h>
#include <QtCore>
#include <QtSql>
#include <iostream>

using namespace std;

void subdivision(QVector<QString>& s){
    if (s.count() > 1){
        if (s[1] == "help"){
            s[1]="subdivision";
            help(s);
        }
        else if (s[1]=="list"){
            QSqlQuery a_query;
            QSqlRecord rec;
            int number;
            QString name;
            if (!a_query.exec("SELECT * FROM subdivision")) {
                qDebug() << "Селект subdivision не выполнен";
                return;
            }
            else{
                rec = a_query.record();
                while (a_query.next()) {
                    number = a_query.value(rec.indexOf("id")).toInt();
                    name = a_query.value(rec.indexOf("name")).toString();
                    qDebug() << "Подразделение {id:" << number
                             << "; название:" << name << "}";
                }
            }
        }
        else{
            cout<<"Неверно введен параметр, информацию можно посмотреть по команде help"<<endl;
        }
    }
    else{
        s.push_back("subdivision");
        help(s);
    }

    return;
}
