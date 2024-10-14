# -*- coding: utf-8 -*-

import time

import matplotlib.pyplot as plt
import numpy


def gaussian(x, delay, sigma):
    '''
    Функция, график которой будет отображаться процессе анимации
    '''
    return numpy.exp(-((x - delay) / sigma) ** 2)


if __name__ == '__main__':
    # Параметры отображаемой функции
    maxSize = 200
    sigma = 10.0

    # Диапазон точек для расчета графика функции
    x = numpy.arange(maxSize)

    # Значения графика функции
    y = numpy.zeros(maxSize)

    # !!! Включить интерактивный режим для анимации
    plt.ion()

    # У функции gaussian будет меняться параметр delay (задержка)
    for delay in numpy.arange(-50.0, 200.0, 1.0):
        start = time.time()
        y = gaussian(x, delay, sigma)

        # !!! Очистить текущую фигуру
        plt.clf()

        # Отобразить график
        plt.plot(x, y)

        # Установка отображаемых интервалов по осям
        plt.xlim(0, maxSize)
        plt.ylim(-1.1, 1.1)

        # !!! Следующие два вызова требуются для обновления графика
        plt.draw()
        plt.gcf().canvas.flush_events()
        end = time.time()
        print('end - start: ', end - start)

        # Задержка перед следующим обновлением
        time.sleep(0.01)

    # Отключить интерактивный режим по завершению анимации
    plt.ioff()

    # Нужно, чтобы график не закрывался после завершения анимации
    plt.show()