/**
 * @author Winston Riggs
 * @version 10/12/21
 *
 * Selection Sort
 */

public class  Selection {
    public static void main(String[] args) {
        int[] arr = {3, 44, 38, 5, 47, 15, 36, 26, 27, 2, 46, 4, 19};

        System.out.println("Unsorted Array:");
        for (int num: arr) {
            System.out.print(num + " ");
        }

        long startTime = System.nanoTime();
        selectionSort(arr);
        long endTime = System.nanoTime();

        long totalTime = endTime - startTime;

        System.out.println("\nSorted Array:");
        for (int num: arr) {
            System.out.print(num + " ");
        }

        System.out.println("\n\nSelection sort took " + totalTime + " nanoseconds to sort the array");
    }

    public static void selectionSort(int[] arr) {
        for (int i = 0; i < arr.length; i++) {
            int min = Integer.MAX_VALUE;
            int minIndex = 0;

            for (int j = i; j < arr.length; j++) {
                if (arr[j] < min) {
                    min = arr[j];
                    minIndex = j;
                }
            }

            int temp = arr[i];
            arr[i] = arr[minIndex];
            arr[minIndex] = temp;
        }
    }
}