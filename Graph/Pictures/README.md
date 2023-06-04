# Задача комивояжера
## Постановка задачи
Реализовать взвешенный смешанный граф на основе матрицы смежности и решить задачу коммивояжёра методом ветвей и границ.
- Создать класс ```Graph_logics``` и реализовать методы по редактированию матрицы смежности.
- Создать класс ```GraphComponent``` и реализовать методы по визуализации и обработке ввода.
- Реализовать структуру и методы, для решения задачи коммивояжера.
- Отрисовать граф, при помощи фреймворка ```JUCE```.
- Продемонстрировать решение задачи.

## Анализ задачи
- Класс ```Graph_logics``` состоит из матрицы смежности, из которой считываются данные и редактируются по необходимости.
- ```Graph_logics``` содержит методы редактирования графа, а также метод для получения матрицы смежности.
- Класс отрисовки и обработки ввода - ```GraphComponent```, основан на ряде функций по настройке окна, отрисовке графа, обработки ввода. Все функции реализованы с помощью флагов, нажатия меняют их, чтобы активировать различные режимы.
- При решении задачи Коммивояжера путь строится по умолчанию из вершины под номером один и подсвечивается градиентом. 

## Тестирование
Тестирование программы представленно в видео на YouTube: [https://youtu.be/MOZ47426eUI](https://youtu.be/MOZ47426eUI)

## Сборка и запуск
Для того, чтобы запустить данную программу необходимо скачать zip архив и разархивировать папку ```Graph``` на накопитель компьютера. 
Далее открыть файл ```Graph\Builds\VisualStudio2022\Project1.sln``` и скомпилировать программу. Для корректного запуска необходима версия ```JUCE v7.0.5``` или более поздняя.

## Работа программы
<img src="./1.png">

## Визуализация решения задачи коммивояжёра
<img src="./2.png">

## Исходный код
Исходный код представлен в репозитории: https://github.com/Karrton/Creative-tasks/tree/main/Graph/Source