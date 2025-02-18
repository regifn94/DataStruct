# Definisi Node
class Node:
    def __init__(self, data):
        self.data = data
        self.next = None

# Definisi Linked List
class LinkedList:
    def __init__(self):
        self.head = None

    # Menambahkan node di awal linked list
    def insert_at_beginning(self, data):
        new_node = Node(data)  # Membuat node baru
        new_node.next = self.head  # Node baru menunjuk ke head saat ini
        self.head = new_node  # Head sekarang adalah node baru

    # Menambahkan node di akhir linked list
    def insert_at_end(self, data):
        new_node = Node(data)  # Membuat node baru
        if self.head is None:  # Jika linked list kosong
            self.head = new_node
            return
        last = self.head
        while last.next: 
            last = last.next
        last.next = new_node

    # Menampilkan isi linked list
    def display(self):
        temp = self.head
        while temp:  # Iterasi melalui setiap node
            print(temp.data, end=" -> ")
            temp = temp.next
        print("None")

llist = LinkedList()
llist.insert_at_beginning(5)
llist.insert_at_end(10)
llist.insert_at_end(20)
llist.display()  # Output: 5 -> 10 -> 20 -> None