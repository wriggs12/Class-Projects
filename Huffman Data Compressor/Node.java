/**
 * @author Winston Riggs
 * @version 1.2 11/22/21
 *
 * Node class with a value, frequency, left/right child, visited, and a parent
 */

package huffmanEncoder;

public class Node {
    private char value;
    private int freq;
    private Node left;
    private Node right;
    private Node parent;

    Node(char v, int f) {
        this.value = v;
        this.freq = f;
        this.left = this.right = null;
        this.parent = null;
    }

    Node(int f) {
        this.freq = f;
        this.left = this.right = null;
        this.parent = null;
    }

    public void setLeft(Node l) {
        left = l;
    }

    public void setRight(Node r) {
        right = r;
    }

    public void setParent(Node p) {
        parent = p;
    }

    public int getFreq() {
        return freq;
    }

    public char getVal() {
        return value;
    }

    public Node getLeft() {
        return left;
    }

    public Node getRight() {
        return right;
    }

    public Node getParent() {
        return parent;
    }
}
