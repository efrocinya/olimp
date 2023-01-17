#include <header.h>
#include <QtCore>
#include <QtSql>
#include <iostream>

using namespace std;

struct Person{
    int id;
    QString first_name;
    QString second_name;
    QString last_name;
    QDate birth_date;
    QString Sbirth_date;
    int position_id;
    QString position_name;
    QString subdivision_name;
    int rank_id;
    QString rank_name;
    QString passport;
};

bool checkPerson(Person p, QVector <int> b = QVector <int> (8, 1)){
    QString str;
    QSqlQuery test_query;
    QSqlRecord rec;
    int position_count=0, occuped_count=0, count=0;

    //Check fiob
    if (b[0] || b[1]||b[2]||b[3]){
        str = "SELECT * FROM person "
              "WHERE first_name = '%1' "
              "AND second_name = '%2' "
              "AND last_name = '%3'"
              "AND birth_date = '%4';";
        str = str.arg(p.first_name, p.second_name, p.last_name, p.birth_date.toString("yyyy.MM.dd"));
        bool isfiob = test_query.exec(str);
        if (isfiob){
            while (test_query.next()) count++;
            if (count>0){
                cout<<"Военнослужащий с такими ФИО и датой рождения уже существует. Добавление невозможно"<<endl;
                return 0;
            }
        }
    }

    //check passport
    if (b[6]){
        str = "SELECT * FROM person "
              "WHERE passport_number = '%1';";
        str = str.arg(p.passport);
        bool ispn = test_query.exec(str);
        if (ispn){
            while (test_query.next()) count++;
            if (count>0){
                cout<<"Военнослужащий с такими паспортными данными уже существует. Добавление невозможно"<<endl;
                return 0;
            }
        }
    }

    //check freePosition
    if (b[4]){
        str = "SELECT id FROM person "
              "WHERE position_id = %1;";
        str = str.arg(p.position_id);
        bool isp = test_query.exec(str);
        if (isp){
            while (test_query.next()) occuped_count++;
        }

        str = "SELECT count FROM position "
              "WHERE id = %1;";
        str = str.arg(p.position_id);
        bool isOk = test_query.exec(str);
        if (isOk){
            rec = test_query.record();
            bool isInt =0;
            while (test_query.next()) position_count = test_query.value(rec.indexOf("count")).toInt(&isInt);
            if (position_count == 0){
                cout << "Неверно введен id должности, добавление невозможно"<<endl;
                return 0;
            }
            if (position_count<=occuped_count){
                cout<<"Все места на данной должности заняты. Добавление невозможно"<<endl;
                return 0;
            }
        }
    }

    //check rank_id
    if (b[5]){
        str = "SELECT name FROM rank "
              "WHERE id = %1;";
        str = str.arg(p.rank_id);
        bool isOk = test_query.exec(str);
        if (isOk){
            isOk = 0;
            while (test_query.next()) isOk =1;
        }
        if (!isOk){
            cout<<"Неверно введен id звания. Добавление невозможно"<<endl;
            return 0;
        }

    }

    //check age
    if (b[3]){
        QDate today = QDate::currentDate();
        int years = today.year() - p.birth_date.year();
        if (years < 16){
            cout<<"Военнослужащему меньше 16 лет, добавление невозможно"<<endl;
            return 0;
        }
        if (years == 16){
            int months = today.month() - p.birth_date.month();
            int days = today.day() - p.birth_date.day();
            if (days<0) months--;
            if (months<0){
                cout<<"Военнослужащему меньше 16 лет, добавление невозможно"<<endl;
                return 0;
            }
        }
    }
    return 1;
}

