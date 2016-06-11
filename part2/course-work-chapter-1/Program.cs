/*+===================================================================
* Паралельне прогорамування - 2. Програмування для паралельних систем
* Лабораторна робота 3. C#. Семафори, мютекси, події, критичні секції
*
* A = sort(Z) * e - C * (MO * MK)
*
* Гунавардана Руслан ІП-31
* 7 червня 2015 року н.э.
===================================================================+*/

using System.Threading;

using static System.Array;
using static System.Console;

namespace course_work_chapter_1
{
    class Program
    {
        static void Main(string[] args)
        {
        }

        private static void T1()
        {
            int e1;
            int[] C1;
            int[,] MO1;

            A = new int[N];
            Input(out Z);
            E_T1_input.Set();

            Sort(Z, 0, H[1]);
            E_T2_sorted.WaitOne();
            int[] B = Merge(Z, 0, H[1], H[1], H[2]);
            for (int i = 0; i < B.Length; i++)
            {
                Z[i] = B[i];
            }
            E_T34_sorted.WaitOne();
            E_T56_sorted.WaitOne();
            int[] B2 = Merge(Z, 0, H[2], H[2], H[4], H[4], H[6]);
            E_all_sorted.Set();

            E_T2_input.WaitOne();
            Mut_e_copy.WaitOne();
            e1 = e;
            Mut_e_copy.ReleaseMutex();

            E_T4_input.WaitOne();
            S_C_copy.WaitOne();
            C1 = (int[])C.Clone();
            S_C_copy.Release();
            lock (MO)
            {
                MO1 = (int[,])MO.Clone();
            }

            E_all_sorted.WaitOne();
            PerformCalculation(0, H[1], e1, C1, MO1);
            Bar_finish.SignalAndWait();
            Output(A);
        }

        private static void T2()
        {
            int e2;
            int[] C2;
            int[,] MO2;

            e = 1;
            Input(out MK);
            E_T2_input.Set();

            E_T1_input.WaitOne();
            Sort(Z, H[1], H[2] - H[1]);
            E_T2_sorted.Set();

            Mut_e_copy.WaitOne();
            e2 = e;
            Mut_e_copy.ReleaseMutex();

            E_T4_input.WaitOne();
            S_C_copy.WaitOne();
            C2 = (int[])C.Clone();
            S_C_copy.Release();
            lock (MO)
            {
                MO2 = (int[,])MO.Clone();
            }

            E_all_sorted.WaitOne();
            PerformCalculation(H[1], H[2], e2, C2, MO2);
            Bar_finish.RemoveParticipant();
        }

        private static void T3()
        {
            int e3;
            int[] C3;
            int[,] MO3;

            E_T1_input.WaitOne();
            Sort(Z, H[2], H[3] - H[2]);
            E_T4_sorted.WaitOne();
            int[] B = Merge(Z, H[2], H[3], H[3], H[4]);
            for (int i = 0; i < B.Length; i++)
            {
                Z[i + H[2]] = B[i];
            }
            E_T34_sorted.Set();

            E_T2_input.WaitOne();
            Mut_e_copy.WaitOne();
            e3 = e;
            Mut_e_copy.ReleaseMutex();

            E_T4_input.WaitOne();
            S_C_copy.WaitOne();
            C3 = (int[])C.Clone();
            S_C_copy.Release();
            lock (MO)
            {
                MO3 = (int[,])MO.Clone();
            }

            E_all_sorted.WaitOne();
            PerformCalculation(H[2], H[3], e3, C3, MO3);
            Bar_finish.RemoveParticipant();
        }

        private static void T4()
        {
            int e4;
            int[] C4;
            int[,] MO4;

            Input(out C);
            Input(out MO);
            E_T4_input.Set();

            E_T1_input.WaitOne();
            Sort(Z, H[3], H[4] - H[3]);
            E_T4_sorted.Set();

            S_C_copy.WaitOne();
            C4 = (int[])C.Clone();
            S_C_copy.Release();
            lock (MO)
            {
                MO4 = (int[,])MO.Clone();
            }

            E_T2_input.WaitOne();
            Mut_e_copy.WaitOne();
            e4 = e;
            Mut_e_copy.ReleaseMutex();

            E_all_sorted.WaitOne();
            PerformCalculation(H[3], H[4], e4, C4, MO4);
            Bar_finish.RemoveParticipant();
        }

        private static void T5()
        {
            int e5;
            int[] C5;
            int[,] MO5;

            E_T1_input.WaitOne();
            Sort(Z, H[4], H[5] - H[4]);
            E_T6_sorted.WaitOne();
            int[] B = Merge(Z, H[4], H[5], H[5], H[6]);
            for (int i = 0; i < B.Length; i++)
            {
                Z[i + H[4]] = B[i];
            }
            E_T56_sorted.Set();

            E_T2_input.WaitOne();
            Mut_e_copy.WaitOne();
            e5 = e;
            Mut_e_copy.ReleaseMutex();

            E_T4_input.WaitOne();
            S_C_copy.WaitOne();
            C5 = (int[])C.Clone();
            S_C_copy.Release();
            lock (MO)
            {
                MO5 = (int[,])MO.Clone();
            }

            E_all_sorted.WaitOne();
            PerformCalculation(H[4], H[5], e5, C5, MO5);
            Bar_finish.RemoveParticipant();
        }

        private static void T6()
        {
            int e6;
            int[] C6;
            int[,] MO6;

            E_T1_input.WaitOne();
            Sort(Z, H[5], H[6] - H[5]);
            E_T6_sorted.Set();

            E_T2_input.WaitOne();
            Mut_e_copy.WaitOne();
            e6 = e;
            Mut_e_copy.ReleaseMutex();

            E_T4_input.WaitOne();
            S_C_copy.WaitOne();
            C6 = (int[])C.Clone();
            S_C_copy.Release();
            lock (MO)
            {
                MO6 = (int[,])MO.Clone();
            }

            E_all_sorted.WaitOne();
            PerformCalculation(H[5], H[6], e6, C6, MO6);
            Bar_finish.RemoveParticipant();
        }

        private static void PerformCalculation(int from, int to, int e_sh, int[] C_sh, int[,] MO_sh)
        {
            for (int i = from; i < to; i++)
            {
                A[i] = Z[i] * e_sh;
                for (int j = 0; j < N; j++)
                {
                    int cell_ji = 0;
                    for (int k = 0; k < N; k++)
                    {
                        cell_ji = cell_ji + MO_sh[j, k] * MK[k, i];
                    }
                    A[i] += C_sh[j] * cell_ji;
                }
            }
        }

        private static void Input(out int[,] m)
        {
            m = new int[N, N];
            for (int i = 0; i < N; i++)
                for (int j = 0; j < N; j++)
                    m[i, j] = 1;
        }

        private static void Input(out int[] v)
        {
            v = new int[N];
            for (int i = 0; i < N; i++) v[i] = 1;
        }

        private static void Output(int[] vector)
        {
            if (N >= 8) return;
            for (int i = 0; i < N; i++)
            {
                Write(vector[i] + " ");
            }
            WriteLine();
        }
    }
}
