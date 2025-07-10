/**
 * @author Winston Riggs
 * @version 1.2 11/22/21
 *
 * Implements the Huffman Encoding Algorithm to compress and decompress files
 */

package huffmanEncoder;

//import required classes
import java.io.File;
import java.io.FileWriter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Scanner;

public class Huffman {
    private HashMap<Character, String> codes;
    private HashMap<String, Character> decompressCodes;
    private BinaryTree huffmanTree;
    private static final String headBodySeparator = "@$$%";

    /**
     * Default Constructor
     */
    public Huffman() {
        codes = new HashMap<Character, String >();
        decompressCodes = new HashMap<String, Character>();
        huffmanTree = new BinaryTree();
    }

    /**
     * Will compress a given file using the Huffman encoding technique
     * @param filePath String of the path to the file to compress
     * @return void
     */
    public void compressFile(String filePath) {
        try{
            File inputFile = new File(filePath);
            Scanner fileReader = new Scanner(inputFile);

            String input = "";

            while (fileReader.hasNextLine()) {
                input += fileReader.nextLine();
            }

            fileReader.close();

            String compressedText = compress(input);
            File outputFile = new File("./Files/compressed.txt");
            FileWriter writer = new FileWriter(outputFile);

            writer.write(compressedText);
            writer.close();
        }
        catch(Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * Given the input string will compress using the Huffman encoding technique
     * @param input String of the input text
     * @return A compressed String of 0's and 1's
     */
    private String compress(String input) {
        char[] msgChar = input.toCharArray();
        ArrayList<Character> characters = new ArrayList<>();

        //Removes any repeated characters
        for (char c : msgChar) {
            if (!(characters.contains(c)))
                characters.add(c);
        }

        int[] countOfChar = new int[characters.size()];

        //Finds the frequency of each character in the String
        for (int i = 0; i < characters.size(); i++) {
            char checker = characters.get(i);
            for (char c : msgChar) {
                if (checker == c)
                    countOfChar[i]++;
            }
        }

        //Sorts the characters in descending order of frequency
        bubbleSort(characters, countOfChar);

        //generates the binary tree with the huffman algorithm
        huffmanTree = generateHuffmanTree(characters, countOfChar);

        //gets the binary values of each character
        codes = findEncryptedVals(huffmanTree.getRoot(), "");

        //Builds the string of binary values with the assigned binary values
        StringBuilder compressedString = new StringBuilder();
        for (int i = 0; i < input.length(); i++) {
            compressedString.append(codes.get(input.charAt(i)));
        }

        String header = generateHeader(characters);

        return header + headBodySeparator + "\n" + compressedString;
    }

    /**
     * Given a compressed file, will decompress
     * @param fileName String of the file to decompress
     * @return void
     */
    public void decompressFile(String fileName) {
        try {
            String input = findCodes(fileName);
            String curCode = "";

            String original = "";

            for (int i = 0; i < input.length(); i++) {
                curCode += input.charAt(i);

                if (decompressCodes.containsKey(curCode)) {
                    original += decompressCodes.get(curCode);
                    curCode = "";
                }
            }

            File outputFile = new File("./Files/decompressed.txt");
            FileWriter writer = new FileWriter(outputFile);

            writer.write(original);
            writer.close();
        }
        catch(Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * Will find the codes associated with each character
     * @param fileName name of the file to compress
     * @post codes contains the keys for each character
     * @return String of the input text
     */
    private String findCodes(String fileName) {
        try {
            File inputFile = new File(fileName);
            Scanner fileReader = new Scanner(inputFile);

            String input = "";
            boolean header = true;

            while (fileReader.hasNextLine()) {
                String temp = fileReader.nextLine();

                if (!header)
                    input += temp;
                else {
                    if (temp.equals(headBodySeparator))
                        header = false;
                    else {
                        char c = temp.charAt(0);
                        String code = temp.substring(3);

                        decompressCodes.put(code, c);
                    }
                }
            }

            fileReader.close();

            return input;
        }
        catch(Exception e) {
            e.printStackTrace();
        }
        return "";
    }

    /**
     * Will generate the header for a file with metadata
     * @param characters ArrayList of the unique characters in the input text
     * @return A String containing the header
     */
    private String generateHeader(ArrayList<Character> characters) {
        String header = "";

        for (int i = 0; i < codes.size(); i++) {
            header += characters.get(i) + ": " + codes.get(characters.get(i)) + "\n";
        }

        return header;
    }

    /**
     * Generates the huffman binary tree
     * @param characters ArrayList containing all the non-repeated characters
     * @param countOfChar Array the contains all the frequencies of the letters in the same order and the characters ArrayList
     * @return Binary tree of Nodes
     */
    private BinaryTree generateHuffmanTree(ArrayList<Character> characters, int[] countOfChar) {
        //Creates all required Nodes with the character and frequency
        ArrayList<Node> allNodes = new ArrayList<>();
        for (int i = 0; i < characters.size(); i++) {
            allNodes.add(new Node(characters.get(i), countOfChar[i]));
        }

        Node current = allNodes.get(0);
        BinaryTree huffmanTree = new BinaryTree();

        //generates the huffman binary tree by connecting the bottom two Nodes and placing them back in
        while(allNodes.size() > 1) {
            //Temporary parent Node
            Node temp = new Node(allNodes.get(0).getFreq() + allNodes.get(1).getFreq());
            temp.setLeft(allNodes.get(0));
            temp.setRight(allNodes.get(1));

            //No need for these as parent node points to them
            allNodes.remove(0);
            allNodes.remove(0);

            //put back in the temporary parent node into the array to be sorted
            siftInNode(allNodes, temp);

            current = allNodes.get(0);
        }

        //Sets the root of the tree to the last node which will be the root
        huffmanTree.setRoot(current);

        return huffmanTree;
    }

    /**
     * Will sift the Node back into place
     * @param allNodes ArrayList of Nodes in descending order of frequency
     * @param temp Temporary Node to sift into the dynamic array
     */
    private void siftInNode(ArrayList<Node> allNodes, Node temp) {
        int index = 0;
        //Finds the index at which the Node belongs
        while(index < allNodes.size() && allNodes.get(index).getFreq() < temp.getFreq())
            index++;
        allNodes.add(index, temp);
    }

    /**
     * Sorts an ArrayList in descending order using bubbleSort
     * @param characters ArrayList of characters
     * @param countOfChar integer array of frequencies related to the characters in the arraylist
     */
    private void bubbleSort(ArrayList<Character> characters, int[] countOfChar) {
        //Bubble Sort
        for (int i = 0; i < countOfChar.length - 1; i++) {
            for (int j = 0; j < countOfChar.length - 1; j++) {
                if (countOfChar[j] > countOfChar[j + 1]) {
                    int temp = countOfChar[j];
                    countOfChar[j] = countOfChar[j + 1];
                    countOfChar[j + 1] = temp;

                    char tempChar = characters.get(j);
                    characters.set(j, characters.get(j + 1));
                    characters.set(j + 1, tempChar);
                }
            }
        }

    }

    /**
     * Given the huffman binary tree, will find the binary values of all the characters in the String
     * @param root Node of the huffman tree
     * @param curCode String of the current code
     * @return An array of Strings and each string has the character at index 0 and the binary code after
     */
    private HashMap<Character, String> findEncryptedVals(Node root, String curCode) {
        if (root.getLeft() == null) {
            codes.put(root.getVal(), curCode);
            return codes;
        }
        else
            codes = findEncryptedVals(root.getLeft(), curCode + "0");

        if (root.getRight() != null) {
            codes = findEncryptedVals(root.getRight(), curCode + "1");
        }

        return codes;
    }
}