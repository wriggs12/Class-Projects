package huffmanEncoder;

import java.io.File;
import java.io.FileWriter;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Huffman encoder = new Huffman();
        encoder.compressFile("./Files/test.txt");
        encoder.decompressFile("./Files/compressed.txt");
    }
}