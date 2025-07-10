/**
 * Will solve any sudoku board using a back-tracking algorithm.
 * @aurthor Winston Riggs
 * @version 1.0 6/2/2020
 */

public class solver {
    public static void main(String[] args) {
        //Creates the board that will be solved.
        int[][] board = new int[][]
                {
                        {8, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 3, 6, 0, 0, 0, 0, 0},
                        {0, 7, 0, 0, 9, 0, 2, 0, 0},
                        {0, 5, 0, 0, 0, 7, 0, 0, 0},
                        {0, 0, 0, 0, 4, 5, 7, 0, 0},
                        {0, 0, 0, 1, 0, 0, 0, 3, 0},
                        {0, 0, 1, 0, 0, 0, 0, 6, 8},
                        {0, 0, 8, 5, 0, 0, 0, 1, 0},
                        {0, 9, 0, 0, 0, 0, 4, 0, 0}
                };

        System.out.println("Welcome to my final project! I will use a back-tracking algorithm to solve the board.");
        System.out.println("Original Board: ");

        printBoard(board);

        System.out.println("Solved Board: ");

        solve(board);
    }

    /**
     * Will solve a given sudoku board.
     *
     * @param board A 2-D array of integers with size of 9, 9.
     * @return A boolean value whether the current stae of the board is valid.
     */
    public static boolean solve(int[][] board) {
        //Keep track whether the board is complete or not
        boolean isFull = true;

        int row = 0;
        int col = 0;

        //Finds the first open spot in the sudoku board
        for (int i = 0; i < board.length; i++) {
            for (int j = 0; j < board[i].length; j++) {
                if (board[i][j] == 0) {
                    //Sets the row and column to the empty spot
                    isFull = false;
                    row = i;
                    col = j;
                    break;
                }
            }
            if (!isFull)
                break;
        }

        //Once determined that no spots are open it will print out the board and exit the method
        if (isFull) {
            printBoard(board);
            return true;
        }

        //Will loop through every number (1 - 9) and try it in the spot and if it work will call this function again to
        //move to the next empty spot.
        for (int i = 1; i <= board.length; i++) {
            //Checks is the current number is valid in the spot
            if (isValid(board, row, col, i)) {
                board[row][col] = i;
                //Will return true when the entire board is complete or will back-track if no numbers work
                if (solve(board))
                    return true;
                else
                    board[row][col] = 0;
            }
        }
        return false;
    }

    /**
     * Will print out the sudoku board.
     *
     * @param board A 2-D array representing a sudoku board
     */
    public static void printBoard(int[][] board) {
        //Loops through the entire array and prints it out in order
        for (int i = 0; i < board.length; i++) {
            for (int j = 0; j < board[i].length; j++) {
                System.out.print(board[i][j]);
            }
            System.out.println();
        }
        System.out.println();
    }

    /**
     * Will check if a row in a 2-D array has no other given number.
     *
     * @param board A 2-D array representing a sudoku board
     * @param row   The index of the desired row number to check
     * @param num   The number to check for
     * @return A boolean value. True if there are no other appearances of the number
     */
    public static boolean isRowValid(int[][] board, int row, int num) {
        //Loops through the entire row
        for (int i = 0; i < board.length; i++) {
            //If it finds the same number will return false and end method
            if (board[row][i] == num)
                return false;
        }
        return true;
    }

    /**
     * Will check if a column in a 2-D array has no other given number.
     *
     * @param board A 2-D array representing a sudoku board
     * @param col   The index of the desired column number to check
     * @param num   The number to check for
     * @return A boolean value. True if there are no other appearances of the number
     */
    public static boolean isColValid(int[][] board, int col, int num) {
        //Loops through the entire column
        for (int i = 0; i < board.length; i++) {
            //If it finds the same number will return false and end method
            if (board[i][col] == num)
                return false;
        }
        return true;
    }

    /**
     * Will check the 3 by 3 square the number is in to see if it is valid.
     *
     * @param board A 2-D array representing a sudoku board
     * @param row   The index of the desired row number
     * @param col   The index of the desired column number
     * @param num   The number to check for
     * @return A boolean value. True if there are no other appearances of the number
     */
    public static boolean isSquareValid(int[][] board, int row, int col, int num) {
        //Sets the row and column to the top left spot in the 3 by 3 square
        row = row - row % 3;
        col = col - col % 3;

        //Loops through the entire square and looks for a matching number
        for (int i = row; i < row + 3; i++) {
            for (int j = col; j < col + 3; j++) {
                if (board[i][j] == num)
                    return false;
            }
        }
        return true;
    }

    /**
     * Will check if the number is valid in the row, column, and square.
     *
     * @param board A 2-D array representing a sudoku board
     * @param row   The index of the desired row number
     * @param col   The index of the desired column number
     * @param num   The number to check for
     * @return A boolean value. True if there are no other appearances of the number
     */
    public static boolean isValid(int[][] board, int row, int col, int num) {
        return isRowValid(board, row, num) && isColValid(board, col, num) && isSquareValid(board, row, col, num);
    }

}
