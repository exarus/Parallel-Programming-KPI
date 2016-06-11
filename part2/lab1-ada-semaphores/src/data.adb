--  Паралельне програмування - 2. Програмування для паралельних систем
--  Лабораторна робота 1. Ада.Семафори
--
--  MA = MB * MC + b * MO * MK - MU
--
--  Гунавардана Руслан ІП-31
--  25 лютого 2015 року н.э.
with Ada.Text_IO,
     Ada.Integer_Text_IO,
     Ada.Streams;
use Ada.Text_IO,
    Ada.Integer_Text_IO,
    Ada.Streams;

package body Data is

   procedure Vector_Input(A : out Vector) is
   begin
      for i in 1..N loop
         A(i) := 1;
      end loop;
   end Vector_Input;

   procedure Vector_Output(A : in Vector) is
   begin
      if (N < 5) then
         for i in 1..N loop
            Put(A(i));
         end loop;
      end if;
   end Vector_Output;

   procedure Matrix_Input (MA : out Matrix) is
   begin
      for i in 1..N loop
         for j in 1..N loop
           MA(i, j) := 1;
         end loop;
      end loop;
   end Matrix_Input;

   procedure Matrix_Output (MA : in Matrix) is
   begin
      if (N < 5) then
         for i in 1..N loop
            for j in 1..N loop
               Put(MA(i, j));
            end loop;
            New_Line;
         end loop;
      end if;
   end Matrix_Output;

   function Copy_Matrix(MA: in Matrix) return Matrix is
      MB : Matrix;
   begin
      for i in 1..N loop
         for j in 1..N loop
            MB(i, j) := MA(i, j);
         end loop;
      end loop;
      return MB;
   end Copy_Matrix;
end Data;
