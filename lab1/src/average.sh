#!/bin/bash

# Проверяем, что есть аргументы
if [ $# -eq 0 ]; then
    echo "No arguments provided"
    echo "Usage: $0 number1 number2 ..."
    exit 1
fi

sum=0
count=$#

# Суммируем все аргументы
for num in "$@"; do
    # Проверяем, что аргумент - число
    if ! [[ "$num" =~ ^-?[0-9]+$ ]]; then
        echo "Error: '$num' is not a valid integer"
        exit 1
    fi
    sum=$((sum + num))
done

# Вычисляем среднее арифметическое без bc
average=$(( (sum * 100) / count ))  # Умножаем на 100 для двух знаков после запятой

# Форматируем вывод
integer_part=$((average / 100))
fractional_part=$((average % 100))

# Добавляем ведущий ноль для дробной части, если нужно
if [ $fractional_part -lt 10 ]; then
    fractional_part="0$fractional_part"
fi

# Выводим результаты
echo "Count: $count"
echo "Sum: $sum"
echo "Average: $integer_part.$fractional_part"