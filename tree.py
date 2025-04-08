from graphviz import Digraph

class Node:
    def __init__(self, key):
        self.left = None
        self.right = None
        self.key = key

class BinaryTree:
    def __init__(self, root_key):
        self.root = Node(root_key)

    def insert_level_order(self, arr, root, i, n):
        if i < n:
            temp = Node(arr[i])
            root = temp
            root.left = self.insert_level_order(arr, root.left, 2 * i + 1, n)
            root.right = self.insert_level_order(arr, root.right, 2 * i + 2, n)
        return root

    def build_tree(self, arr):
        self.root = self.insert_level_order(arr, self.root, 0, len(arr))

    def preorder(self, node):
        if node:
            print(node.key, end=" ")
            self.preorder(node.left)
            self.preorder(node.right)

    def inorder(self, node):
        if node:
            self.inorder(node.left)
            print(node.key, end=" ")
            self.inorder(node.right)

    def postorder(self, node):
        if node:
            self.postorder(node.left)
            self.postorder(node.right)
            print(node.key, end=" ")

    def visualize_tree(self, filename="binary_tree"):
        dot = Digraph()
        self._add_nodes_edges(dot, self.root)
        dot.render(filename, format='png', cleanup=False)
        print(f"Tree visualization saved as {filename}.png")

    def _add_nodes_edges(self, dot, node):
        if node:
            dot.node(str(node.key), str(node.key))
            if node.left:
                dot.edge(str(node.key), str(node.left.key))
                self._add_nodes_edges(dot, node.left)
            if node.right:
                dot.edge(str(node.key), str(node.right.key))
                self._add_nodes_edges(dot, node.right)

# Membuat binary tree dengan "JHDBGMIKACFL"
bt = BinaryTree('J')
bt.build_tree("JHDBGMIKACFL")

print("Preorder Traversal:")
bt.preorder(bt.root)
print("\nInorder Traversal:")
bt.inorder(bt.root)
print("\nPostorder Traversal:")
bt.postorder(bt.root)

# Visualisasi pohon
bt.visualize_tree()
