# messenger_cpp

C++17 minimum

Windows only

# Тип User
содержит данные пользователя:
1)логин

2)пароль

3)имя пользователя


Имеент контрукторы

сеттеры и геттеры для взаимодействия с объектом этого класса

# Тип Message

Cодержит информацию об отправленном сообщении

1)отправитель

2)получатель

3)содержание письма

4)время отправки письма


Имеент контрукторы

сеттеры и геттеры для взаимодействия с объектом этого класса

# Тип DataBase

Наследован открыто от класса Singleton, поэтому существует в единственном экземпляре

Является "backend" частью чата, данный класс не поддерживает ввод c клавитуры и ничего не выводит на экран. Он работает только с файлами json

и возвращает результаты обращений к "базе данных"

Содержит все для работы с файлами "базы данных"

1)поток для "базы данных" пользователей

2)поток для "базы данных" сообщений

3)путь к "базе данных"

4)позиция в файле текущего авторизированного пользователя

Конструктор создает "базу данных" если она не была создана ранее:

1)json file users

2)отдельную папку где будут храниться сообщения


Функции-члены to_json для отправки переданных данных о пользователе или сообщении в файл "базы данных"

Функции-члены from_json для извлечения данных о пользователе или сообщении в файл "базы данных"

Функция-член login выполняет авторизацию пользователя на основе переданных в функцию данных, если такой пользователь существует и введен правильный логин и 
пароль то функция возвращает true, иначе false

Функция-член SingUp выполняет регистрацию пользователя. Если пользователь с данным логином или именем существует, то функция вернет false, иначе функция передает данные нового пользователя функции to_json, которая записывает их в "базу данных" и возвращает true

Функция-член isExisting проверяет существование пользователя в "базе данных"

Функция-член clear_chat очищает чат у двух пользователей одновременно.

Функция-член changeLogin меняет логин пользователя

Функция-член changePassword меняет пароль пользователя

Функция-член getMessages извлекает переписку определенного чата и возвращает массив сообщений

Функция-член sendMessage выполняет отправку сообщения указанному пользователю или в указанный чат

Функция-член adjustment выполняет выравнивание байт при записи в файл, очищая лишние данные


# Тип Сhat

Наследован открыто от класса Singleton, поэтому существует в единственном экземпляре

Является "frontend" частью чата, данный класс делает запросы у пользователя на ввод с клавиатуры некоторых данных. После сбора информации он направляет все

нужные данные классу DataBase. Так же данный тип выполняет проверку на корректность вводимых данных.

Содержит

1)указталеь на класс DataBase

2)указатель на текущего пользователя

3)указатель на функцию котороя будет выполняться

4)статус авторизации пользователя


Конструктор по умолчанию

Деструктор: закрывает файл


Функции-члены:

1)chatMenu выводит в консоль меню 

2)userMenu выводит в консоль меню пользователя

3)chatBox делает отрисовку бокса для чата

4)login запрашивает у пользователя ввод данных с клавитуры для авторизации

5)signUp запрашивает у пользователя ввод данных с клавитуры для регистрации

6)chatLoop цикл с запросами выбора, пока не будет выхода из учетной записи

7)userLoop цикл с запросами выбора, пока не будет выхода из программы

8)action_for_chat запрос на выбор функций чата

9)action_for_user запрос на выбор функций пользователя

10)getMessages запрашивает у пользователя ввод имени пользователя/чата для получения диалога

11)sendMessage запрашивает у пользователя ввод имени пользователя/чата и сообщения для отправки сообщения

12)clear_chat запрашивает у пользователя ввод имени пользователя для очистки диалога с ним(общий чат нельзя очищать)

13)get_info выводит информация о пользователе(логин, имя, пароль)

14)changePassword запрашивает у пользователя ввод нового пароля

15)changeLogin запрашивает у пользователя ввод нового логина

16)set_user_data данная функция проверяет вводимые данные на корректность

17)set_current_user устанавливает нового текущего пользователя для работы с ним

18)logout прекращает цикл пользователя и очищает указатель на текущего пользователя

19)exit выбрасывает исключение CloseChat для завершения программы

20)flush_input_buffer очищает поток ввода от мусора

21)time_to_string переводит тип time_t в string

22)set_data_base устанавливает "базу данных" в которую будет передавать данные 


# Тип Singleton

Реализует паттерн singleton CRTP
