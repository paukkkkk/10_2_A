#ifndef NOMINMAX
#define NOMINMAX                    // вимикає макроси min/max з windows.h (вони ламають numeric_limits<...>::max())
#endif
#include <iostream>                 
#include <iomanip>                  
#include <string>                   
#include <windows.h>                
#include <limits>                   
#include <cmath>

using namespace std;                

enum Speciality { Komp_Nauky, Informatyka, Matem_Econom, Phys_Inform, Trud };   

struct Student                      
{
    string prizv;                   
    int kurs;                       
    Speciality speciality;          
    int fizyka;                     
    int matematyka;                 
    int informatyka;               
};

int InputInt(const string& promt, int minValue, int maxValue);                      // ввід цілого з перевіркою діапазону
double InputDouble(const string& promt, double minValue, double maxValue);          // ввід дійсного з перевіркою діапазону
void Create(Student* p, const int N, const string specialityStr[]);                 // заповнення масиву з клавіатури
void Print(const Student* p, const int N, const string specialityStr[]);            // вивід таблиці
int Sum(const Student& s);                                                          // сума трьох оцінок
double Average(const Student& s);                                                   // середній бал
void Sort(Student* p, const int N);                                                 // ФІЗИЧНЕ впорядкування
int* IndexSort(const Student* p, const int N);                                      // побудова ІНДЕКСНОГО масиву
void PrintIndexSorted(const Student* p, const int* I, const int N, const string specialityStr[]); // вивід за індексним масивом
int BinSearch(const Student* p, const int N, const string& prizv, const int kurs, const double avg); // бінарний пошук

int main()
{
    SetConsoleOutputCP(65001);      
    SetConsoleCP(65001);            

    const string specialityStr[] = {    
        "Комп'ютерні науки",
        "Інформатика",
        "Математика та економіка",
        "Фізика та інформатика",
        "Трудове навчання"
    };

    int N = InputInt("Введіть кількість студентів N: ", 1, 1000);   

    Student* p = new Student[N];    

    bool created = false;           // чи вже введені дані (щоб не працювати зі сміттям)
    bool sorted = false;            // чи масив уже фізично впорядкований (потрібно для бінарного пошуку)
    int* I = nullptr;               // вказівник на індексний масив
    string prizv;                   // ключ пошуку: прізвище
    int kurs;                       // ключ пошуку: курс
    double avg;                     // ключ пошуку: середній бал
    int found;                      // результат пошуку (індекс або -1)

    int menuItem;                   // вибраний пункт меню
    do {
        cout << endl << endl << endl;
        cout << "Виберіть дію:" << endl << endl;
        cout << " [1] - введення даних з клавіатури" << endl;
        cout << " [2] - вивід даних на екран" << endl;
        cout << " [3] - фізичне впорядкування даних" << endl;
        cout << " [4] - бінарний пошук студента за прізвищем, курсом та середнім балом" << endl;
        cout << " [5] - індексне впорядкування та вивід даних" << endl << endl;
        cout << " [0] - вихід та завершення роботи програми" << endl << endl;
        menuItem = InputInt("Введіть значення: ", 0, 5);    // ввід пункту меню з контролем помилок
        cout << endl << endl;

        switch (menuItem)
        {
        case 1:
            Create(p, N, specialityStr);                    // ввід даних
            created = true;                                 // дані з'явилися
            sorted = false;                                 // порядок порушений – пошук знову недоступний
            break;
        case 2:
            if (created)
                Print(p, N, specialityStr);
            else
                cout << "Спочатку введіть дані (пункт 1)!" << endl;
            break;
        case 3:
            if (created)
            {
                Sort(p, N);                                 // фізичне впорядкування
                sorted = true;
                cout << "Масив фізично впорядковано." << endl;
            }
            else
                cout << "Спочатку введіть дані (пункт 1)!" << endl;
            break;
        case 4:
            if (!sorted)
            {
                cout << "Спочатку фізично впорядкуйте масив (пункт 3)!" << endl;
                break;
            }
            cout << "Введіть ключі пошуку:" << endl;
            cout << "     прізвище: ";
            getline(cin, prizv);                            // прізвище з пробілами
            kurs = InputInt("     курс (1-6): ", 1, 6);
            avg = InputDouble("     середній бал (1-5, з крапкою): ", 1.0, 5.0);
            cout << endl;
            found = BinSearch(p, N, prizv, kurs, avg);
            if (found != -1)
                cout << "Знайдено студента в позиції " << found + 1 << endl;
            else
                cout << "Шуканого студента не знайдено" << endl;
            break;
        case 5:
            if (created)
            {
                I = IndexSort(p, N);                        // будуємо індексний масив
                PrintIndexSorted(p, I, N, specialityStr);   // виводимо в порядку індексів
                delete[] I;                                 // звільняємо індексний масив
                I = nullptr;
            }
            else
                cout << "Спочатку введіть дані (пункт 1)!" << endl;
            break;
        case 0:
            break;                                          // вихід
        }
    } while (menuItem != 0);

    delete[] p;                     // звільняємо динамічний масив
    return 0;
}

