--  Паралельне прогорамування - 1. Основи паралельного програмування
--  Лабораторна робота N 1.
--  АДА.ПІДПРОГРАМИ ТА ПАКЕТИ
--
--  F1 1.16: d = ((A + B)* (C *(MA*MD)))
--  F2 2.23: f = MAX(MK * ML - MO)
--  F3 3.28: V = MAX(MR*S) + MIN((MT*MW + MV))
--
--  Гунавардана Руслан ІП-31
--  6 вересня 2015 року н.э.
with Data, Ada.Text_IO, Ada.Numerics.Real_Arrays;
use Ada.Text_IO, Ada.Numerics.Real_Arrays;

procedure Lab1 is
   type Fixed is delta 0.01 digits 10;
   Size: constant Integer := 3;
   package Configured_Data is new Data(Float);
   use Configured_Data;
   A, B, C, S : constant Real_Vector (1..Size) := (others => 1.0);
   MA, MD, MK, ML, MO, MT, MR, MW, MV : constant Real_Matrix (1..Size, 1..Size) := (others => (others => 1.0));
begin
   Matrix_Output(Func1(A, B, C, MA, MD));
   Put(Fixed'Image (Fixed (Func2(MK, ML, MO))) & ASCII.LF);
   Put(Fixed'Image (Fixed (Func3(S, MT, MR, MW, MV))) & ASCII.LF);
end Lab1;
