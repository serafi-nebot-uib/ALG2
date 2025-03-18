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

    private static void swap(int[] arr, int i, int j) {
        int t = arr[i];
        arr[i] = arr[j];
        arr[j] = t;
        MainActivity.repaint(arr);
    }

    public static void selectionSort(int[] arr) {
        for (int i = 0; i < arr.length - 1; i++) {
            int m = i;
            for (int j = i + 1; j < arr.length; j++)
                if (arr[j] < arr[m])
                    m = j;
            swap(arr, m, i);
        }
    }

    public static int partition(int[] arr, int l, int r) {
        int s = l, p = r;
        for (int i = s; i < p; i++) {
            if (arr[i] >= arr[p]) continue;
            swap(arr, s, i);
            s++;
        }
        swap(arr, s, p);
        return s;
    }

    public static void quickSort(int[] arr, int l, int r) {
        if (r - l < 1) return;
        int s = partition(arr, l, r);
        quickSort(arr, l, s-1);
        quickSort(arr, s+1, r);
    }

    public static void merge(int[] arr, int l, int m, int r) {
        int[] larr = new int[m-l+1];
        int[] rarr = new int[r-m];
        for (int i = 0; i < larr.length; i++) larr[i] = arr[l+i];
        for (int i = 0; i < rarr.length; i++) rarr[i] = arr[m+1+i];

        int i = 0, j = 0, k = l;
        while (i < larr.length && j < rarr.length) {
            arr[k++] = larr[i] < rarr[j] ? larr[i++] : rarr[j++];
            MainActivity.repaint(arr);
        }
        while (i < larr.length) {
            arr[k++] = larr[i++];
            MainActivity.repaint(arr);
        }
        while (j < rarr.length) {
            arr[k++] = rarr[j++];
            MainActivity.repaint(arr);
        }
    }

    public static void mergeSort(int[] arr, int l, int r) {
        if (l >= r) return;
        int m = (r + l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);
        merge(arr, l, m, r);
    }
}