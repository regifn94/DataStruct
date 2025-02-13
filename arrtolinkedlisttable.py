from tabulate import tabulate

class Node:
    def __init__(self, data):
        self.data = data
        self.next = None

class LinkedList:
    def __init__(self):
        self.head = None

    def append(self, data):
        new_node = Node(data)
        if not self.head:
            self.head = new_node
            return
        last = self.head
        while last.next:
            last = last.next
        last.next = new_node

    def display(self):
        temp = self.head
        data_list = []
        
        while temp:
            data_list.append([temp.data, hex(id(temp)), hex(id(temp.next)) if temp.next else "None"])
            temp = temp.next
        
        print(tabulate(data_list, headers=["Data", "Alamat Node", "Alamat Next"], tablefmt="grid"))

arr = [2, 4, 5, 6, 10]
linked_list = LinkedList()
for num in arr:
    linked_list.append(num)

linked_list.display()
