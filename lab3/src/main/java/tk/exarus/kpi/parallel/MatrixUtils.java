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
package tk.exarus.kpi.parallel;

import org.apache.commons.math3.linear.*;

/**
 * Utils implement the missing methods for matrices and vectors in Apache Commons.
 *
 * @author Ruslan Gunawardana
 */
public final class MatrixUtils {
    private MatrixUtils() {
        // utility class should not be initialized
    }

    public static double max(RealMatrix matrix) {
        return matrix.walkInOptimizedOrder(new RealMatrixPreservingVisitor() {
            private double maxValue = Double.NEGATIVE_INFINITY;

            @Override
            public void start(int rows, int columns, int startRow, int endRow, int startColumn, int endColumn) {
                if (rows == 0 && columns == 0) maxValue = Double.NaN;
            }

            @Override
            public void visit(int row, int column, double value) {
                if (value > maxValue) maxValue = value;
            }

            @Override
            public double end() {
                return maxValue;
            }
        });
    }

    public static double max(RealVector vector) {
        return vector.getMaxValue();
    }

    public static double min(RealMatrix matrix) {
        return matrix.walkInOptimizedOrder(new RealMatrixPreservingVisitor() {
            private double minValue = Double.POSITIVE_INFINITY;

            @Override
            public void start(int rows, int columns, int startRow, int endRow, int startColumn, int endColumn) {
                if (rows == 0 && columns == 0) minValue = Double.NaN;
            }

            @Override
            public void visit(int row, int column, double value) {
                if (value < minValue) minValue = value;
            }

            @Override
            public double end() {
                return minValue;
            }
        });
    }

    public static double min(RealVector vector) {
        return vector.getMinValue();
    }
}
