#include "createdb.h"
#include <iostream>
#include <QtSql>

void createDb()
{
    QList<QString> list;

    QSqlQuery c_query, i_query, s_query, test_query;
    QString str, str_insert;

    /*bool drop = c_query.exec("DROP TABLE person;");
    if (!drop) qDebug()<<"no drop"<<c_query.lastError();
    else qDebug()<<"drop";*/

    bool b = test_query.exec("SELECT * FROM rank WHERE id = 1;");
    if (!b){
        str = "CREATE TABLE rank("
                    "id integer PRIMARY KEY AUTOINCREMENT, "
                    "name VARCHAR(200)"
                    ");";
        bool create = c_query.exec(str);
        if (!create) {
            qDebug() << "Не удается создать таблицу rank";
        }
        else qDebug()<<"Таблица rank создана";

        list = {"рядовой","ефрейтор","младший сержант",
                "сержант","старший сержант","старшина",
                "прапорщик","старший прапорщик",
                "лейтенант","старший лейтенант",
                "капитан","майор","подполковник",
                "полковник","генерал-майор","генерал-лейтенант",
                "генерал-полковник", "генерал армии"};

        QString rank_name;
        foreach (rank_name, list){
            QString str_insert = "INSERT INTO rank(name) VALUES ('%1');";
            QString str = str_insert.arg(rank_name);

            bool insert = i_query.exec(str);
            if (!insert){
                qDebug() << "Данные в rank не вставляются";
            }
        }
    }

    b = test_query.exec("SELECT * FROM subdivision WHERE id = 1;");
    if (!b){
        qDebug() << "Таблицы subdivision нет";
        str = "CREATE TABLE subdivision("
                    "id integer PRIMARY KEY AUTOINCREMENT, "
                    "name VARCHAR(200)"
                    ");";

        bool create = c_query.exec(str), insert=0;
        if (!create) {
            qDebug() << "Не удается создать таблицу subdivision";
        }
        else qDebug()<<"Таблица subdivision создана";

        list = {"1 взвод", "2 взвод", "3 взвод"};
        QString subdivision_name;
        foreach(subdivision_name, list){
            str_insert = "INSERT INTO subdivision(name) VALUES ('%1');";
            str = str_insert.arg(subdivision_name);

            insert = i_query.exec(str);
            if (!insert){
                qDebug() << "Данные в subdivision не вставляются";
            }
        }
    }

    b = test_query.exec("SELECT * FROM position WHERE id = 1;");
    if (!b){
        str = "CREATE TABLE position("
                    "id integer PRIMARY KEY AUTOINCREMENT, "
                    "name VARCHAR(200),"
                    "subdivision_id integer,"
                    "count integer NOT NULL,"
                    "FOREIGN KEY (subdivision_id) REFERENCES subdivision (id)"
                    ");";
        bool create = c_query.exec(str), insert =0;
        if (!create){
            qDebug() << "Таблица position не создается"<<endl<<c_query.lastError();
        }
        else qDebug()<<"Таблица position создана";

        list = {"командир взвода", "командир отделения", "курсант"};
        int arr_subdivision[] = {1, 2, 3};
        int arr_count[] = {1, 3, 26};
        QString position_name;

        for (int i = 0; i<3; i++){
            for (int j = 0; j<3; j++){
                str_insert = "INSERT INTO position(name, subdivision_id, count) VALUES ('%1', %2, %3);";
                str = str_insert.arg(list[j]).arg(arr_subdivision[i]).arg(arr_count[j]);

                insert = i_query.exec(str);
                if (!insert){
                    qDebug() << "Данные в position не вставляются"<<endl<<i_query.lastError();
                }
            }
        }
    }



    b = test_query.exec("SELECT * FROM person WHERE id = 1;");
    if (!b){
        qDebug() << "Таблицы person нет";
        str = "CREATE TABLE person("
                    "id integer PRIMARY KEY AUTOINCREMENT, "
                    "first_name VARCHAR(30) NOT NULL, "
                    "second_name VARCHAR(30), "
                    "last_name VARCHAR(30) NOT NULL, "
                    "birth_date DATE NOT NULL, "
                    "position_id integer NOT NULL, "
                    "rank_id integer NOT NULL, "
                    "passport_number VARCHAR(10), "
                    "FOREIGN KEY (position_id) REFERENCES position (id), "
                    "FOREIGN KEY (rank_id) REFERENCES rank (id)"
                    ");";

        bool create = c_query.exec(str);
        if (!create){
            qDebug() << "Таблица person не создается"<<endl<<c_query.lastError();
        }
        else qDebug()<<"Таблица person создана";
    }
    return;
}
