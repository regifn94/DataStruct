# excercise 1

class Node:
    def __init__(self, data=None):
        self.data = data
        self.next = None

class LinkedList:
    def __init__(self):
        self.head = None

    def append(self, data):
        new_node = Node(data)
        if self.head is None:
            self.head = new_node
            return
        last = self.head
        while last.next:
            last = last.next
        last.next = new_node

    def display(self):
        temp = self.head
        while temp:
            print(temp.data, end=" -> ")
            temp = temp.next
        print("None")


ll = LinkedList()
print("Masukkan 5 nilai untuk Linked List:")
for i in range(5):
    data = input(f"Masukkan nilai untuk node {i+1}: ")
    ll.append(data)

print("\nLinked List setelah input:")
ll.display()