Person checkPersonById(int id, bool &find){
    Person p;
    QSqlQuery s_query;
    QString str;
    find =0;
    str = "SELECT * FROM person WHERE id = %1";
    str = str.arg(id);
    bool select = s_query.exec(str);
    if (!select){
        qDebug()<<"Нет доступа к таблицe person";
        return p;
    }
    QSqlRecord rec = s_query.record();
    while (s_query.next()){
        find =1;
        p.id = s_query.value(rec.indexOf("id")).toInt();
        p.first_name = s_query.value(rec.indexOf("first_name")).toString();
        p.second_name = s_query.value(rec.indexOf("second_name")).toString();
        p.last_name = s_query.value(rec.indexOf("last_name")).toString();
        p.birth_date = s_query.value(rec.indexOf("birth_date")).toDate();
        p.position_id = s_query.value(rec.indexOf("position_id")).toInt();
        p.rank_id = s_query.value(rec.indexOf("rank_id")).toInt();
        p.passport = s_query.value(rec.indexOf("passport_number")).toString();

    }
    return p;
}

bool addPerson(Person person){
    QString str_insert, str;
    QSqlQuery i_query;
    if (person.second_name.isNull()){
        str_insert = "INSERT INTO person (first_name, last_name, birth_date, position_id, rank_id, passport_number)"
                     " VALUES ('%1', '%2', '%3', %4, %5, '%6');";
        str = str_insert.arg(person.first_name).arg(person.last_name).arg(person.birth_date.toString("yyyy.MM.dd"))
                .arg(person.position_id).arg(person.rank_id).arg(person.passport);

        bool insert = i_query.exec(str);
        if (!insert){
            qDebug() << "Данные в person не вставляются"<<i_query.lastError();
            return 0;
        }
        else qDebug()<<"Данные в person успешно добавлены";
        return 1;
    }
    str_insert = "INSERT INTO person (first_name, second_name, last_name, birth_date, position_id, rank_id, passport_number)"
                 " VALUES ('%1', '%2', '%3', '%4', %5, %6, '%7');";
    str = str_insert.arg(person.first_name).arg(person.second_name).arg(person.last_name).arg(person.birth_date.toString("yyyy.MM.dd"))
            .arg(person.position_id).arg(person.rank_id).arg(person.passport);

    bool insert = i_query.exec(str);
    if (!insert){
        qDebug() << "Данные в person не вставляются"<<i_query.lastError();
        return 0;
    }
    else qDebug()<<"Данные в person успешно добавлены";
    return 1;
}

void deletePerson(int id){
    QString str = "DELETE FROM person WHERE id = %1";
    str = str.arg(id);
    QSqlQuery delete_query;
    bool b = delete_query.exec(str);
    if (b){
        cout<<"Данные успешно удалены"<<endl;
        return;
    }
    cout<<"Ошибка при удалении данных"<<delete_query.lastError().text().toStdString()<<endl;
}

