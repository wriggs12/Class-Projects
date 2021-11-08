/**
 * @author Winston Riggs
 * @version 10/12/21
 *
 * Bubble sort
 */

public class Bubble {
    public static void main(String[] args) {
        int[] arr = {3, 44, 38, 5, 47, 15, 36, 26, 27, 2, 46, 4, 19};

        System.out.println("Unsorted Array:");
        for (int num: arr) {
            System.out.print(num + " ");
        }

        long startTime = System.nanoTime();
        bubbleSort(arr);
        long endTime = System.nanoTime();

        long totalTime = endTime - startTime;

        System.out.println("\nSorted Array:");
        for (int num: arr) {
            System.out.print(num + " ");
        }

        System.out.println("\n\nBubble sort took " + totalTime + " nanoseconds to sort the array");
    }

    public static void bubbleSort(int[] arr) {
        boolean swapped;
        do {
            swapped = false;
            for (int i = 1; i < arr.length; i++) {
                if (arr[i - 1] > arr[i]) {
                    int temp = arr[i - 1];
                    arr[i - 1] = arr[i];
                    arr[i] = temp;
                    swapped = true;
                }
            }
        }
        while (swapped);
    }
}

