/**
 * @author Winston Riggs
 * @version 1.2 11/22/21
 *
 * Binary Tree class with a root using the Node class
 */

package huffmanEncoder;

public class BinaryTree {
    private Node root;

    BinaryTree() {
        root = null;
    }

    BinaryTree(Node n) {
        root = n;
    }

    public Node getRoot() {
        return root;
    }

    public void setRoot(Node r) {
        root = r;
    }
}
