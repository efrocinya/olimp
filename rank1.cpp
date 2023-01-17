#include <header.h>
#include <QtCore>
#include <QtSql>
#include <iostream>

using namespace std;

void rank1(QVector <QString>& s)
{
    if (s.count() > 1){
        if (s[1] == "help"){
            s[1]="rank";
            help(s);
        }
        else if (s[1]=="list"){
            QSqlQuery a_query;
            QSqlRecord rec;
            int number;
            QString name;
            if (!a_query.exec("SELECT * FROM rank")) {
                qDebug() << "Селект rank не выполнен";
                return;
            }
            rec = a_query.record();
            number = 0;
            name = "";
            while (a_query.next()) {
                number = a_query.value(rec.indexOf("id")).toInt();
                name = a_query.value(rec.indexOf("name")).toString();
                qDebug() << "Звание {id:" << number << "; название:" << name <<"}";
            }
        }
        else cout<<"Неверно введен параметр. Информацию можно посмотреть по команде help"<<endl;
    }
    else{
        s.push_back("rank");
        help(s);
    }

    return;
}
