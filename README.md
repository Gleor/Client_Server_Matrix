# Client Server Matrix
Проект клиент-серверного взаимодействия. Клиент отправляет данные на сервер (матрица 6х6), на котором происходят вычисления, после чего выводятся результаты.

## Сборка проекта

```
git clone git@github.com:Gleor/Client_Server_Matrix.git
cd ClientServerMatrix
mkdir build && cd build
cmake ..  
cmake --build .
```
## Работа с проектом

Для начала работы нужно запустить сервер и клиент: 

`./Server (нужно указать номер порта)` - пример: `./Server 5555`

`./Client (нужно указать адрес сервера и номер порта)` - пример: `./Client 127.0.0.1 5555`

Для отправки данных на сервер, в консоли клиента нужно ввести любую букву (и нажать Enter). После этого через 1 секунду будет отображена матрица, которая была отправлна на сервер, а в консоли сервера появятся данные в виде:

```
Det. = 5.0
Avg. det = 3.0
Del. det = 1.0
```
