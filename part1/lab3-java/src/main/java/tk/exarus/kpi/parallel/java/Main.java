/*
 * Паралельне прогорамування - 1. Основи паралельного програмування
 * Лабораторна робота N 3.
 * ПОТОКИ  В ЯЗЫКЕ JAVA
 * F1 1.16: d = ((A + B)* (C *(MA*MD)))
 * F2 2.23: f = MAX(MK * ML - MO)
 * F3 3.28: V = MAX(MR*S) + MIN((MT*MW + MV))
 * Гунавардана Руслан ІП-31
 * 6 жовтня 2015 року н.э.
 */
package tk.exarus.kpi.parallel.java;

import lombok.val;

import java.util.concurrent.atomic.AtomicInteger;

import static java.lang.System.out;
import static java.util.Arrays.asList;
import static java.util.Arrays.fill;
import static org.apache.commons.math3.linear.MatrixUtils.createRealMatrix;
import static org.apache.commons.math3.linear.MatrixUtils.createRealVector;
import static tk.exarus.kpi.parallel.java.MatrixUtils.max;
import static tk.exarus.kpi.parallel.java.MatrixUtils.min;

/**
 * Multi-thread matrix expression calculations.
 *
 * @author Ruslan Gunawardana
 */
public final class Main {
    private static final int DEFAULT_SIZE = 4;

    public static void main(String[] args) throws InterruptedException {
        final int size = args.length > 0 ? Integer.parseInt(args[0]) : DEFAULT_SIZE;

        val vectorTemplate = new double[size];
        fill(vectorTemplate, 1);
        val matrixTemplate = new double[size][size];
        fill(matrixTemplate, vectorTemplate);

        val A = createRealVector(vectorTemplate);
        val B = createRealVector(vectorTemplate);
        val C = createRealVector(vectorTemplate);
        val S = createRealVector(vectorTemplate);

        val MA = createRealMatrix(matrixTemplate);
        val MD = createRealMatrix(matrixTemplate);
        val MK = createRealMatrix(matrixTemplate);
        val ML = createRealMatrix(matrixTemplate);
        val MO = createRealMatrix(matrixTemplate);
        val MT = createRealMatrix(matrixTemplate);
        val MR = createRealMatrix(matrixTemplate);
        val MW = createRealMatrix(matrixTemplate);
        val MV = createRealMatrix(matrixTemplate);

        val threads = asList(
                new PrintingThread(() -> out.println(A.add(B).dotProduct(MA.multiply(MD).preMultiply(C)))),
                new PrintingThread(() -> out.println(max(MK.multiply(ML).subtract(MO)))),
                new PrintingThread(() -> out.println(max(MR.preMultiply(S)) + min(MT.multiply(MW).add(MV))))
        );
        threads.forEach(t -> t.setPriority(Thread.MAX_PRIORITY));
        threads.forEach(Thread::start);
        for (Thread thread : threads) {
            thread.join();
        }
        out.println("Threads joined.");
    }

    /**
     * Thread, that prints to the System.out about self execution.
     */
    private static class PrintingThread extends Thread {
        private static AtomicInteger lastTaskNumber = new AtomicInteger(1);

        private final int taskNumber;

        public PrintingThread(Runnable target) {
            super(target);
            taskNumber = lastTaskNumber.getAndIncrement();
        }

        @Override
        public void run() {
            out.println("Task " + taskNumber + " started.");
            try {
                Thread.sleep(taskNumber * 1000);
            } catch (InterruptedException e) {
                out.println("Thread " + taskNumber + " was interrupted.");
            }
            super.run();
            out.println("Task " + taskNumber + " finished.");
        }
    }
}
