"""
Паралельне прогорамування - 1. Основи паралельного програмування
Лабораторна робота N 8.
ПОТОКИ В ЯЗЫКЕ Python
F1 1.16: d = ((A + B)* (C *(MA*MD)))
F2 2.23: f = MAX(MK * ML - MO)
F3 3.28: v = MAX(MR*S) + MIN((MT*MW + MV))
Гунавардана Руслан ІП-31
17 листопада 2015 року н.э.
"""

import sys
import time
from multiprocessing import Process
from threading import Thread

from numpy import max, min
from numpy.matlib import ones, inner


def execute_and_print(num, function):
    time.sleep(num)
    print('Task ' + str(num) + ' started.')
    print(function())
    print('Task ' + str(num) + ' finished.')


def f1(A, B, C, MA, MD):
    execute_and_print(1, lambda: inner(A + B, C * (MA * MD)))


def f2(MK, ML, MO):
    execute_and_print(2, lambda: max(MK * ML - MO))


def f3(MR, S, MT, MW, MV):
    execute_and_print(3, lambda: max(MR * S) + min(MT * MW + MV))


if __name__ == '__main__':
    n = int(sys.argv[1]) if len(sys.argv) > 1 else 4
    A, B, C = (ones(n),) * 3
    S = ones((n, 1))
    MA, MD, MK, ML, MO, MT, MR, MW, MV = (ones((n, n)),) * 9

    tasks = (
        Thread (name='F1', target=f1, args=(A, B, C, MA, MD)),
        Process(name='F2', target=f2, args=(MK, ML, MO)),
        Process(name='F3', target=f3, args=(MR, S, MT, MW, MV)),
    )
    for t in tasks:
        t.start()
    for t in tasks:
        t.join()
    print('Processes joined.')
