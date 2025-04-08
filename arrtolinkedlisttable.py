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

    def insert_at_beginning(self, data):
        new_node = Node(data)
        new_node.next = self.head
        self.head = new_node

    def insert_at_position(self, data, position):
        if position == 0:
            self.insert_at_beginning(data)
            return
        new_node = Node(data)
        current = self.head
        for _ in range(position - 1):
            if current is None:
                raise Exception("Position out of bounds")
            current = current.next
        new_node.next = current.next
        current.next = new_node

    def delete_node(self, key):
        current = self.head
        if current is not None:
            if current.data == key:
                self.head = current.next
                current = None
                return
        while current is not None:
            if current.data == key:
                break
            prev = current
            current = current.next
        if current == None:
            return
        prev.next = current.next
        current = None

    def search(self, key):
        current = self.head
        while current:
            if current.data == key:
                return True
            current = current.next
        return False

    def reverse(self):
        prev = None
        current = self.head
        while current:
            next_node = current.next
            current.next = prev
            prev = current
            current = next_node
        self.head = prev

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