Person checkParams(QVector <QString>& s, int start, QVector<int>& b, Person person){
    for (int i=start; i<s.size(); i++){
        if (s[i] == "-f" or s[i] == "--first_name"){
            if (s.size()>i+1){
                b[0]=1;
                person.first_name = s[++i];
            }
            else{
                b[0]=-1;
                cout<<"Не введены значения для параметра 'Имя'"<<endl;
                return person;
            }
            continue;
        }
        if (s[i] == "-s" or s[i] == "--second_name"){
            if (s.size()>i+1){
                b[1]=1;
                person.second_name = s[++i];
            }
            else{
                b[1]=-1;
                cout<<"Не введены значения для параметра 'Отчество'"<<endl;
                return person;
            }
            continue;
        }
        if (s[i] == "-l" or s[i] == "--last_name"){
            if (s.size()>i+1){
                b[2]=1;
                person.last_name = s[++i];
            }
            else{
                b[2]=-1;
                cout<<"Не введены значения для параметра 'Фамилия'"<<endl;
                return person;
            }
            continue;
        }
        if (s[i] == "-b" or s[i] == "--birth_date"){
            if (s.size()>i+1){
                b[3]=1;
                person.birth_date = QDate::fromString(s[++i], "dd.MM.yyyy");
            }
            else{
                b[3]=-1;
                cout<<"Не введены значения для параметра 'Дата рождения'"<<endl;
                return person;
            }
            if (!person.birth_date.isValid()){
                cout<<"Неверный формат даты";
                b[3]=-1;
                return person;
            }
            continue;
        }
        if (s[i] == "-p" or s[i] == "--position_id"){
            if (s.size()>i+1){
                b[4]=1;
                bool isNumber = 0;
                person.position_id = s[++i].toInt(&isNumber);
                if (!isNumber){
                    b[4]=-1;
                    cout<<"Неверный формат id должности"<<endl;
                    return person;
                }
            }
            else{
                b[4]=-1;
                cout<<"Не введены значения для параметра 'id должности'"<<endl;
                return person;
            }
            continue;
        }
        if (s[i] == "-r" or s[i] == "--rank_id"){
            if (s.size()>i+1){
                b[5]=1;
                bool isNumber;
                person.rank_id = s[++i].toInt(&isNumber);
                if (!isNumber){
                    b[5]=-1;
                    cout<<"Неверный формат id звания"<<endl;
                    return person;
                }
            }
            else{
                b[5]=-1;
                cout<<"Не введены значения для параметра 'id звания'"<<endl;
                return person;
            }
            continue;
        }
        if (s[i] == "-pn" or s[i] == "--passport_number"){
            if (s.size()>i+1){
                b[6]=1;
                person.passport = s[++i];
                bool isNumber;
                person.passport.toLong(&isNumber);
                if (!isNumber || person.passport.length()!=10){
                    b[6]=-1;
                    cout<<"Неверный формат паспорта"<<endl;
                    return person;
                }
            }
            else{
                b[6]=-1;
                cout<<"Не введены значения для параметра 'Номер паспорта'"<<endl;
                return person;
            }
            continue;
        }
    }
    return person;
}

bool updatePerson(Person p, QVector<int>& b){
    QSqlQuery u_query;
    for (int i=0; i<b.length()-1; i++){
        if (b[i]==1){
            QString paramName, arg;
            int intArg;
            switch (i) {
            case 0:
                paramName="first_name";
                arg = p.first_name;
                break;
            case 1:
                paramName="second_name";
                arg = p.second_name;
                break;
            case 2:
                paramName="last_name";
                arg = p.last_name;
                break;
            case 3:
                paramName="birth_date";
                arg = p.birth_date.toString("yyyy.MM.dd");
                break;
            case 4:
                paramName="position_id";
                intArg = p.position_id;
                break;
            case 5:
                paramName="rank_id";
                intArg = p.rank_id;
                break;
            case 6:
                paramName="passport_number";
                arg = p.passport;
                break;
            default: return 0;
            }

            QString str;
            if (i == 4 || i==5){
                str = "UPDATE person SET '%1' = %2 WHERE id = %3";
                str = str.arg(paramName).arg(intArg).arg(p.id);
            }
            else {
                str = "UPDATE person SET '%1' = '%2' WHERE id = %3";
                str = str.arg(paramName).arg(arg).arg(p.id);
            }

            bool b = u_query.exec(str);
            if (!b){
                qDebug()<<"Ошибка обновления"<<u_query.lastError();
                return 0;
            }
            else return 1;
        }
    }
    return 1;
}

