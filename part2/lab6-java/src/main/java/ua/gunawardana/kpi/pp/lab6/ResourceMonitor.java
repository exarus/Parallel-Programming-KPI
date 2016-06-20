package ua.gunawardana.kpi.pp.lab6;

import java.util.Objects;

import static java.util.Arrays.stream;

public final class ResourceMonitor {
    private volatile int v = 0;
    private final int[][] MB, MO;

    public ResourceMonitor(int[][] MB, int[][] MO) {
        Objects.requireNonNull(MB);
        Objects.requireNonNull(MO);
        this.MB = MB;
        this.MO = MO;
    }

    public int getV() {
        return v;
    }

    public void setV(int v) {
        this.v = v;
    }

    public synchronized int[][] getMB() {
        return deepCopy(MB);
    }

    public synchronized int[][] getMO() {
        return deepCopy(MO);
    }

    private static int[][] deepCopy(int[][] m) {
        return stream(m).map(int[]::clone).toArray($ -> m.clone());
    }
}
