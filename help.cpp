#include "header.h"

#include <iostream>
#include <QtCore>


using namespace std;

void globalHelp(){
    cout<<"createDB"<<endl<<"person"<<endl
    <<"subdivision"<<endl<<"position"<<endl<<"rank"<<endl;
    return;
}

void help(QVector<QString>& s){
    if (s.size() == 1){
        globalHelp();
        return;
    }
    if (s[1] == "person"){
        if (s.size()>2){
            if (s[2]=="list"){
                cout<<"Вывод списка всех военнослужащих"<<endl;
                return;
            }
            if (s[2]=="add"){
                cout<<"Добавление военнослужащего"<<endl
                   <<"Параметры:"<<endl
                  <<"-f, --first_name"<<endl
                 <<"[-s, --second_name]"<<endl
                <<"-l, --last_name "<<endl
                <<"-b, --birth_date"<<endl
                <<"-p, --position_id"<<endl
                <<"-r, --rank_id"<<endl
                <<"-pn, --passport_number"<<endl;
                return;
            }
            if (s[2]=="update"){
                cout<<"Обновление данных военнослужащего по id"<<endl
                   <<"Параметры:"<<endl
                  <<"-i, --id"<<endl
                 <<"[-f, --first_name]"<<endl
                <<"[-s, --second_name]"<<endl
                <<"[-l, --last_name]"<<endl
                <<"[-b, --birth_date]"<<endl
                <<"[-p, --position_id]"<<endl
                <<"[-r, --rank_id]"<<endl
                <<"[-pn, --passport_number]"<<endl;
                return;
            }
            if (s[2]=="delete"){
                cout<<"Удаление военнослужащего по id"<<endl
                   <<"Параметры:"<<endl
                  <<"-i, --id"<<endl;
                return;
            }
            if (s[2]=="search"){
                cout<<"Поиск военнослужащего по заданным параметрам"<<endl
                   <<"Параметры:"<<endl
                  <<"[-f, --first_name]"<<endl
                 <<"[-s, --second_name]"<<endl
                <<"[-l, --last_name]"<<endl
                <<"[-b, --birth_date]"<<endl
                <<"[-p, --position_id]"<<endl
                <<"[-r, --rank_id]"<<endl
                <<"[-pn, --passport_number]"<<endl;
                return;
            }
            cout<<"Неизвестный параметр"<<endl;
            return;
        }
        else cout<<"help"<<endl<<"list"<<endl<<"add"<<endl
                <<"update"<<endl<<"delete"<<endl<<"search"<<endl;
        return;
    }
    if (s[1] == "rank"){
        if (s.size()>2){
            if (s[2]=="list") {
                cout<<"Вывод списка всех званий"<<endl;
                return;
            }
            else {
                cout<<"Неизвестный параметр"<<endl;
                return;
            }
        }
        cout<<"help"<<endl<<"list"<<endl;
        return;
    }

    if (s[1] == "subdivision"){
        if (s.size()>2){
            if (s[2]=="list") {
                cout<<"Вывод списка всех подразделений"<<endl;
                return;
            }
            else {
                cout<<"Неизвестный параметр"<<endl;
                return;
            }
        }
        cout<<"help"<<endl<<"list"<<endl;
        return;
    }
    if (s[1] == "position"){
        if (s.size()>2){
            if (s[2]=="free"){
                cout<<"Вывод списка свободных мест на должностях"<<endl
                   <<"Параметры:"<<endl
                  <<"[-i, --id]"<<endl
                 <<"[-s, --subdivision_id]"<<endl;
                return;
            }
            if (s[2]=="list"){
                cout<<"Вывод списка всех должностей"<<endl;
                return;
            }
            cout<<"Неизвестный параметр"<<endl;
            return;
        }
        cout<<"help"<<endl<<"list"<<endl<<"free"<<endl;
        return;
    }
    if (s[1]=="createDB"){
        cout<<"Инициализация базы данных при первом запуске программы"<<endl;
        return;
    }
    else cout<<"Неверный параметр"<<endl;
    return;
}
