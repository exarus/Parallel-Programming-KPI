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
with Ada.Text_IO, Ada.Integer_Text_IO;
use Ada.Text_IO, Ada.Integer_Text_IO;

package body Data is
   function Max(Matrix: in Real_Matrix) return Float is
      Max_Element: Float := Matrix (1, 1);
   begin
      for I in Matrix'Range (1) loop
         for J in Matrix'Range (2) loop
            if Matrix (I, J) > Max_Element then
               Max_Element := Matrix (I, J);
            end if;
         end loop;
      end loop;
      return Max_Element;
   end;

   function Max(Vector: in Real_Vector) return Float is
      Max_Element: Float := Vector (1);
   begin
      for I in Vector'Range (1) loop
         if Vector (I) > Max_Element then
            Max_Element := Vector (I);
         end if;
      end loop;
      return Max_Element;
   end;

   function Min(Matrix: in Real_Matrix) return Float is
      Min_Element: Float := Matrix (1, 1);
   begin
      for I in Matrix'Range (1) loop
         for J in Matrix'Range (2) loop
            if Matrix (I, J) < Min_Element then
               Min_Element := Matrix (I, J);
            end if;
         end loop;
      end loop;
      return Min_Element;
   end;

   function Min(Vector: in Real_Vector) return Float is
      Min_Element: Float := Vector (1);
   begin
      for I in Vector'Range (1) loop
         if Vector (I) < Min_Element then
            Min_Element := Vector (I);
         end if;
      end loop;
      return Min_Element;
   end;

   function Func1(A, B, C : Real_Vector; MA, MD: Real_Matrix) return Real_Matrix is
   begin
      return (A + B) * (C * (MA * MD));
   end;

   function Func2(MK, ML, MO: Real_Matrix) return Float is
   begin
      return Max(MK * ML - MO);
   end;

   function Func3(S: Real_Vector; MT, MR, MW, MV: Real_Matrix) return Float is
   begin
      return Max(MR * S) + Min(MT * MW + MV);
   end;

   procedure Vector_Output(Vector : in Real_Vector) is
      type Fixed is delta 0.01 digits 10;
   begin
      for I in Vector'Range (1) loop
         Put (Fixed'Image (Fixed (Vector (I))));
      end loop;
      New_Line;
   end;

   procedure Matrix_Output(Matrix: in Real_Matrix) is
      type Fixed is delta 0.01 digits 10;
   begin
      for I in Matrix'Range (1) loop
         for J in Matrix'Range (2) loop
            Put (Fixed'Image (Fixed (Matrix (I, J))));
         end loop;
         New_Line;
      end loop;
   end;
end Data;
