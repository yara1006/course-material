import java.util.*;

public class EggDrop {

    static int k = 10000; // 鸡蛋数
    static int n = 10000; // 楼层数

    // 普通 DP 二维数组
    static int dpNormal(int k, int n) {
        int[][] dp = new int[k + 1][n + 1];
        for (int i = 1; i <= k; i++)
            for (int j = 1; j <= n; j++)
                dp[i][j] = j;

        for (int i = 2; i <= k; i++) {
            for (int j = 1; j <= n; j++) {
                for (int x = 1; x <= j; x++) {
                    dp[i][j] = Math.min(dp[i][j],
                            Math.max(dp[i - 1][x - 1], dp[i][j - x]) + 1);
                }
            }
        }
        return dp[k][n];
    }

    // 一维空间优化
    static int dpOneArray(int k, int n) {
        int[] prev = new int[n + 1];
        int[] curr = new int[n + 1];
        for (int i = 1; i <= n; i++) curr[i] = i;

        for (int egg = 2; egg <= k; egg++) {
            System.arraycopy(curr, 0, prev, 0, n + 1);
            for (int i = 1; i <= n; i++) curr[i] = i;

            for (int f = 1; f <= n; f++) {
                for (int x = 1; x < f; x++) {
                    curr[f] = Math.min(curr[f], Math.max(prev[x - 1], curr[f - x]) + 1);
                }
            }
        }
        return curr[n];
    }

    // 二分搜索优化
    static int dpBinary(int k, int n) {
        int[][] dp = new int[k + 1][n + 1];
        for (int i = 1; i <= k; i++)
            for (int j = 1; j <= n; j++)
                dp[i][j] = j;

        for (int i = 2; i <= k; i++) {
            for (int j = 1; j <= n; j++) {
                int low = 1, high = j;
                while (low <= high) {
                    int mid = (low + high) / 2;
                    int breakCase = dp[i - 1][mid - 1];
                    int notBreakCase = dp[i][j - mid];
                    if (breakCase > notBreakCase) {
                        dp[i][j] = Math.min(dp[i][j], breakCase + 1);
                        high = mid - 1;
                    } else {
                        dp[i][j] = Math.min(dp[i][j], notBreakCase + 1);
                        low = mid + 1;
                    }
                }
            }
        }
        return dp[k][n];
    }

    // 状态转移优化（操作次数 m 为阶段）
    static int dpStateTransfer(int k, int n) {
        int[][] dp = new int[n + 2][k + 2];
        int m = 0;
        while (dp[m][k] < n) {
            m++;
            for (int i = 1; i <= k; i++) {
                dp[m][i] = dp[m - 1][i - 1] + dp[m - 1][i] + 1;
            }
        }
        return m;
    }

    // 状态转移 + 一维空间优化
    static int dpState1D(int k, int n) {
        int[] dp = new int[k + 1];
        int m = 0;
        while (dp[k] < n) {
            m++;
            for (int i = k; i >= 1; i--) {
                dp[i] = dp[i] + dp[i - 1] + 1;
            }
        }
        return m;
    }

    public static void main(String[] args) {
        for (int method = 2; method <= 6; method++) {
            long start = System.currentTimeMillis();
            int ans = 0;
            switch (method) {
                case 2:
                    System.out.println("方法2：二维DP暴力");
                    ans = dpNormal(k, n);
                    break;
                case 3:
                    System.out.println("方法3：一维空间优化");
                    ans = dpOneArray(k, n);
                    break;
                case 4:
                    System.out.println("方法4：二分搜索优化");
                    ans = dpBinary(k, n);
                    break;
                case 5:
                    System.out.println("方法5：状态转移优化");
                    ans = dpStateTransfer(k, n);
                    break;
                case 6:
                    System.out.println("方法6：状态转移 + 一维空间优化");
                    ans = dpState1D(k, n);
                    break;
            }
            long end = System.currentTimeMillis();
            System.out.println("最少尝试次数: " + ans);
            System.out.println("耗时: " + (end - start) + " ms");
            System.out.println();
        }
    }
}
