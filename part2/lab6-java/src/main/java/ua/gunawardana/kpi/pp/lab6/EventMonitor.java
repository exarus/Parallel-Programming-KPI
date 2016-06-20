package ua.gunawardana.kpi.pp.lab6;

public final class EventMonitor {
    private final int P;
    private boolean fInput;
    private int fCalc1;
    private int fCalc2;

    public EventMonitor(int p) {
        P = p;
    }

    public synchronized void waitInput() {
        while (!fInput) {
            try {
                wait();
            } catch (InterruptedException ignored) {}
        }
    }

    public synchronized void signalInput() {
        if (!fInput) {
            notifyAll();
            fInput = true;
        }
    }

    public synchronized void waitCalcV() {
        while (fCalc1 < P) {
            try {
                wait();
            } catch (InterruptedException ignored) {}
        }
    }

    public synchronized void signalCalcV() {
        if (++fCalc1 == P) notifyAll();
    }

    public synchronized void waitCalcMA() {
        while (fCalc2 < P - 1) {
            try {
                wait();
            } catch (InterruptedException ignored) {}
        }
    }

    public synchronized void signalCalcMA() {
        if (++fCalc2 == P) notifyAll();
    }
}
