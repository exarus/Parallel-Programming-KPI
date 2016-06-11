--  Паралельне програмування - 2. Програмування для паралельних систем
--  Лабораторна робота 1. Ада.Семафори
--
--  MA = MB * MC + b * MO * MK - MU
--
--  Гунавардана Руслан ІП-31
--  25 лютого 2015 року н.э.

generic
   N : in Integer;
package Data is
   type Vector is array(1..N) of Integer;
   type Matrix is array(1..N, 1..N) of Integer;

   procedure Vector_Input(A : out Vector);
   procedure Vector_Output(A : in Vector);
   procedure Matrix_Input(MA : out Matrix);
   procedure Matrix_Output(MA : in Matrix);
   function Copy_Matrix(MA : in Matrix) return Matrix;
end Data;
