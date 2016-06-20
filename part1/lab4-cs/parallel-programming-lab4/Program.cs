/*+===================================================================
 * Паралельне прогорамування - 1. Основи паралельного програмування
 * Лабораторна робота N 4.
 * ПОТОКИ  В ЯЗЫКЕ С#
 * F1 1.16: d = ((A + B)* (C *(MA*MD)))
 * F2 2.23: f = MAX(MK * ML - MO)
 * F3 3.28: v = MAX(MR*S) + MIN((MT*MW + MV))
 * Гунавардана Руслан ІП-31
 * 9 листопада 2015 року н.э.
===================================================================+*/

using MathNet.Numerics.LinearAlgebra.Double;
using MathNet.Numerics.Statistics;
using System;
using System.Threading;
using static System.Array;
using static System.Console;
using static System.Threading.Thread;

namespace ParallelProgramming.Lab4
{
    internal sealed class Program
    {
        private const int DefaultSize = 4;

        private static void Main(string[] args)
        {
            int size = args.Length > 0 ? int.Parse(args[0]) : DefaultSize;

            var A = Vector.Build.Dense(size, Vector.One);
            var B = Vector.Build.Dense(size, Vector.One);
            var C = Vector.Build.Dense(size, Vector.One);
            var S = Vector.Build.Dense(size, Vector.One);

            var MA = Matrix.Build.Dense(size, size, Matrix.One);
            var MD = Matrix.Build.Dense(size, size, Matrix.One);
            var MK = Matrix.Build.Dense(size, size, Matrix.One);
            var ML = Matrix.Build.Dense(size, size, Matrix.One);
            var MO = Matrix.Build.Dense(size, size, Matrix.One);
            var MT = Matrix.Build.Dense(size, size, Matrix.One);
            var MR = Matrix.Build.Dense(size, size, Matrix.One);
            var MW = Matrix.Build.Dense(size, size, Matrix.One);
            var MV = Matrix.Build.Dense(size, size, Matrix.One);

            var f1 = PrintingTask(() => (A + B) * (C * (MA * MD)));
            var f2 = PrintingTask(() => (MK * ML - MO).Enumerate().Maximum());
            var f3 = PrintingTask(() => (MR * S).Maximum() + (MT * MW + MV).Enumerate().Minimum());

            Thread[] threads =
            {
                new Thread(f1) { Name = "F1 Task", Priority = ThreadPriority.AboveNormal },
                new Thread(f2) { Name = "F2 Task", Priority = ThreadPriority.Highest     },
                new Thread(f3) { Name = "F3 Task", Priority = ThreadPriority.Lowest      },
            };
            ForEach(threads, t => t.Start());
            ForEach(threads, t => t.Join());
            WriteLine("Threads joined.");
        }

        private static int _taskNumber;
        private static readonly object TaskNumberLock = new object();

        private static ThreadStart PrintingTask(Func<object> task)
        {
            // current taskNumber copied for lambda
            int num;
            lock (TaskNumberLock) num = ++_taskNumber;
            return () =>
            {
                WriteLine("Task " + num + " started.");
                Sleep(num * 1000);
                WriteLine(task());
                WriteLine("Task " + num + " finished.");
            };
        }
    }
}

