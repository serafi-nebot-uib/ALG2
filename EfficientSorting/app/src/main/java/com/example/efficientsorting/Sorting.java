package com.example.efficientsorting;

import android.util.Log;

import java.util.Random;

public class Sorting {
    public static final Integer MAX_VAL = 100;
    public static String printArray(int [] arr) {
        StringBuilder str = new StringBuilder();
        int m = arr.length, i = 0;
        m = Math.min(m, 20);
        for (; i < m-1; i++) str.append(arr[i]).append(",");
        str.append(arr[i]);
        return str.toString();
    }

    public static int [] generateRandomArray(int n) {
        Random random = new Random();
        int[] arr = new int[n];
        for (int i = 0; i < n; i++) arr[i] = random.nextInt(100);
        return arr;
    }

    public static void selectionSort(int [] arr) {
        for (int i = 0; i < arr.length - 1; i++) {
            int m = i;
            for (int j = i + 1; j < arr.length; j++)
                if (arr[j] < arr[m])
                    m = j;
            int tmp = arr[m];
            arr[m] = arr[i];
            arr[i] = tmp;
            MainActivity.repaint(arr);
        }
    }
}