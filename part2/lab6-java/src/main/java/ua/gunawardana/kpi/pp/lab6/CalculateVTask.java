package ua.gunawardana.kpi.pp.lab6;

import java.util.concurrent.RecursiveTask;

public final class CalculateVTask extends RecursiveTask<Integer> {
    private final int[] E, T;
    private final int threshold;
    private final int from, to;

    public CalculateVTask(int[] E, int[] T, int threshold) {
        this(E, T, threshold, 0, E.length);
    }

    private CalculateVTask(int[] E, int[] T, int threshold, int from, int to) {
        if (E.length != T.length) throw new IllegalArgumentException("E should be sized as T");
        this.E = E;
        this.T = T;
        this.threshold = threshold;
        this.from = from;
        this.to = to;
    }

    @Override
    protected Integer compute() {
        if (to - from > threshold) {
            int mid = (to + from) >>> 1;
            CalculateVTask left = new CalculateVTask(E, T, threshold, from, mid);
            CalculateVTask right = new CalculateVTask(E, T, threshold, mid, to);
            left.fork();
            right.fork();
            return right.join() + left.join();
        } else {
            int product = 0;
            for (int i = from; i < to; i++) {
                product += E[i] * T[i];
            }
            return product;
        }
    }
}
