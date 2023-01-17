#include <header.h>
#include <QtCore>
#include <QtSql>
#include <iostream>

using namespace std;

void position(QVector<QString>& s){
    if (s.count() > 1){
        if (s[1] == "help"){
            s[1]="position";
            help(s);
        }
        else if (s[1]=="list"){
            QSqlQuery a_query;
            QSqlRecord rec;
            QString str;
            int number, count;
            QString name, subdivision_name;
            str = "SELECT position.id, position.name, subdivision.name, position.count "
                    "FROM subdivision, position "
                    "WHERE position.subdivision_id=subdivision.id;";
            if (!a_query.exec(str)) {
                qDebug() << "Селект position не выполнен"<<a_query.lastError();
                return;
            }
            else {
                rec = a_query.record();
                while (a_query.next()) {
                    number = a_query.value(rec.indexOf("position.id")).toInt();
                    name = a_query.value(rec.indexOf("position.name")).toString();
                    subdivision_name = a_query.value(rec.indexOf("subdivision.name")).toString();
                    count = a_query.value(rec.indexOf("position.count")).toInt();
                    qDebug() << "Должность: {id:" << number
                             << "; название:" << name
                             << "; подразделение:" << subdivision_name
                             << "; количество:" << count << "}";
                }
            }

        }
        else if (s[1]=="free"){
            if (s.size()==2){
                QSqlQuery a_query, s_query;
                QSqlRecord rec, rec_person;
                QString str;
                int number, count, occuped_count, free_count, seemed_count =0;
                QString name, subdivision_name;
                str = "SELECT position.id, position.name, subdivision.name, position.count "
                        "FROM subdivision, position "
                        "WHERE position.subdivision_id=subdivision.id;";
                if (!a_query.exec(str)) {
                    qDebug() << "Селект position не получается"<<a_query.lastError();
                    return;
                }
                else {
                    rec = a_query.record();
                    while (a_query.next()) {
                        number = a_query.value(rec.indexOf("position.id")).toInt();
                        name = a_query.value(rec.indexOf("position.name")).toString();
                        subdivision_name = a_query.value(rec.indexOf("subdivision.name")).toString();
                        count = a_query.value(rec.indexOf("position.count")).toInt();

                        occuped_count =0;
                        str = "SELECT id FROM person WHERE position_id = %1;";
                        str = str.arg(number);
                        bool b = s_query.exec(str);
                        if (!b) {
                            qDebug()<<"Ошибка при запросе в person"<<s_query.lastError();
                            return;
                        }
                        rec_person = s_query.record();
                        while (s_query.next()) occuped_count++;
                        free_count = count-occuped_count;
                        if (free_count>0){
                            seemed_count++;
                            qDebug() << "Должность: {id:" << number
                                     << "; название:" << name
                                     << "; подразделение:" << subdivision_name
                                     << "; количество свободных:" << free_count << "}";
                        }
                    }
                    if (!seemed_count){
                        cout<<"Нет свободных должностей"<<endl;
                    }
                }

            }
            else if (s.size()==4){
                QString param;
                int arg;
                if (s[2] == "-i" || s[2] == "--id"){
                    param = "id";
                }
                else if (s[2] == "-s" || s[2] == "--subdivision_id"){
                    param = "subdivision_id";
                }
                else {
                    qDebug()<<s[2];
                    cout<<"Неизвестный параметр"<<endl;
                    return;
                }
                bool isNumber = 0;
                arg = s[3].toInt(&isNumber);
                if (!isNumber){
                    cout<<"Неверно введен параметр";
                    return;
                }
                QSqlQuery a_query, s_query;
                QSqlRecord rec, rec_person;
                QString str;
                int number, count, occuped_count, free_count, result_count=0, seemed_count=0;
                QString name, subdivision_name;
                str = "SELECT position.id, position.name, subdivision.name, position.count "
                      "FROM subdivision, position "
                      "WHERE position.%1 = %2 "
                      "AND position.subdivision_id=subdivision.id;";
                str = str.arg(param).arg(arg);

                if (!a_query.exec(str)) {
                    qDebug() << "Селект position не выполнен"<<a_query.lastError();
                    return;
                }
                else {
                    rec = a_query.record();
                    while (a_query.next()) {
                        result_count++;
                        number = a_query.value(rec.indexOf("position.id")).toInt();
                        name = a_query.value(rec.indexOf("position.name")).toString();
                        subdivision_name = a_query.value(rec.indexOf("subdivision.name")).toString();
                        count = a_query.value(rec.indexOf("position.count")).toInt();

                        occuped_count = 0;
                        str = "SELECT id FROM person WHERE position_id = %1;";
                        str = str.arg(number);
                        bool b = s_query.exec(str);
                        if (!b) {
                            qDebug()<<"Ошибка при запросе в person"<<s_query.lastError();
                            return;
                        }
                        rec_person = s_query.record();
                        while (s_query.next()) occuped_count++;
                        free_count = count-occuped_count;
                        if (free_count>0){
                            seemed_count++;
                            qDebug() << "Должность: {id:" << number
                                     << "; название:" << name
                                     << "; подразделение:" << subdivision_name
                                     << "; количество свободных:" << free_count << "}";
                        }

                    }
                    if (!result_count){
                        str = "Неверно введен %1";
                        str = str.arg(param);
                        cout<<str.toStdString()<<endl;
                        return;
                    }
                    if (!seemed_count){
                        cout<<"Нет свободных должностей по Вашему запросу"<<endl;
                        return;
                    }
                }
            }
            else{
                cout<<"Не хватает обязательных параметров"<<endl;
                return;
            }
        }
        else{
            cout<<"Неверно введен параметр, информацию можно посмотреть по команде help"<<endl;
            return;
        }
    }
    else{
        s.push_back("position");
        help(s);
    }

    return;
}
