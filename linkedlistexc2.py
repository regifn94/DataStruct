class Node:
    def __init__(self, data=None):
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

    def insert_after(self, prev_node_data, new_data):
        temp = self.head
        while temp and temp.data != prev_node_data:
            temp = temp.next
        if not temp:
            print("Node tidak ditemukan!")
            return
        new_node = Node(new_data)
        new_node.next = temp.next
        temp.next = new_node

    def display(self):
        temp = self.head
        while temp:
            print(temp.data, end=" -> ")
            temp = temp.next
        print("None")

# Program utama
ll = LinkedList()
print("Masukkan 4 nilai untuk Linked List:")
for i in range(4):
    data = input(f"Masukkan nilai untuk node {i+1}: ")
    ll.append(data)

ll.display()

# Memasukkan nilai di tengah (setelah node ke-2)
insert_value = input("\nMasukkan nilai untuk disisipkan setelah node ke-2: ")
second_node_data = ll.head.next.data  # Mengambil nilai node ke-2
third_node_data = ll.head.next.next.data # Mengambil nilai node ke-3
ll.insert_after(third_node_data, insert_value)

print("\nLinked List setelah penyisipan:")
ll.display()