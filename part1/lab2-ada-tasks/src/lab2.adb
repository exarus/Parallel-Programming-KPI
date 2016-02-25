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
with Data, 
     Ada.Text_IO, 
     Ada.Numerics.Real_Arrays, 
     System.Multiprocessors;

use Ada.Text_IO, 
    Ada.Numerics.Real_Arrays, 
    System.Multiprocessors;

procedure Lab2 is
   type Fixed is delta 0.01 digits 10;
   Size: constant Integer := 4;
   package Configured_Data is new Data(Float);
   use Configured_Data;
   A, B, C, S : constant Real_Vector (1..Size) := (others => 1.0);
   MA, MD, MK, ML, MO, MT, MR, MW, MV : constant Real_Matrix (1..Size, 1..Size) := (others => (others => 1.0));
   
   CPU_1 :  CPU_Range := 0;
   CPU_2 :  CPU_Range := 1;
   CPU_3 :  CPU_Range := 2;

   task T1 is
      pragma Task_Name("T1");
      pragma Priority(15);
      pragma Storage_Size(500_000_000);
      pragma CPU (CPU_1);
   end T1;

   task T2 is
      pragma Task_Name("T2");
      pragma Priority(15);
      pragma Storage_Size(500_000_000);
      pragma CPU (CPU_2);
   end T2;

   task T3 is
      pragma Task_Name("T3");
      pragma Priority(15);
      pragma Storage_Size(500_000_000);
      pragma CPU (CPU_3);
   end T3;


   task body T1 is
        A, B, C: constant Real_Vector (1..Size) := (others => 1.0);
        MA, MD : constant Real_Matrix (1..Size, 1..Size) := (others => (others => 1.0));
   begin
      Put("Task1 started" & ASCII.LF);
      delay 1.0;
      Matrix_Output(Func1(A, B, C, MA, MD));
      Put("Task1 finished" & ASCII.LF);
   end T1;

   ----------------------------- end T1 --------------------------------------

   task body T2 is
      MK, ML, MO : constant Real_Matrix (1..Size, 1..Size) := (others => (others => 1.0));
   begin
      Put("Task2 started" & ASCII.LF);
      delay 2.0;
      Put(Fixed'Image (Fixed (Func2(MK, ML, MO))) & ASCII.LF);
      Put("Task2 finished" & ASCII.LF);
   end T2;

   ----------------------------- end T2 --------------------------------------

   task body T3 is
      S : constant Real_Vector (1..Size) := (others => 1.0);
      MT, MR, MW, MV : constant Real_Matrix (1..Size, 1..Size) := (others => (others => 1.0));
   begin
      Put("Task3 started" & ASCII.LF);
      delay 3.0;
      Put(Fixed'Image (Fixed (Func3(S, MT, MR, MW, MV))) & ASCII.LF);
      Put("Task3 finished" & ASCII.LF);
   end T3;

   ----------------------------- end T3 --------------------------------------
   
begin
    null;
end Lab2;
