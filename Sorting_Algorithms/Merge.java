/**
 * @author Winston Riggs
 * @version 10/12/21
 *
 * Merge sort
 */

import java.util.Arrays;

public class Merge {
    public static void main(String[] args) {
        int[] arr = {3, 44, 38, 5, 47, 15, 36, 26, 27, 2, 46, 4, 19};

        System.out.println("Unsorted Array:");
        for (int num: arr) {
            System.out.print(num + " ");
        }

        long startTime = System.nanoTime();
        arr = mergeSort(arr);
        long endTime = System.nanoTime();

        long totalTime = endTime - startTime;

        System.out.println("\nSorted Array:");
        for (int num: arr) {
            System.out.print(num + " ");
        }

        System.out.println("\n\nMerge sort took " + totalTime + " nanoseconds to sort the array");
    }

    public static int[] mergeSort(int[] arr) {
        if (arr.length < 2)
            return arr;

        int[] left = Arrays.copyOfRange(arr, 0, arr.length / 2);
        left = mergeSort(left);

        int[] right = Arrays.copyOfRange(arr,arr.length / 2, arr.length);
        right = mergeSort(right);

        return merge(left, right);
    }

    public static int[] merge(int[] arr1, int[] arr2) {
        int[] mergedArr = new int[arr1.length + arr2.length];

        int left = 0;
        int right = 0;

        for (int i = 0; i < mergedArr.length; i++) {
            if (left != -1 && right != -1) {
                if (arr1[left] < arr2[right]) {
                    mergedArr[i] = arr1[left];
                    left++;
                    if (left == arr1.length)
                        left = -1;
                }
                else {
                    mergedArr[i] = arr2[right];
                    right++;
                    if (right == arr2.length)
                        right = -1;
                }
            }
            else if (left == -1) {
                mergedArr[i] = arr2[right];
                right++;
            }
            else {
                mergedArr[i] = arr1[left];
                left++;
            }
        }

        return mergedArr;
    }
}
