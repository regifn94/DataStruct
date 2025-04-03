class Node:

    def __init__(self, data, next=NULL, prev=NULL):
        self.data = data
        self.next = next
        self.prev = prev
    
    def namaMethod(self):
        print("ini adalah method dari class Node")

node = Node()
node.namaMethod()  # Output: ini adalah method dari class Node