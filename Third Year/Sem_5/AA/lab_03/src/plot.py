import pandas as pd
import matplotlib.pyplot as plt

class DataLoader:
    def __init__(self, filenames):
        self.data = {name: pd.read_csv(f"{name}.csv") for name in filenames}

    def getData(self, name):
        return self.data.get(name)

class Histogram:
    def __init__(self, dataLoader):
        self.dataLoader = dataLoader

    def plot_individual(self, name, title, outputFilename):
        data = self.dataLoader.getData(name)
        if data is not None:
            plt.figure(figsize=(10, 6))
            x_axis_label = 'index' if 'index' in data.columns else 'n'
            plt.bar(data[x_axis_label], data['comparisons'], label=name.replace("_", " ").title(), alpha=0.7)
            x_axis_label = 'Индекс элемента' if 'index' in data.columns else 'Упорядоченные количества сравнений'
            plt.xlabel(x_axis_label)
            plt.ylabel('Количество сравнений')
            plt.title(title)
            plt.legend()
            plt.grid(True)
            plt.savefig(outputFilename)
            plt.show()

if __name__ == '__main__':
    filenames = [
        ('binarysearch', "Поиск элемента алгоритмом бинарного поиска", 'binarysearch_histogram.png'),
        ('binarysearchcompstat', "Поиск элемента алгоритмом бинарного поиска (в порядке возрастания сравнений)", 'binarysearchcompstat_histogram.png'),
        ('bruteforce', "Поиск элемента алгоритмом полного перебора", 'bruteforce_histogram.png')
    ]

    dataLoader = DataLoader([file[0] for file in filenames])
    histogram = Histogram(dataLoader)

    for filename, title, outputFilename in filenames:
        histogram.plot_individual(filename, title, outputFilename)
