--  Паралельне програмування - 2. Програмування для паралельних систем
--  Лабораторна робота 1. Ада.Семафори
--
--  MA = MB * MC + b * MO * MK - MU
--
--  Гунавардана Руслан ІП-31
--  25 лютого 2015 року н.э.
with Data,
     Ada.Text_IO,
     Ada.Integer_Text_IO,
     Ada.Synchronous_Task_Control;
use Ada.Text_IO,
    Ada.Integer_Text_IO,
    Ada.Synchronous_Task_Control;

procedure Lab1 is
   n: Integer := 4;   -- vector size
   P: Integer := 2;   -- CPU count
   H: Integer := n/P; -- Subvector size

   subtype FirstHRows is Integer range 1..H;
   subtype LastHRows is Integer range H+1..N;
   package Lab1Data is new Data(n);
   use Lab1Data;

   b : Integer;
   MA, MB, MC, MO, MK, MU : Matrix;

   -- critical section semaphores
   MBCriticalSemaphore : Suspension_Object;
   bCriticalSemaphore  : Suspension_Object;
   MOCriticalSemaphore : Suspension_Object;

   -- application signal semaphores
   InputSemaphore, T2EndSemaphore : Suspension_Object;

   procedure Start_Tasks is
      ------------T1------------
      task T1;
      task body T1 is
         b1 : Integer;
         MB1, MO1 : Matrix;
      begin
         Put_Line("Process T1 started");
         b := 1;
         Matrix_Input(MO);
         Matrix_Input(MK);
         Matrix_Input(MB);
         Matrix_Input(MC);
         Matrix_Input(MU);
         Set_True(InputSemaphore);

         Suspend_Until_True(MBCriticalSemaphore);
         MB1 := Copy_Matrix(MB);
         Set_True(MBCriticalSemaphore);

         Suspend_Until_True(bCriticalSemaphore);
         b1 := b;
         Set_True(bCriticalSemaphore);

         Suspend_Until_True(MOCriticalSemaphore);
         MO1 := Copy_Matrix(MO);
         Set_True(MOCriticalSemaphore);

         for i in FirstHRows loop
            for j in 1..n loop
               MA(i, j) := 0;
               for k in 1..n loop
                  MA(i, j) :=  MA(i, j) + MB1(i, k) * MC(k, j)
                               + b1 * MO1(i, k) * MK(k, j);
               end loop;
               MA(i, j) := MA(i, j) - MU(i, j);
            end loop;
         end loop;

         Suspend_Until_True(T2EndSemaphore);
         Matrix_Output(MA);
         Put_Line("Process T1 finished");
      end T1;


      ------------T2------------
      task T2;
      task body T2 is
         b2 : Integer;
         MB2, MO2 : Matrix;
      begin
         Put_Line("Process T2 started");
         Suspend_Until_True(InputSemaphore);

         Suspend_Until_True(MBCriticalSemaphore);
         MB2 := Copy_Matrix(MB);
         Set_True(MBCriticalSemaphore);

         Suspend_Until_True(bCriticalSemaphore);
         b2 := b;
         Set_True(bCriticalSemaphore);

         Suspend_Until_True(MOCriticalSemaphore);
         MO2 := Copy_Matrix(MO);
         Set_True(MOCriticalSemaphore);

         for i in LastHRows loop
            for j in 1..n loop
               MA(i, j) := 0;
               for k in 1..n loop
                  MA(i, j) :=  MA(i, j) + MB2(i, k) * MC(k, j)
                               + b2 * MO2(i, k) * MK(k, j);
               end loop;
               MA(i, j) := MA(i, j) - MU(i, j);
            end loop;
         end loop;

         Set_True(T2EndSemaphore);
         Put_Line("Process T2 finished");
      end T2;
   begin
      null;
   end Start_Tasks;
begin
   Put_Line("Main procedure started");
   Set_True(MBCriticalSemaphore);
   Set_True(bCriticalSemaphore);
   Set_True(MOCriticalSemaphore);
   Start_Tasks;
end Lab1;
