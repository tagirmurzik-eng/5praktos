#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include <memory>
#define NOMINMAX
#include <Windows.h>

using namespace std;

class Book {
private:
    string title;
    string author;
    int year;
    bool is_issued;

public:
    Book(string b_title, string b_author, int b_year) {
        title = b_title;
        author = b_author;
        year = b_year;
        is_issued = false;
    }

    string get_title() const { return title; }
    string get_author() const { return author; }
    int get_year() const { return year; }
    bool get_is_issued() const { return is_issued; }

    void set_title(string b_title) { title = b_title; }
    void set_author(string b_author) { author = b_author; }
    void set_year(int b_year) { year = b_year; }
    void set_is_issued(bool status) { is_issued = status; }

    void print() const {
        cout << "\"" << title << "\" - " << author
            << " (" << year << ")";
        if (is_issued) cout << " ВЫДАНА";
        else cout << " ДОСТУПНА";
        cout << endl;
    }
};

class Reader {
private:
    string name;
    vector<shared_ptr<Book>> borrowed_books;
public:
    Reader(string r_name) {
        name = r_name;
    }

    string get_name() const { return name; }
    void set_name(string r_name) { name = r_name; }

    vector<shared_ptr<Book>>& get_borrowed_books() {
        return borrowed_books;
    }

    void print() const {
        cout << "Читатель: " << name
            << " Книг на руках: "
            << borrowed_books.size()<< endl;
    }
};