void searchPerson(Person p, QVector<int>& b){
    QString str, argsStr = "";
    int intArg;
    QSqlQuery s_query;
    QSqlRecord rec;
    str = "SELECT person.id, person.first_name, person.second_name, person.last_name, "
          "person.birth_date, position.name, rank.name, subdivision.name, person.passport_number "
          "FROM person, position, rank, subdivision "
          "WHERE person.position_id = position.id "
          "AND person.rank_id = rank.id "
          "AND position.subdivision_id = subdivision.id "
          "%1;";
    for (int i=0; i<b.length()-1; i++){
        if (b[i]==1){
            QString insertStr="", paramName, arg;
            switch (i) {
            case 0:
                paramName="first_name";
                arg = p.first_name;
                break;
            case 1:
                paramName="second_name";
                arg = p.second_name;
                break;
            case 2:
                paramName="last_name";
                arg = p.last_name;
                break;
            case 3:
                paramName="birth_date";
                arg = p.birth_date.toString("yyyy.MM.dd");
                break;
            case 4:
                paramName="position_id";
                intArg = p.position_id;
                break;
            case 5:
                paramName="rank_id";
                intArg = p.rank_id;
                break;
            case 6:
                paramName="passport_number";
                arg = p.passport;
                break;
            default: return;
            }
            if (i == 4 || i==5){
                insertStr = "AND person.%1 = %2 ";
                insertStr = insertStr.arg(paramName).arg(intArg);
            }
            else {
                insertStr = "AND person.%1 = '%2' ";
                insertStr = insertStr.arg(paramName).arg(arg);
            }
            argsStr += insertStr;
        }
    }
    str = str.arg(argsStr);
    if (!s_query.exec(str)) {
        qDebug() << "Селект person не получается"<<s_query.lastError();
        return;
    }
    else {
        rec = s_query.record();
        int findCount =0;
        while (s_query.next()) {
            findCount++;
            p.id = s_query.value(rec.indexOf("person.id")).toInt();
            p.first_name = s_query.value(rec.indexOf("person.first_name")).toString();
            p.second_name = s_query.value(rec.indexOf("person.second_name")).toString();
            p.last_name = s_query.value(rec.indexOf("person.last_name")).toString();
            p.birth_date = s_query.value(rec.indexOf("person.birth_date")).toDate();
            p.position_name = s_query.value(rec.indexOf("position.name")).toString();
            p.rank_name = s_query.value(rec.indexOf("rank.name")).toString();
            p.passport = s_query.value(rec.indexOf("person.passport_number")).toString();
            p.subdivision_name = s_query.value(rec.indexOf("subdivision.name")).toString();

            qDebug() << "Военнослужащий: {id:" << p.id
                     << "; имя:" << p.first_name
                     << "; отчество:" << p.second_name
                     << "; фамилия:" << p.last_name
                     << "; дата рождения:" << p.birth_date.toString("dd.MM.yyyy")
                     << "; должность:" << p.subdivision_name << "," << p.position_name
                     << "; звание:" << p.rank_name
                     << "; номер паспорта:" << p.passport << "}";
        }
        if (!findCount){
            cout<<"По Вашему запросу ничего не найдено"<<endl;
            return;
        }
    }

    return;
}

void personList(){
    Person person;
    QSqlQuery a_query;
    QSqlRecord rec;
    QString str;
    str = "SELECT person.id, person.first_name, person.second_name, person.last_name, "
          "person.birth_date, position.name, rank.name, subdivision.name, person.passport_number "
          "FROM person, position, rank, subdivision "
          "WHERE person.position_id = position.id "
          "AND person.rank_id = rank.id "
          "AND position.subdivision_id = subdivision.id;";
    if (!a_query.exec(str)) {
        qDebug() << "Селект person не получается"<<a_query.lastError();
        return;
    }
    else {
        rec = a_query.record();
        while (a_query.next()) {
            person.id = a_query.value(rec.indexOf("person.id")).toInt();
            person.first_name = a_query.value(rec.indexOf("person.first_name")).toString();
            person.second_name = a_query.value(rec.indexOf("person.second_name")).toString();
            person.last_name = a_query.value(rec.indexOf("person.last_name")).toString();
            person.birth_date = a_query.value(rec.indexOf("person.birth_date")).toDate();
            person.position_name = a_query.value(rec.indexOf("position.name")).toString();
            person.rank_name = a_query.value(rec.indexOf("rank.name")).toString();
            person.passport = a_query.value(rec.indexOf("person.passport_number")).toString();
            person.subdivision_name = a_query.value(rec.indexOf("subdivision.name")).toString();

            qDebug() << "Военнослужащий: {id:" << person.id
                     << "; имя:" << person.first_name
                     << "; отчество:" << person.second_name
                     << "; фамилия:" << person.last_name
                     << "; дата рождения:" << person.birth_date.toString("dd.MM.yyyy")
                     << "; должность:" << person.subdivision_name << "," << person.position_name
                     << "; звание:" << person.rank_name
                     << "; номер паспорта:" << person.passport << "}";
        }
    }
    return;
}

