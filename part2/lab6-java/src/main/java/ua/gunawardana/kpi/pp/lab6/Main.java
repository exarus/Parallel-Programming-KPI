package ua.gunawardana.kpi.pp.lab6;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ForkJoinPool;

import static ua.gunawardana.kpi.pp.lab6.MatrixUtil.input;
import static ua.gunawardana.kpi.pp.lab6.MatrixUtil.output;

public final class Main {
    private static final int DEFAULT_N = 4;
    private static final int DEFAULT_P = 4;

    private static int[] E, T;
    private static int[][] MA, MB, MO, MK;

    private static EventMonitor eventMonitor;
    private static ResourceMonitor resourceMonitor;

    public static void main(String[] args) {
        final int N = (args.length > 0) ? Integer.parseInt(args[0]) : DEFAULT_N;
        final int P = (args.length > 1) ? Integer.parseInt(args[1]) : DEFAULT_P;
        final int[] H = new int[P + 1];
        for (int i = 0; i < H.length; i++) {
            H[i] = (int) (N * ((double) i) / P);
        }

        E = new int[N];
        T = new int[N];
        MA = new int[N][N];
        MB = new int[N][N];
        MO = new int[N][N];
        MK = new int[N][N];

        eventMonitor = new EventMonitor(P);
        resourceMonitor = new ResourceMonitor(MB, MO);
        ForkJoinPool pool = new ForkJoinPool(P);

        pool.execute(Main::inputData);
        eventMonitor.waitInput();
        resourceMonitor.setV(pool.invoke(new CalculateVTask(E, T, N / P)));

        List<Runnable> tasks = new ArrayList<>();
        for (int i = 0; i < P; i++) {
            final int finalI = i;
            tasks.add(() -> calculateMA(H[finalI], H[finalI + 1]));
        }
        tasks.forEach(pool::execute);
        eventMonitor.waitCalcMA();
        output(MA);
    }

    public static void inputData() {
        input(E);
        input(T);
        input(MB);
        input(MK);
        input(MO);
        eventMonitor.signalInput();
    }

    public static void calculateMA(int from, int to) {
        final int vCopy = resourceMonitor.getV();
        final int[][] MBCopy = resourceMonitor.getMB();
        final int[][] MOCopy = resourceMonitor.getMO();

        for (int i = from; i < to; i++) {
            for (int j = 0; j < MA[i].length; j++) {
                MA[i][j] = 0;
                for (int k = 0; k < MB[i].length; k++) {
                    MA[i][j] += (MBCopy[i][k] * MB[k][j]) + (MOCopy[i][k] * MK[k][j] * vCopy);
                }
            }
        }
        eventMonitor.signalCalcMA();
    }
}
