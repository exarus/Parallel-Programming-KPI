--  Паралельне прогорамування - 1. Основи паралельного програмування
--  Лабораторна робота N 2.
--  ПРОЦЕССЫ В ЯЗЫКЕ АДА.ЗАДАЧИ
--
--  F1 1.16: d = ((A + B)* (C *(MA*MD)))
--  F2 2.23: f = MAX(MK * ML - MO)
--  F3 3.28: V = MAX(MR*S) + MIN((MT*MW + MV))
--
--  Гунавардана Руслан ІП-31
--  6 вересня 2015 року н.э.
with Ada.Numerics.Real_Arrays;
use Ada.Numerics.Real_Arrays;

generic
   type T is private;
package Data is
   function Func1(A,B,C : Real_Vector; MA, MD: Real_Matrix) return Real_Matrix;
   function Func2(MK, ML, MO: Real_Matrix) return Float;
   function Func3(S: Real_Vector; MT, MR, MW, MV: Real_Matrix) return Float;
   procedure Vector_Output(Vector : in Real_Vector);
   procedure Matrix_Output(Matrix: in Real_Matrix);
end Data;