void people(QVector<QString>& s){
    if (s.count() > 1){
        if (s[1] == "help"){
            s[1]="person";
            help(s);
        }
        else if (s[1]=="add"){
            QVector <int> b(8, 0);
            Person person;
            person = checkParams(s, 2, b, person);
            if (b[7]==-1){
                cout<<"Неизвестный параметр"<<endl;
                return;
            }
            for (int i=0; i<7; i++){
                if (b[i]!=1){
                    if (b[i]==0 && i!=1){
                        b[7]=-1;
                    }
                    if (b[i]==-1){
                        return;
                    }
                }
            }
            if (b[7]==-1){
                cout<<"Не хватает обязательных параметров"<<endl;
                return;
            }

            if (checkPerson(person)) addPerson(person);
            else return;
        }
        else if (s[1]=="update"){
            if (s.size()>5){
                if (s[2]=="-i" || s[2]=="--id"){
                    int id = s[3].toInt();
                    bool find = 0;
                    Person p = checkPersonById(id, find);
                    if (find){
                        QVector <int> b(8, 0);
                        Person person = checkParams(s, 4, b, p);
                        if (b[7]==-1){
                            cout<<"Неизвестный параметр"<<endl;
                            return;
                        }
                        for (int i=0; i<7; i++){
                            if (b[i]!=1){
                                if (b[i]==-1){
                                    return;
                                }
                            }
                        }
                        if (checkPerson(person, b)){
                            if (updatePerson(person, b)){
                                cout<<"Данные обновлены"<<endl;
                            }
                            else{
                                cout<<"Данные не обновлены"<<endl;
                                return;
                            }
                        }
                    }
                    else {
                        cout<<"Не найден военнослужащий с таким id. Изменение невозможно"<<endl;
                        return;
                    }
                }
                else {
                    cout<<"Неизвестный параметр, ожидалось '-i', '--id' "<<endl;
                    return;
                }
            }
            else{
                cout<<"Не хватает обязательных параметров"<<endl;
                return;
            }
        }
        else if (s[1]=="delete"){
            if (s.size()<4){
                cout<<"Не хватает параметров, удаление невозможно"<<endl;
                return;
            }
            if (s[2]=="-i" || s[2] == "--id"){

                int id = s[3].toInt();
                bool find;
                checkPersonById(id, find);
                if (find){
                    deletePerson(id);
                }
                else cout<<"Не найден военнослужащий с таким id. Удаление невозможно"<<endl;
            }
        }

        else if (s[1]=="list"){
            personList();
        }

        else if (s[1]=="search"){
            if (s.size()<4){
                cout<<"Не хватает обязательных параметров"<<endl;
                return;
            }
            QVector <int> b(8, 0);
            Person person;
            person = checkParams(s, 2, b, person);
            if (b[7]==-1){
                cout<<"Неизвестный параметр"<<endl;
                return;
            }
            for (int i=0; i<7; i++){
                if (b[i]!=1){
                    if (b[i]==-1){
                        cout<<"Неверно введен параметр"<<endl;
                        return;
                    }
                    if (b[i]==-2) return;
                }
            }
            searchPerson(person, b);
        }
        else{
            cout<<"Неверно введен параметр, информацию можно посмотреть по команде help"<<endl;
        }
    }
    else{
        s.push_back("person");
        help(s);
    }
    return;
}