int InputInt(const string& promt, int minValue, int maxValue)
{
    int x;                                                  // змінна для числа
    while (true)                                            // повторюємо, поки ввід неправильний
    {
        cout << promt;                                      // виводимо запрошення
        if (cin >> x && x >= minValue && x <= maxValue)     // число прочитане і в діапазоні
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');   // викидаємо решту рядка (щоб не заважала getline)
            return x;
        }
        cout << "Помилка введіть число від " << minValue << " до " << maxValue << endl;
        cin.clear();                                        // скидаємо стан помилки потоку
        cin.ignore(numeric_limits<streamsize>::max(), '\n');   // викидаємо неправильний ввід
    }
}

double InputDouble(const string& promt, double minValue, double maxValue)
{
    double x;                                               // змінна для дійсного числа
    while (true)
    {
        cout << promt;
        if (cin >> x && x >= minValue && x <= maxValue)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return x;
        }
        cout << "Помилка введіть число від " << minValue << " до " << maxValue << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void Create(Student* p, const int N, const string specialityStr[])
{
    for (int i = 0; i < N; i++)                             // для кожного студента
    {
        cout << "Студент N " << i + 1 << ":" << endl;

        cout << "Прізвище: ";
        getline(cin, p[i].prizv);                           // прізвище одним рядком
        p[i].kurs = InputInt(" Курс (1-6): ", 1, 6);

        cout << "Спеціальність: " << endl;
        for (int k = 0; k < 5; k++)                         // меню спеціальностей
            cout << "    " << k << " - " << specialityStr[k] << endl;
        p[i].speciality = (Speciality)InputInt(" Ваш вибір: ", 0, 4);   // число -> елемент переліку

        p[i].fizyka = InputInt("     Фізика (1-5): ", 1, 5);
        p[i].matematyka = InputInt("     Математика (1-5): ", 1, 5);
        p[i].informatyka = InputInt("     Інформатика (1-5): ", 1, 5);
        cout << endl;
    }
}

void Print(const Student* p, const int N, const string specialityStr[])
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    const string line(91, '=');                             // верхня/нижня лінія таблиці
    const string dash(91, '-');                             // лінія під шапкою

    cout << "Таблиця успішності студентів:" << endl;
    cout << line << endl;
    cout << "| № |    Прізвище    | Курс |      Спеціальність      | Фізика | Математика | Інформатика |" << endl;
    cout << dash << endl;

    for (int i = 0; i < N; i++)
    {
        int lenP = 0;                                       // довжина прізвища в СИМВОЛАХ
        for (unsigned char c : p[i].prizv)
            if ((c & 0xC0) != 0x80) lenP++;                 // байти-продовження UTF-8 (10xxxxxx) не рахуємо

        int lenS = 0;                                       // довжина назви спеціальності в символах
        for (unsigned char c : specialityStr[p[i].speciality])
            if ((c & 0xC0) != 0x80) lenS++;

        cout << "| " << setw(1) << right << i + 1
            << " | " << p[i].prizv << string(lenP < 14 ? 14 - lenP : 0, ' ')           // добиваємо пробілами до 14  виводимо прізвище і добиваємо пробілами до ширини 14 символів
            << " | " << setw(4) << right << p[i].kurs
            << " | " << specialityStr[p[i].speciality] << string(lenS < 23 ? 23 - lenS : 0, ' ')
            << " | " << setw(6) << right << p[i].fizyka
            << " | " << setw(10) << right << p[i].matematyka
            << " | " << setw(11) << right << p[i].informatyka
            << " |" << endl;
    }
    cout << line << endl << endl;
}

int Sum(const Student& s)
{
    return s.fizyka + s.matematyka + s.informatyka;         
}

double Average(const Student& s)
{
    return Sum(s) / 3.0;                                    
}