int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    vector<shared_ptr<Book>> books;
    vector<unique_ptr<Reader>> readers;

    //данные книги
    books.push_back(make_shared<Book>("Криштиану Роналду", "Л.Кайоли", 2012));
    books.push_back(make_shared<Book>("Война и мир", "Л.Толстой", 1869));
    books.push_back(make_shared<Book>("Мастер и Маргарита", "М.Булгаков", 1967));
    books.push_back(make_shared<Book>("Отцы и дети", "И.Тургенев", 1862));

    readers.push_back(make_unique<Reader>("Мурзагалиев Тагир"));
    readers.push_back(make_unique<Reader>("Литвин Михаил"));

    readers[0]->get_borrowed_books().push_back(books[0]);
    books[0]->set_is_issued(true);

    int choice{ -1 };

    do {
        cout << "\nВыберите пункт меню:" << endl;
        cout << "1. Список книг" << endl;
        cout << "2. Список читателей" << endl;
        cout << "3. Добавить книгу" << endl;
        cout << "4. Изменить книгу" << endl;
        cout << "5. Удалить книгу" << endl;
        cout << "6. Добавить читателя" << endl;
        cout << "7. Изменить читателя" << endl;
        cout << "8. Удалить читателя" << endl;
        cout << "9. Выдать книгу читателю" << endl;
        cout << "10. Посмотреть книги читателя" << endl;
        cout << "11. Вернуть книгу в библиотеку" << endl;
        cout << "0. Выход" << endl;
        cout << "Выбор: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ошибка ввода. Введите число." << endl;
            choice = -1;
            continue;
        }

        switch (choice) {
        case 1:
            cout << "\nСписок книг библиотеки:" << endl;
            for (int i{ 0 }; i < books.size(); i++) {
                cout << i + 1 << ". ";
                books[i]->print();
            }
            break;

        case 2:
            cout << "\nСписок читателей:" << endl;
            for (int i{ 0 }; i < readers.size(); i++) {
                cout << i + 1 << ". ";
                readers[i]->print();
            }
            break;

        case 3: {
            string title;
            string author;
            int year{ 0 };

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Введите название книги: ";
            getline(cin, title);

            cout << "Введите автора: ";
            getline(cin, author);

            cout << "Введите год издания: ";

            if (!(cin >> year)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ошибка. Год должен быть числом." << endl;
                break;
            }

            books.push_back(make_shared<Book>(title, author, year));

            cout << "Книга добавлена." << endl;
            break;
        }

        case 4: {
            int number{ 0 };

            cout << "Введите номер книги для изменения: ";

            if (!(cin >> number)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ошибка ввода." << endl;
                break;
            }

            if (number < 1 || number > books.size()) {
                cout << "Книга не найдена." << endl;
                break;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            string title;
            string author;
            int year{ 0 };

            cout << "Введите новое название: ";
            getline(cin, title);

            cout << "Введите нового автора: ";
            getline(cin, author);

            cout << "Введите новый год издания: ";

            if (!(cin >> year)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ошибка. Год должен быть числом." << endl;
                break;
            }

            books[number - 1]->set_title(title);
            books[number - 1]->set_author(author);
            books[number - 1]->set_year(year);

            cout << "Данные книги изменены." << endl;
            break;
        }

        case 5: {
            int number{ 0 };

            cout << "Введите номер книги для удаления: ";

            if (!(cin >> number)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ошибка ввода." << endl;
                break;
            }

            if (number < 1 || number > books.size()) {
                cout << "Книга не найдена." << endl;
                break;
            }

            if (books[number - 1]->get_is_issued()) {
                cout << "Нельзя удалить выданную книгу." << endl;
                break;
            }

            books.erase(books.begin() + number - 1);

            cout << "Книга удалена." << endl;
            break;
        }

        case 6: {
            string name;

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Введите никнейм читателя: ";
            getline(cin, name);

            readers.push_back(make_unique<Reader>(name));

            cout << "Читатель добавлен." << endl;
            break;
        }

        case 7: {
            int number{ 0 };

            cout << "Введите номер читателя: ";

            if (!(cin >> number)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ошибка ввода." << endl;
                break;
            }

            if (number < 1 || number > readers.size()) {
                cout << "Читатель не найден." << endl;
                break;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            string name;

            cout << "Введите новый никнейм: ";
            getline(cin, name);

            readers[number - 1]->set_name(name);

            cout << "Данные читателя изменены." << endl;
            break;
        }

        case 8: {
            int number{ 0 };

            cout << "Введите номер читателя: ";

            if (!(cin >> number)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ошибка ввода." << endl;
                break;
            }

            if (number < 1 || number > readers.size()) {
                cout << "Читатель не найден." << endl;
                break;
            }

            if (!readers[number - 1]->get_borrowed_books().empty()) {
                cout << "Нельзя удалить читателя, у которого есть книги." << endl;
                break;
            }

            readers.erase(readers.begin() + number - 1);

            cout << "Читатель удален." << endl;
            break;
        }
        case 9: {
            int reader_number{ 0 };
            int book_number{ 0 };

            cout << "Введите номер читателя: ";
            if (!(cin >> reader_number)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ошибка ввода." << endl;
                break;
            }

            if (reader_number < 1 || reader_number > readers.size()) {
                cout << "Такого читателя нет." << endl;
                break;
            }

            cout << "Введите номер книги: ";
            if (!(cin >> book_number)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ошибка ввода." << endl;
                break;
            }

            if (book_number < 1 || book_number > books.size()) {
                cout << "Такой книги нет." << endl;
                break;
            }

            if (books[book_number - 1]->get_is_issued()) {
                cout << "Эта книга уже выдана." << endl;
                break;
            }

            readers[reader_number - 1]->get_borrowed_books()
                .push_back(books[book_number - 1]);

            books[book_number - 1]->set_is_issued(true);

            cout << "Книга успешно выдана читателю." << endl;
            break;
        }
        case 10: {
            int reader_number{ 0 };

            cout << "Введите номер читателя: ";
            if (!(cin >> reader_number)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ошибка ввода." << endl;
                break;
            }

            if (reader_number < 1 || reader_number > readers.size()) {
                cout << "Такого читателя нет." << endl;
                break;
            }

            auto& borrowed =
                readers[reader_number - 1]->get_borrowed_books();

            if (borrowed.empty()) {
                cout << "У читателя нет книг на руках." << endl;
            }
            else {
                cout << "Книги читателя:" << endl;

                for (size_t i = 0; i < borrowed.size(); i++) {
                    cout << i + 1 << ". ";
                    borrowed[i]->print();
                }
            }

            break;
        }
        case 11: {
            int reader_number{ 0 };
            int book_number{ 0 };

            cout << "Введите номер читателя: ";
            if (!(cin >> reader_number)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ошибка ввода." << endl;
                break;
            }

            if (reader_number < 1 || reader_number > readers.size()) {
                cout << "Такого читателя нет." << endl;
                break;
            }

            auto& borrowed =
                readers[reader_number - 1]->get_borrowed_books();

            if (borrowed.empty()) {
                cout << "У читателя нет книг на руках." << endl;
                break;
            }

            cout << "Книги читателя:" << endl;

            for (size_t i = 0; i < borrowed.size(); i++) {
                cout << i + 1 << ". ";
                borrowed[i]->print();
            }

            cout << "Введите номер книги для возврата: ";
            if (!(cin >> book_number)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ошибка ввода." << endl;
                break;
            }

            if (book_number < 1 || book_number > borrowed.size()) {
                cout << "Неверный номер книги." << endl;
                break;
            }

            borrowed[book_number - 1]->set_is_issued(false);
            borrowed.erase(borrowed.begin() + book_number - 1);

            cout << "Книга успешно возвращена в библиотеку." << endl;
            break;
        }


              





        case 0:
            readers.clear();
            books.clear();
            cout << "Память очищена. Программа завершена." << endl;
            break;

        default:
            cout << "Такого пункта нет. Попробуйте еще раз." << endl;
        }

    } while (choice != 0);

    return 0;
}

