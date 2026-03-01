import csv
import argparse
import matplotlib.pyplot as plt
import numpy as np


def parse_arguments():
    parser = argparse.ArgumentParser()
    parser.add_argument("input_file", nargs="?", help="Путь к файлу для обработки")
    parser.add_argument("output_file", nargs="?", help="Файл для записи результатов")
    return parser.parse_args()


def read_file(input_file):
    data = {"Size": [], "Time_Seconds": [], "Operations": []}

    try:
        with open(input_file, "r") as file:
            next(file)

            for row_num, line in enumerate(file, 1):
                row = line.strip().split(",")
                try:
                    data["Size"].append(float(row[0]))
                    data["Time_Seconds"].append(float(row[1]))
                    data["Operations"].append(float(row[2]))
                except ValueError:
                    print(f"Ошибка в строке {row_num}: неверный формат чисел")
        return data

    except FileNotFoundError:
        print(f"Файл {input_file} не найден")
        return None


def plot_graphs(data):
    plt.figure(figsize=(14, 10))

    # 1. График зависимости времени от размера
    plt.subplot(1, 2, 1)
    plt.plot(data["Size"], data["Time_Seconds"])
    plt.xlabel("Размер матрицы")
    plt.ylabel("Время (секунды)")
    plt.title("Зависимость времени от размера")
    plt.grid(True, alpha=0.3)

    # 2. Количество операций
    plt.subplot(1, 2, 2)
    ops_millions = [o / 1e6 for o in data["Operations"]]
    plt.plot([str(int(s)) for s in data["Size"]], ops_millions)
    plt.xlabel("Размер матрицы")
    plt.ylabel("Операции (миллионы)")
    plt.title("Количество операций")
    plt.grid(True, alpha=0.3, axis="y")
    plt.suptitle("Анализ умножения матриц", fontsize=16)
    plt.savefig(args.output_file, dpi=150, bbox_inches="tight")
    plt.show()


if __name__ == "__main__":
    args = parse_arguments()
    data = read_file(args.input_file)
    plot_graphs(data)
