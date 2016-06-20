package ua.gunawardana.kpi.pp.lab6;

import static java.lang.System.out;
import static java.util.Arrays.stream;

public final class MatrixUtil {
    public static final int FILL_WITH_VALUE = 1;
    public static final int MAX_PRINTABLE = 8;

    private MatrixUtil() {
        throw new AssertionError("Should not be instantiated.");
    }

    public static void input(int[] vector) {
        for (int i = 0; i < vector.length; i++) {
            vector[i] = FILL_WITH_VALUE;
        }
    }

    public static void input(int[][] matrix) {
        for (int i = 0; i < matrix.length; i++) {
            for (int j = 0; j < matrix[i].length; j++) {
                matrix[i][j] = FILL_WITH_VALUE;
            }
        }
    }

    public static void output(int[] vector) {
        if (vector.length > MAX_PRINTABLE) return;
        stream(vector).mapToObj(e -> e + " ").forEach(out::print);
        out.println();
    }

    public static void output(int[][] matrix) {
        if (matrix.length > MAX_PRINTABLE) return;
        stream(matrix).forEach(MatrixUtil::output);
    }
}
