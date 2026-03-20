import csv
import argparse
import matplotlib.pyplot as plt


def parse_arguments():
    parser = argparse.ArgumentParser()
    parser.add_argument("input_file", nargs="?", default="result_statistic.csv")
    parser.add_argument("output_file", nargs="?")
    return parser.parse_args()


def read_data(input_file):
    threads = []
    times = []

    with open(input_file, "r", encoding="utf-8") as f:
        reader = csv.DictReader(f)
        for row in reader:
            threads.append(int(row["Num threads"]))
            times.append(float(row["Time_Seconds"]))

    return threads, times


def plot_graph(threads, times, output_file):
    plt.figure(figsize=(8, 5))

    plt.plot(threads, times, marker="o", linewidth=2, color="#2E86AB")

    plt.xlabel("Количество потоков")
    plt.ylabel("Время выполнения, с")
    plt.title("Зависимость времени умножения матриц от числа потоков")
    plt.grid(True, alpha=0.3, linestyle="--")
    plt.xticks(sorted(set(threads)))
    plt.tight_layout()
    plt.savefig(output_file, dpi=150, bbox_inches="tight")
    plt.show()


if __name__ == "__main__":
    args = parse_arguments()
    threads, times = read_data(args.input_file)
    plot_graph(threads, times, args.output_file)
