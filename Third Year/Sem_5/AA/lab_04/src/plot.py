import pandas as pd
import matplotlib.pyplot as plt

def load_data(filename):
    return pd.read_csv(filename)

def plot_data(data, title, output_filename):
    plt.figure(figsize=(10, 6))
    plt.plot(data['threads'], data['time'], label='Зависимость времены работы от количества потоков', marker='o')
    plt.xlabel('Количество потоков')
    plt.ylabel('Время (в миллисекундах)')
    plt.title(title)
    plt.grid(True)
    plt.legend()
    plt.savefig(output_filename)
    plt.show()

if __name__ == '__main__':
    filename = 'parallelScrapingAll.csv'
    data = load_data(filename)
    plot_data(data, 'Зависимость времены работы от количества потоков', 'performanceScraping.png')