void Sort(Student* p, const int N)
{   // фізичне впорядкування методом "бульбашки":
    // 1) за середнім балом за зростанням; 
    // 2) для однакових – за прізвищем за СПАДАННЯМ; 
    // 3) для однакових – за курсом
    // (середні бали порівнюємо через суми – вони цілі, тому порівняння точне)
    Student tmp;                                            // тимчасова змінна для обміну
    for (int i0 = 0; i0 < N - 1; i0++)                      // N-1 проходів
        for (int i1 = 0; i1 < N - i0 - 1; i1++)             // порівнюємо сусідів
            if ((Sum(p[i1]) > Sum(p[i1 + 1]))               // 1-й ключ: більший бал стоїть раніше – міняємо
                ||
                (Sum(p[i1]) == Sum(p[i1 + 1]) &&            // 2-й ключ: бали рівні,
                    p[i1].prizv < p[i1 + 1].prizv)             // прізвище менше – за спаданням має стояти пізніше
                ||
                (Sum(p[i1]) == Sum(p[i1 + 1]) &&            // 3-й ключ: бали та прізвища рівні,
                    p[i1].prizv == p[i1 + 1].prizv &&
                    p[i1].kurs > p[i1 + 1].kurs))              // більший курс стоїть раніше – міняємо
            {
                tmp = p[i1];                                // обмін елементів масиву
                p[i1] = p[i1 + 1];
                p[i1 + 1] = tmp;
            }
}

int* IndexSort(const Student* p, const int N)
{   // індексне впорядкування методом вставки. Масив p не змінюється – впорядковується масив номерів I.
    // Порядок: середній бал за зростанням, потім прізвище за зростанням, потім курс
    int* I = new int[N];                                    // створили індексний масив
    for (int k = 0; k < N; k++)
        I[k] = k;                                           // початково I[k] = k

    int i, j, value;
    for (i = 1; i < N; i++)
    {
        value = I[i];                                       // індекс, який вставляємо
        for (j = i - 1;
            j >= 0 && ((Sum(p[I[j]]) > Sum(p[value])) ||   // елемент I[j] має стояти ПІСЛЯ value – зсуваємо його вправо
                (Sum(p[I[j]]) == Sum(p[value]) &&
                    p[I[j]].prizv > p[value].prizv) ||
                (Sum(p[I[j]]) == Sum(p[value]) &&
                    p[I[j]].prizv == p[value].prizv &&
                    p[I[j]].kurs > p[value].kurs));
            j--)
        {
            I[j + 1] = I[j];
        }
        I[j + 1] = value;                                   // вставляємо індекс на своє місце
    }
    return I;                                               // повертаємо вказівник (потім його треба delete[])
}

void PrintIndexSorted(const Student* p, const int* I, const int N, const string specialityStr[])
{   // те саме, що Print(), але замість p[i] використовуємо p[I[i]]
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    const string line(91, '=');
    const string dash(91, '-');

    cout << "Таблиця успішності студентів (індексне впорядкування):" << endl;
    cout << line << endl;
    cout << "| № |    Прізвище    | Курс |      Спеціальність      | Фізика | Математика | Інформатика |" << endl;
    cout << dash << endl;

    for (int i = 0; i < N; i++)
    {
        int lenP = 0;
        for (unsigned char c : p[I[i]].prizv)
            if ((c & 0xC0) != 0x80) lenP++;

        int lenS = 0;
        for (unsigned char c : specialityStr[p[I[i]].speciality])
            if ((c & 0xC0) != 0x80) lenS++;

        cout << "| " << setw(1) << right << i + 1
            << " | " << p[I[i]].prizv << string(lenP < 14 ? 14 - lenP : 0, ' ')
            << " | " << setw(4) << right << p[I[i]].kurs
            << " | " << specialityStr[p[I[i]].speciality] << string(lenS < 23 ? 23 - lenS : 0, ' ')
            << " | " << setw(6) << right << p[I[i]].fizyka
            << " | " << setw(10) << right << p[I[i]].matematyka
            << " | " << setw(11) << right << p[I[i]].informatyka
            << " |" << endl;
    }
    cout << line << endl << endl;
}

int BinSearch(const Student* p, const int N, const string& prizv, const int kurs, const double avg)
{   // повертає індекс знайденого елемента або -1. Масив МАЄ бути впорядкований функцією Sort()
    const double eps = 0.005;                               // допуск: користувач вводить бал із 2 знаками
    int L = 0, R = N - 1, m;                                // ліва, права межі та середина
    do {
        m = (L + R) / 2;                                    // середній елемент
        double am = Average(p[m]);                          // його середній бал
        bool sameAvg = fabs(am - avg) < eps;                // чи збігаються середні бали
        if (sameAvg && p[m].prizv == prizv && p[m].kurs == kurs)
            return m;                                       // знайшли за всіма трьома ключами
        if ((!sameAvg && am < avg) ||                       // елемент m стоїть ДО шуканого у порядку Sort():
            (sameAvg && p[m].prizv > prizv) ||              //   менший бал, або (бал той самий і прізвище більше – бо спадання),
            (sameAvg && p[m].prizv == prizv && p[m].kurs < kurs))   // або (бал і прізвище ті самі, курс менший)
        {
            L = m + 1;                                      // шукаємо праворуч
        }
        else
        {
            R = m - 1;                                      // шукаємо ліворуч
        }
    } while (L <= R);
    return -1;                                              // не знайшли
}