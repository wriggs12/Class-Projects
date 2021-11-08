/**
 * @author Winston Riggs
 * @version 10/12/21
 *
 * Insertion sort
 */

public class Insertion {
    public static void main(String[] args) {
        int[] arr = {3, 44, 38, 5, 47, 15, 36, 26, 27, 2, 46, 4, 19};

        System.out.println("Unsorted Array:");
        for (int num: arr) {
            System.out.print(num + " ");
        }

        long startTime = System.nanoTime();
        insertionSort(arr);
        long endTime = System.nanoTime();

        long totalTime = endTime - startTime;

        System.out.println("\nSorted Array:");
        for (int num: arr) {
            System.out.print(num + " ");
        }

        System.out.println("\n\nInsertion sort took " + totalTime + " nanoseconds to sort the array");
    }

    public static void insertionSort(int[] arr) {
        for (int i = 1; i < arr.length; i++) {
            int curNum = arr[i];

            int index = i - 1;
            int numIndex = i;

            while(index > -1 && curNum < arr[index]) {
                int temp = arr[numIndex];
                arr[numIndex] = arr[index];
                arr[index] = temp;
                index--;
                numIndex--;
            }
        }
    }
}