class TreeNode:
    def __init__(self, data):
        self.data = data
        self.children = []
        self.parent = None
    
    def insertChild(self, child):
        child.parent = self
        self.children.append(child)
    
    def print_tree(self):
        print(self.data)
        if self.children:
            for child in self.children:
                child.print_tree()
    

def build_product_tree():
    root = TreeNode("Electronics")
    laptop = TreeNode("Laptop")
    cellphone = TreeNode("Cellphone")
        
    root.insertChild(laptop)
    root.insertChild(cellphone)
    
    return root
        

root = build_product_tree()
root.print_tree()
        