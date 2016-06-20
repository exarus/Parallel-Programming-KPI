--  Паралельне програмування - 2. Програмування для паралельних систем
--  Лабораторна робота 5. Ада. Захищені модулі
--
--  MA = MB * MB + MO * MK * (E * T)
--
--  Гунавардана Руслан ІП-31
--  16 червня 2015 року н.э.

with Data,
     Ada.Text_IO,
     Ada.Integer_Text_IO,
     Ada.Synchronous_Task_Control;
use Ada.Text_IO,
    Ada.Integer_Text_IO,
    Ada.Synchronous_Task_Control;

procedure Lab5 is
   N: Integer := 2000;     -- vector size
   P: Integer := 4;     -- CPU count
   H: Integer := N / P; -- Subvector size

   package Lab5Data is new Data(N);
   use Lab5Data;

   E, T : Vector;
   MA, MB, MK, MO : Matrix;

   protected PModule is
      entry WInputSync;
      entry WCalc1Sync;
      entry WCalc2Sync;

      procedure SInputSync;
      procedure SCalc1Sync;
      procedure SCalc2Sync;

      function Copy_v return Integer;
      function Copy_MB return Matrix;
      function Copy_MO return Matrix;

      procedure Add_v (addend: in Integer);
      procedure Write_MO (inputMatrix: in Matrix);
      procedure Write_MB (inputMatrix: in Matrix);
   private
      fInput: integer := 0;
      fCalc1: integer := 0;
      fCalc2: integer := 0;

      v: integer := 0;
      MB: Matrix;
      MO: Matrix;
   end PModule;

   protected body PModule is

      entry WInputSync when fInput = 1 is
      begin
         null;
      end WInputSync;

      entry WCalc1Sync when fCalc1 = P is
      begin
         null;
      end WCalc1Sync;


      entry WCalc2Sync when fCalc2 = P - 1 is
      begin
         null;
      end WCalc2Sync;

      procedure SInputSync is
      begin
         fInput := fInput + 1;
      end SInputSync;

      procedure SCalc1Sync is
      begin
         fCalc1 := fCalc1 + 1;
      end SCalc1Sync;

      procedure SCalc2Sync is
      begin
         fCalc2 := fCalc2 + 1;
      end SCalc2Sync;

      function Copy_v return Integer is
      begin
         return v;
      end Copy_v;

      function Copy_MB return Matrix is
      begin
         return MB;
      end Copy_MB;

      function Copy_MO return Matrix is
      begin
         return MO;
      end Copy_MO;

      procedure Add_v(addend: in Integer) is
      begin
         v := v + addend;
      end Add_v;

      procedure Write_MB(inputMatrix: in Matrix) is
      begin
         MB := inputMatrix;
      end Write_MB;


      procedure Write_MO(inputMatrix: in Matrix) is
      begin
        MO := inputMatrix;
      end Write_MO;

   end PModule;

   procedure Start_Tasks is
      procedure Calculate_MA (from: in Integer; to: in Integer) is
         v_copy : Integer := 0;
         MB_copy, MO_copy : Matrix;
      begin
         for i in from..to loop
            v_copy := v_copy + E(i) * T(i);
         end loop;
         PModule.Add_v(v_copy);
         PModule.SCalc1Sync;
         PModule.WCalc1Sync;

         v_copy := PModule.Copy_v;
         MB_copy := PModule.Copy_MB;
         MO_copy := PModule.Copy_MO;

         for i in from..to loop
            for j in 1..N loop
               MA(i, j) := 0;
               for k in 1..N loop
                  MA(i, j) :=  MA(i, j) + MB_copy(i, k) * MB(k, j)
                               + MO_copy(i, k) * MK(k, j) * v_copy;
               end loop;
            end loop;
         end loop;
      end Calculate_MA;


      ------------T1------------
      task T1;
      task body T1 is
      begin
         Put_Line("Process T1 started");
         Vector_Input(E);
         Vector_Input(T);
         Matrix_Input(MB);
         Matrix_Input(MK);
         Matrix_Input(MO);
         PModule.Write_MO(MO);
         PModule.Write_MB(MB);
         PModule.SInputSync;
         Calculate_MA(1, H);
         PModule.WCalc2Sync;
         Matrix_Output(MA);
         Put_Line("Process T1 finished");
      end T1;

      ------------T2------------
      task T2;
      task body T2 is
      begin
         Put_Line("Process T2 started");
         PModule.WInputSync;
         Calculate_MA(H + 1, 2 * H);
         PModule.SCalc2Sync;
         Put_Line("Process T2 finished");
      end T2;

      ------------T3------------
      task T3;
      task body T3 is
      begin
         Put_Line("Process T3 started");
         PModule.WInputSync;
         Calculate_MA(2 * H + 1, 3 * H);
         PModule.SCalc2Sync;
         Put_Line("Process T3 finished");
      end T3;

      ------------T4------------
      task T4;
      task body T4 is
      begin
         Put_Line("Process T4 started");
         PModule.WInputSync;
         Calculate_MA(3 * H + 1, N);
         PModule.SCalc2Sync;
         Put_Line("Process T4 finished");
      end T4;
   begin
      null;
   end Start_Tasks;
begin
   Put_Line("Main procedure started");
   Start_Tasks;
   Put_Line("Main procedure finished");
end Lab5;
